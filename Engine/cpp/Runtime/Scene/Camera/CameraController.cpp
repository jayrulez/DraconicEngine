module;

#include <cmath>
#include <bx/math.h>

module scene.camera.controller;

import input;

namespace draco::scene
{
    void CameraController::init(f32 x, f32 y, f32 z)
    {
        x = x;
        y = y;
        z = z;

        yaw = 0.0f;
        pitch = 0.0f;

        speed = 5.0f;          // units per second
        sensitivity = 0.002f;  // mouse sensitivity
    }

    void CameraController::update(f32 dt)
    {
        using namespace input;

        yaw   += getMouseDx() * sensitivity;
        pitch -= getMouseDy() * sensitivity; // Temp fix to flip mouse input

        // Clamp pitch
        if (pitch > 1.5f)  pitch = 1.5f;
        if (pitch < -1.5f) pitch = -1.5f;

        bx::Vec3 forward = {
            cosf(pitch) * sinf(yaw),
            sinf(pitch),
            cosf(pitch) * cosf(yaw)
        };

        bx::Vec3 right = {
            sinf(yaw - bx::kPiHalf),
            0.0f,
            cosf(yaw - bx::kPiHalf)
        };

        f32 velocity = speed * dt;

        if (isDown(Key::W))
        {
            x += forward.x * velocity;
            y += forward.y * velocity;
            z += forward.z * velocity;
        }

        if (isDown(Key::S))
        {
            x -= forward.x * velocity;
            y -= forward.y * velocity;
            z -= forward.z * velocity;
        }

        if (isDown(Key::A))
        {
            x += right.x * velocity;
            z += right.z * velocity;
        }

        if (isDown(Key::D))
        {
            x -= right.x * velocity;
            z -= right.z * velocity;
        }
    }

    rendering::renderer::Camera CameraController::getCamera() const
    {
        const bx::Vec3 forward = {
            cosf(pitch) * sinf(yaw),
            sinf(pitch),
            cosf(pitch) * cosf(yaw)
        };

        rendering::renderer::Camera cam{};

        cam.position = { x, y, z };
        cam.target   = {
            x + forward.x,
            y + forward.y,
            z + forward.z
        };

        cam.up = { 0.0f, 1.0f, 0.0f };

        cam.fov = 60.0f;
        cam.nearPlane = 0.1f;
        cam.farPlane  = 100.0f;

        return cam;
    }
}
