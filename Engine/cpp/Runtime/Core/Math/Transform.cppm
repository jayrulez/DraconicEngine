export module core.math.transform;

import core.stdtypes;
import core.math.types;
import core.math.matrix4;
import core.math.quaternion;

export namespace draco::math
{
    // =======================================================================
    // Transform - position / rotation / scale, composed as S * R * T.
    // =======================================================================
    struct Transform
    {
        Vector3 position = Vector3::zero;
        Quaternion rotation = Quaternion::identity;
        Vector3 scale = Vector3::one;

        [[nodiscard]] Matrix4 toMatrix() const noexcept
        {
            Matrix4 result = Matrix4::scale(scale) * rotationMatrix(rotation);
            result.m[3][0] = position.x;
            result.m[3][1] = position.y;
            result.m[3][2] = position.z;
            return result;
        }

        // Component-wise interpolation: position/scale lerp, rotation slerp.
        [[nodiscard]] static Transform lerp(const Transform& a, const Transform& b, f32 t) noexcept
        {
            return Transform{
                draco::math::lerp(a.position, b.position, t),
                slerp(a.rotation, b.rotation, t),
                draco::math::lerp(a.scale, b.scale, t),
            };
        }
    };

    // Identity transform (position 0, rotation identity, scale 1) - the
    // default-constructed value.
    inline constexpr Transform identityTransform{};
}
