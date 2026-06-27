module;

#include <vector>
#include <bgfx/bgfx.h>
#include "macros.h"

module rendering.rhi;

import core.stdtypes;
import core.math.constants;

namespace draco::rendering::rhi
{
    PipelineHandle createPipeline(const PipelineDesc& desc)
    {
        RHI_ASSERT(desc.vs != InvalidShader, "Pipeline missing vertex shader");
        RHI_ASSERT(desc.fs != InvalidShader, "Pipeline missing fragment shader");

        bgfx::ProgramHandle prog = bgfx::createProgram(resolve(desc.vs), resolve(desc.fs), true);

        u64 state = mapState(desc.state, desc.blend, desc.depth, desc.cull, desc.depthWrite);

        return g_pipelines.create({ prog, state });
    }

    LayoutHandle createVertexLayout(const VertexLayoutDesc& desc)
    {
        bgfx::VertexLayout layout;
        layout.begin();

        for (const auto& e : desc.elements)
        {
            layout.add(map_attrib(e.attrib), e.count, map_attrib_type(e.type), e.normalized);
        }

        layout.end();

        return g_layouts.create({ layout });
    }

    ShaderHandle createShader(const void* data, u32 size)
    {
        RHI_ASSERT(data && size > 0, "Invalid shader data");

        bgfx::ShaderHandle sh = bgfx::createShader(bgfx::copy(data, size));

        return g_shaders.create(sh);
    }

    bgfx::ShaderHandle resolve(ShaderHandle h)
    {
        auto* sh = g_shaders.get(h);
        return sh ? *sh : bgfx::ShaderHandle{ bgfx::kInvalidHandle };
    }

    // For debugging/tooling
    bgfx::ShaderHandle* getShaderNative(ShaderHandle h)
    {
        return getChecked(g_shaders, h, "Shader");
    }

    void destroyShader(ShaderHandle h)
    {
        if (auto* sh = getChecked(g_shaders, h, "Shader"))
        {
            destroyLater(*sh);
            g_shaders.destroy(h);
        }
    }
}
