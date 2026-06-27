module;

#include <print>
#include <algorithm>

#include <bgfx/bgfx.h>

module rendering.rendergraph;

import rendering.rhi;

namespace draco::rendering::rendergraph {

    static void sortMaterial(std::vector<rhi::RenderPacket>& packets)
    {
        std::sort(packets.begin(), packets.end(),
        [](const rhi::RenderPacket& a, const rhi::RenderPacket& b)
        {
            // Pipeline first
            if (a.pipeline != b.pipeline)
                return a.pipeline.value < b.pipeline.value;

            // Texture second
            if (a.textureHandle != b.textureHandle)
                return a.textureHandle.value < b.textureHandle.value;

            // Vertex buffer third
            if (a.vertexBuffer != b.vertexBuffer)
                return a.vertexBuffer.value < b.vertexBuffer.value;

            // Index buffer fallback
            return a.indexBuffer.value < b.indexBuffer.value;
        });
    }

    // Placeholder until depth sorting exists
    static void sortFrontToBack(std::vector<rhi::RenderPacket>& packets)
    {
        sortMaterial(packets);
    }

    static void sortBackToFront(std::vector<rhi::RenderPacket>& packets)
    {
        sortMaterial(packets);
    }

    static void sortPackets(std::vector<rhi::RenderPacket>& packets, SortMode mode)
    {
        switch (mode)
        {
            case SortMode::None:
                break;

            case SortMode::Material:
                sortMaterial(packets);
                break;

            case SortMode::FrontToBack:
                sortFrontToBack(packets);
                break;

            case SortMode::BackToFront:
                sortBackToFront(packets);
                break;
        }
    }

    void RenderGraph::reset()
    {
        passes.clear(); // Directly clear
    }

    Pass& RenderGraph::addPass(const std::string& name)
    {
        passes.emplace_back();

        auto& pass = passes.back();
        pass.name = name;

        return pass;
    }

    Pass* RenderGraph::getPass(const std::string& name)
    {
        for (auto& p : passes)
        {
            if (p.name == name)
                return &p;
        }

        return nullptr;
    }

    void RenderGraph::execute()
    {
        for (auto& pass : passes)
        {
            // Future dependency handling hook
            for (const auto& dep : pass.dependencies)
            {
                (void)dep;
            }

            sortPackets(pass.packets, pass.sortMode);

            rhi::applyView(pass.view, {pass.framebuffer, 0, 0, pass.width, pass.height, pass.clearFlags, pass.clearColor});

            rhi::setViewProjection(pass.view, pass.viewMatrix, pass.projMatrix);

            if (pass.clearFlags)
            {
                bgfx::setViewClear(pass.view, pass.clearFlags, pass.clearColor);
            }

            for (auto& pkt : pass.packets)
            {
                rhi::submit(pkt, pass.view);
            }
        }
    }
}
