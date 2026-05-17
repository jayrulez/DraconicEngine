module;

#include <cstdint>
#include <cstring>
#include <cmath>

#include <bx/math.h>

export module scene.transform;

export namespace draco::scene::transform
{
    struct Transform
    {
        float position[3] = { 0.0f, 0.0f, 0.0f };
        float rotation[3] = { 0.0f, 0.0f, 0.0f }; // Euler (radians)
        float scale[3] = { 1.0f, 1.0f, 1.0f };

        bool dirty = true;
    };

    // Creates a default identity transform
    Transform make_transform();

    // Recompute matrix from transform (column-major, bx compatible)
    void compute_matrix(const Transform& t, float out[16]);

    // Helpers
    void set_position(Transform& t, float x, float y, float z);
    void set_rotation(Transform& t, float x, float y, float z);
    void set_scale(Transform& t, float x, float y, float z);

    void mark_dirty(Transform& t);
}