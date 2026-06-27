module;

#include <array>

export module rendering.renderer;

import core.stdtypes;
import core.math.transform;
import rendering.rhi;
import rendering.rendergraph;
import rendering.quad;
import rendering.material;
import rendering.mesh;

export namespace draco::rendering::renderer {

    struct Camera {
        std::array<f32, 3> position = {0.0f, 0.0f, 0.0f};
        std::array<f32, 3> target = {0.0f, 0.0f, 0.0f};
        std::array<f32, 3> up = {0.0f, 1.0f, 0.0f};
        f32 fov = 60.0f;
        f32 nearPlane = 0.1f;
        f32 farPlane = 1000.0f;
    };

    struct SceneContext {
        u16 screenWidth = 0;
        u16 screenHeight = 0;
        Camera mainCamera;

        rendergraph::RenderGraph graph;
    };

    inline SceneContext g_ctx;

    void init(u16 width, u16 height);
    void resize(u16 width, u16 height);

    void beginFrame(const Camera& cam);

    void submitEntity(rhi::RenderPacket& packet, u16 view);
    void submitRenderable(const math::Transform& transform, const material::Material& material, mesh::MeshHandle mesh_id);
    void submitUI(quad::QuadRenderer& quad_renderer);

    void endFrame();

    rendergraph::RenderGraph& getGraph();
}
