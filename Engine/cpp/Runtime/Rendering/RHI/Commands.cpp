module;

#include <vector>
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include "macros.h"

module rendering.rhi;

import core.stdtypes;
import core.math.constants;

namespace draco::rendering::rhi
{
    void perspective(f32* out, f32 fov, f32 aspect, f32 nearp, f32 farp)
    {
        bx::mtxProj(out, fov, aspect, nearp, farp, bgfx::getCaps()->homogeneousDepth);
    }

    void lookAt(f32* out, const f32* eye, const f32* at, const f32* up)
    {
        bx::Vec3 eye_v { eye[0], eye[1], eye[2] };
        bx::Vec3 at_v  { at[0],  at[1],  at[2]  };
        bx::Vec3 up_v  { up[0],  up[1],  up[2]  };

        bx::mtxLookAt(out, eye_v, at_v, up_v);
    }

    // Note: Internal use only, use apply_view() instead
    void setViewRect(ViewID view, u16 x, u16 y, u16 w, u16 h)
    {
       bgfx::setViewRect(view, x, y, w, h);
    }

    // Note: Internal use only, use apply_view() instead
    void setViewFramebuffer(ViewID view, FramebufferHandle h)
    {
        auto* fb = getChecked(g_framebuffers, h, "Framebuffer");

        if (!fb)
            return;

        bgfx::setViewFrameBuffer(view, fb->fbh);
    }

    void setViewProjection(ViewID view, const f32* view_mtx, const f32* proj_mtx)
    {
        bgfx::setViewTransform(view, view_mtx, proj_mtx);
    }

    void setScissor(const ScissorRect& r)
    {
        if (!r.enabled)
            bgfx::setScissor(math::UINT16_MAX_VAL);
        else
            bgfx::setScissor(r.x, r.y, r.w, r.h);
    }

    void setStencil(u32 fstencil, u32 bstencil)
    {
        bgfx::setStencil(fstencil, bstencil);
    }

    void applyView(ViewID view, const ViewDesc& desc)
    {
        if (desc.fb != InvalidFramebuffer)
        {
            auto* fb = getChecked(g_framebuffers, desc.fb, "Framebuffer");

            if (fb && bgfx::isValid(fb->fbh))
            {
                bgfx::setViewFrameBuffer(view, fb->fbh);
            }
            else
            {
                RHI_WARN(false, "Framebuffer invalid at apply_view");
            }
        }

        bgfx::setViewRect(view, desc.x, desc.y, desc.w, desc.h);

        if (desc.clearFlags != 0)
        {
            bgfx::setViewClear(view, desc.clearFlags, desc.clearColor);
        }
    }

    void identityMatrix(f32* mtx)
    {
        bx::mtxIdentity(mtx);
    }

    void submit(const RenderPacket& p, ViewID view)
    {
        auto* pipeline = getChecked(g_pipelines, p.pipeline, "Pipeline");
        auto* vb = getChecked(g_buffers, p.vertexBuffer, "VertexBuffer");
        Buffer* ib = nullptr;

        if (!pipeline || !vb)
            return;
        
        if (p.indexBuffer != InvalidBuffer)
            ib = getChecked(g_buffers, p.indexBuffer, "IndexBuffer");

        // Transform matrix (model)
        bgfx::setTransform(p.model);

        // Vertex buffer binding with explicit range control
        if (vb->isDynamic)
        {
            // If count is UINT32_MAX, bgfx will fallback to drawing the full buffer automatically
            bgfx::setVertexBuffer(0, vb->dvbh, 0, p.vertexCount);
        } else {
            bgfx::setVertexBuffer(0, vb->vbh, 0, p.vertexCount);
        }

        // Index buffer binding with explicit range control
        if (ib && ib->isIndex)
        {
            if (ib->isDynamic)
            {
                bgfx::setIndexBuffer(ib->dibh, 0, p.indexCount);
            } else {
                bgfx::setIndexBuffer(ib->ibh, 0, p.indexCount);
            }
        }

        // Uniforms
        for (const auto& u : p.uniforms)
        {
            if (auto* handle = getChecked(g_uniforms, u.handle, "UniformBind"))
            {
                bgfx::setUniform(*handle, u.data, u.num);
            }
        }

        // Texture binding
        if (auto* tex = getChecked(g_textures, p.textureHandle, "Texture"))
        {
            if (auto* sampler = getChecked(g_uniforms, p.samplerUniform, "Sampler"))
            {
                bgfx::setTexture(p.textureUnit, *sampler, *tex, p.samplerFlags);
            }
        }

        // Apply pipeline state & submit draw call
        bgfx::setState(pipeline->state);
        bgfx::submit(view, pipeline->program);
    }
}
