module;

#include <bgfx/bgfx.h>
#include "macros.h"

module rendering.rhi;

import core.stdtypes;
import core.math.constants;

namespace draco::rendering::rhi
{
        UniformHandle createUniform(const char* name, UniformType type, u16 num)
    {
        RHI_ASSERT(name != nullptr, "Uniform name is null");

        auto u = bgfx::createUniform(name, map_uniform_type(type), num);
        return g_uniforms.create(u);
    }

    void setUniform(UniformHandle h, const void* data, u16 num)
    {
        auto* u = getChecked(g_uniforms, h, "Uniform");
        if (!u) return;

        RHI_ASSERT(data != nullptr, "Uniform data is null");

        bgfx::setUniform(*u, data, num);
    }

    void destroyUniform(UniformHandle h)
    {
        auto* u = getChecked(g_uniforms, h, "Uniform");
        if (!u) return;

        destroyLater(*u);
        g_uniforms.destroy(h);
    }

    TextureHandle createTexture(const void* data, u32 w, u32 h, u32 flags)
    {
        RHI_ASSERT(data != nullptr, "Texture data is null");
        RHI_ASSERT(w > 0 && h > 0, "Invalid texture dimensions");

        auto tex = bgfx::createTexture2D(
            w, h, false, 1,
            bgfx::TextureFormat::RGBA8,
            flags == 0 ? (BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP) : flags,
            bgfx::copy(data, w * h * 4)
        );

        return g_textures.create(tex);
    }

    void destroyTexture(TextureHandle h)
    {
        auto* tex = getChecked(g_textures, h, "Texture");
        if (!tex) return;

        destroyLater(*tex);
        g_textures.destroy(h);
    }

    FramebufferHandle createFramebuffer(u32 width, u32 height, TextureFormat format)
    {
        // We set render target flags so it can be attached to a framebuffer object
        u64 flags = BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

        bgfx::TextureFormat::Enum bgfx_format = bgfx::TextureFormat::RGBA8; 
        
        bgfx::TextureHandle th = bgfx::createTexture2D(static_cast<u16>(width),  static_cast<u16>(height),  false, 1, bgfx_format, flags);

        RHI_ASSERT(bgfx::isValid(th), "Failed to allocate backing texture for Framebuffer");

        TextureHandle color_tex_h = g_textures.create(th);

        bgfx::FrameBufferHandle fbh = bgfx::createFrameBuffer(1, &th, false);
        
        if (!bgfx::isValid(fbh))
        {
            RHI_WARN(false, "Failed to construct native bgfx Framebuffer target!");
            // Roll back the allocated texture if the framebuffer generation bricks
            destroyTexture(color_tex_h);
            return InvalidFramebuffer;
        }

        FramebufferResource res{};
        res.fbh = fbh;
        res.texture = color_tex_h;

        return g_framebuffers.create(res);
    }

    void destroyFramebuffer(FramebufferHandle handle)
    {
        if (auto* fb = getChecked(g_framebuffers, handle, "Framebuffer"))
        {
            // Safely queue the native hardware framebuffer destruction 2 frames out
            destroyLater(fb->fbh);

            // Clean up the associated internal texture resource using existing pipelines
            if (fb->texture != InvalidTexture)
            {
                if (auto* th = g_textures.get(fb->texture))
                {
                    destroyLater(*th);
                }
                g_textures.destroy(fb->texture);
            }

            // Evict our registry tracking slot
            g_framebuffers.destroy(handle);
        }
    }

    TextureHandle getFramebufferTexture(FramebufferHandle handle)
    {
        auto* fb = getChecked(g_framebuffers, handle, "Framebuffer");
        if (!fb)
        {
            return InvalidTexture;
        }
        
        return fb->texture;
    }
}
