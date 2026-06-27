module;

#include <cmath>
#include <algorithm>

#include <bgfx/bgfx.h>
#include <bx/math.h>

module rendering.quad;

import core.stdtypes;
import rendering.rhi;
import rendering.rhi.vertex;
import rendering.rendergraph;

namespace draco::rendering::quad {

    static constexpr f32 QuadUV[4][2] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    void QuadRenderer::init(rhi::PipelineHandle pipeline)
    {
        using namespace draco::rendering::rhi;

        VertexLayoutDesc layout{};
        layout.elements.push_back({Attrib::Position, 3, AttribType::Float});
        layout.elements.push_back({Attrib::TexCoord0, 2, AttribType::Float});
        layout.elements.push_back({Attrib::Color0, 4, AttribType::Uint8, true});

        m_pipeline = pipeline;
        m_layout = createVertexLayout(layout);

        // Allocating dynamic streaming buffers
        m_vb = createDynamicVertexBuffer(sizeof(TexturedVertex) * MaxVertices, m_layout);
        
        // Pass BGFX_BUFFER_NONE implicitly to match tracking
        m_ib = createDynamicIndexBuffer(MaxIndices * sizeof(u16), BGFX_BUFFER_NONE);

        m_sampler = createUniform("s_texColor", UniformType::Sampler);
    }

    void QuadRenderer::begin()
    {
        m_vertices.clear();
        m_indices.clear();

        m_quad_count = 0;

        m_batch_key = {};
    }

    void QuadRenderer::submit(const QuadCommand& cmd)
    {
        if (m_quad_count >= MaxQuads)
            return;

        BatchKey new_key{cmd.texture, m_pipeline, draco::rendering::rhi::InvalidSampler};

        if (m_batch_key.texture == draco::rendering::rhi::InvalidTexture)
        {
            m_batch_key = new_key;
        }

        bool state_change = !(new_key == m_batch_key);

        if (state_change)
        {
            // TODO: Flush current batch automatically

            return;
        }

        pushQuad(cmd);

        m_quad_count++;
    }

    void QuadRenderer::pushQuad(const QuadCommand& cmd)
    {
        f32 hw = cmd.width * 0.5f;
        f32 hh = cmd.height * 0.5f;

        f32 c = cosf(cmd.rotation);
        f32 s = sinf(cmd.rotation);

        f32 corners[4][2] = {
            {-hw, -hh},
            { hw, -hh},
            { hw,  hh},
            {-hw,  hh}
        };

        u16 start = static_cast<u16>(m_vertices.size());

        for (int i = 0; i < 4; i++)
        {
            f32 rx = corners[i][0] * c - corners[i][1] * s;

            f32 ry = corners[i][0] * s + corners[i][1] * c;

            draco::rendering::rhi::TexturedVertex v{};

            v.x = cmd.x + rx;
            v.y = cmd.y + ry;
            v.z = cmd.z;

            v.u = QuadUV[i][0];
            v.v = QuadUV[i][1];

            v.color = cmd.color;

            m_vertices.push_back(v);
        }

        m_indices.push_back(start + 0);
        m_indices.push_back(start + 1);
        m_indices.push_back(start + 2);

        m_indices.push_back(start + 2);
        m_indices.push_back(start + 3);
        m_indices.push_back(start + 0);
    }

    void QuadRenderer::flushToPass(draco::rendering::rendergraph::Pass& pass)
    {
        using namespace draco::rendering::rhi;

        if (m_vertices.empty())
            return;

        // Upload only the exact slices we are using this frame
        updateDynamicVertexBuffer(m_vb, 0, m_vertices.data(), static_cast<u32>(m_vertices.size() * sizeof(TexturedVertex)));
        updateDynamicIndexBuffer(m_ib, 0, m_indices.data(), static_cast<u32>(m_indices.size() * sizeof(u16)));

        RenderPacket pkt{};
        pkt.vertexBuffer  = m_vb;
        pkt.indexBuffer   = m_ib;
        pkt.pipeline       = m_pipeline;
        pkt.textureHandle = m_batch_key.texture;
        pkt.samplerUniform = m_sampler;

        pkt.vertexCount   = static_cast<u32>(m_vertices.size());
        pkt.indexCount    = static_cast<u32>(m_indices.size());

        pkt.sortKey = makeSortKey(0, 0, static_cast<u16>(m_pipeline.value), static_cast<u16>(m_batch_key.texture.value), 0);

        bx::mtxIdentity(pkt.model);

        pass.packets.push_back(pkt);

        m_vertices.clear();
        m_indices.clear();
    }

    void QuadRenderer::shutdown()
    {
        using namespace draco::rendering::rhi;

        destroyBuffer(m_vb);
        destroyBuffer(m_ib);

        destroyUniform(m_sampler);
    }

    void QuadRenderer::buildOrtho(OrthoCamera& cam, f32 width, f32 height)
    {
        using namespace draco::rendering::rhi;

        identityMatrix(cam.view);
        identityMatrix(cam.proj);

        f32 rl = std::max(width, 1.0f);
        f32 tb = std::max(height, 1.0f);

        cam.proj[0]  =  2.0f / rl;
        cam.proj[5]  = -2.0f / tb;
        cam.proj[10] = -1.0f;

        cam.proj[12] = -1.0f;
        cam.proj[13] =  1.0f;
    }
}
