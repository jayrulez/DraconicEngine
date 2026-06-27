module;

#include <vector>
#include <string>

export module rendering.rendergraph;

import core.stdtypes;
import rendering.rhi;

export namespace draco::rendering::rendergraph {

    enum class PassType : u8
    {
        Graphics,
        Transparent,
        Shadow,
        PostProcess,
        UI
    };

    enum class SortMode : u8
    {
        None,
        Material,
        FrontToBack,
        BackToFront
    };

    struct Pass
    {
        std::string name;

        PassType type = PassType::Graphics;
        SortMode sortMode = SortMode::Material;

        std::vector<std::string> dependencies;

        rhi::ViewID view = 0;
        rhi::FramebufferHandle framebuffer = rhi::InvalidFramebuffer;

        std::vector<rhi::RenderPacket> packets;

        f32 viewMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        f32 projMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        u16 width = 0;
        u16 height = 0;

        u32 clearFlags = 0;
        u32 clearColor = 0;
    };

    class RenderGraph
    {
    public:
        void reset();

        Pass& addPass(const std::string& name);

        Pass* getPass(const std::string& name);

        void execute();

    private:
        std::vector<Pass> passes;
    };
}
