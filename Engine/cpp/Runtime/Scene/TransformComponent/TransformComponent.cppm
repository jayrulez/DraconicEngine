export module scene.transform_component;

import core.math.transform;

export namespace draco::scene
{
    struct TransformComponent
    {
        math::Transform local{};
        math::Transform world{};
        bool dirty = true;
    };

    void markDirty(TransformComponent& t);
}
