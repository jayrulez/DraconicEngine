module;

#include <cmath>

#include <bx/math.h>

module core.math.transform;

import core.stdtypes;

namespace draco::math
{
    void Transform::toMatrix(f32 out[16]) const
    {
        f32 translation[16];
        f32 rx[16];
        f32 ry[16];
        f32 rz[16];
        f32 scale[16];
        f32 temp[16];

        bx::mtxIdentity(out);

        bx::mtxScale(scale, scale[0], scale[1], scale[2]);

        bx::mtxRotateX(rx, rotation[0]);
        bx::mtxRotateY(ry, rotation[1]);
        bx::mtxRotateZ(rz, rotation[2]);

        bx::mtxTranslate(translation, position[0], position[1], position[2]);

        // scale * rotation * translation
        bx::mtxMul(temp, scale, rx);
        bx::mtxMul(temp, temp, ry);
        bx::mtxMul(temp, temp, rz);
        bx::mtxMul(out, temp, translation);
    }
}
