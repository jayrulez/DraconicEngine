module scene.transform_component;

namespace draco::scene
{
    void markDirty(TransformComponent& t)
    {
        t.dirty = true;
    }
}
