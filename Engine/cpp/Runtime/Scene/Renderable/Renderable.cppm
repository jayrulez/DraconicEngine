export module scene.renderable;

import rendering.mesh;
import rendering.material;
import core.math.transform;

export namespace draco::scene::renderable
{
    struct Renderable
    {
        rendering::mesh::MeshHandle mesh{};
        math::Transform transform{};
        rendering::material::Material material{};
    };
}