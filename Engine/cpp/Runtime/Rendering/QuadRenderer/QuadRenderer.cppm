module;

#include <vector>

export module rendering.quad;

import core.stdtypes;
import rendering.rhi;
import rendering.rhi.vertex;
import rendering.rendergraph;

export namespace draco::rendering::quad {

    struct BatchKey {
        rhi::TextureHandle texture = rhi::InvalidTexture;

        rhi::PipelineHandle pipeline = rhi::InvalidPipeline;

        rhi::SamplerHandle sampler = rhi::InvalidSampler;

        [[nodiscard]] bool operator==(const BatchKey&) const = default;
    };

    struct QuadCommand {
        rhi::TextureHandle texture = rhi::InvalidTexture;

        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 z = 0.0f;

        f32 width  = 1.0f;
        f32 height = 1.0f;

        f32 rotation = 0.0f;

        u32 color = 0xffffffff;
    };

    struct OrthoCamera {
        f32 view[16];
        f32 proj[16];

        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 zoom = 1.0f;
    };

    class QuadRenderer {
    public:
        static constexpr u32 MaxQuads    = 10000;
        static constexpr u32 MaxVertices = MaxQuads * 4;
        static constexpr u32 MaxIndices  = MaxQuads * 6;

        void init(draco::rendering::rhi::PipelineHandle pipeline);

        void begin();

        void submit(const QuadCommand& cmd);

        void flushToPass(rendergraph::Pass& pass);

        void shutdown();

        static void buildOrtho(OrthoCamera& cam, f32 width, f32 height);

    private:
        void pushQuad(const QuadCommand& cmd);

        BatchKey m_batch_key{};

        std::vector<rhi::TexturedVertex> m_vertices;
        std::vector<u16> m_indices;

        rhi::BufferHandle m_vb = rhi::InvalidBuffer;
        rhi::BufferHandle m_ib = rhi::InvalidBuffer;
        rhi::LayoutHandle m_layout = rhi::InvalidLayout;
        rhi::PipelineHandle m_pipeline = rhi::InvalidPipeline;
        rhi::UniformHandle m_sampler = rhi::InvalidUniform;
        u32 m_quad_count = 0;
    };
}
