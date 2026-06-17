module;

#include <math.h>
#include <bx/math.h>

module scene.camera.controller;

import input;

namespace draco::scene
{
    void CameraController::init(f32 x, f32 y, f32 z)
    {
        m_x = x;
        m_y = y;
        m_z = z;

        m_yaw = 0.0f;
        m_pitch = 0.0f;

        m_speed = 5.0f;          // units per second
        m_sensitivity = 0.002f;  // mouse sensitivity
    }

    void CameraController::update(f32 dt)
    {
        m_yaw   += input::getMouseDx() * m_sensitivity;
        m_pitch -= input::getMouseDy() * m_sensitivity; // Temp fix to flip mouse input

        // Clamp pitch
        if (m_pitch > 1.5f)  m_pitch = 1.5f;
        if (m_pitch < -1.5f) m_pitch = -1.5f;

        bx::Vec3 forward = {
            cosf(m_pitch) * sinf(m_yaw),
            sinf(m_pitch),
            cosf(m_pitch) * cosf(m_yaw)
        };

        bx::Vec3 right = {
            sinf(m_yaw - bx::kPiHalf),
            0.0f,
            cosf(m_yaw - bx::kPiHalf)
        };

        f32 velocity = m_speed * dt;

        if (input::isDown(input::Key::W))
        {
            m_x += forward.x * velocity;
            m_y += forward.y * velocity;
            m_z += forward.z * velocity;
        }

        if (input::isDown(input::Key::S))
        {
            m_x -= forward.x * velocity;
            m_y -= forward.y * velocity;
            m_z -= forward.z * velocity;
        }

        if (input::isDown(input::Key::A))
        {
            m_x += right.x * velocity;
            m_z += right.z * velocity;
        }

        if (draco::input::isDown(draco::input::Key::D))
        {
            m_x -= right.x * velocity;
            m_z -= right.z * velocity;
        }
    }

    draco::rendering::renderer::Camera CameraController::get_camera() const
    {
        bx::Vec3 forward = {
            cosf(m_pitch) * sinf(m_yaw),
            sinf(m_pitch),
            cosf(m_pitch) * cosf(m_yaw)
        };

        draco::rendering::renderer::Camera cam{};

        cam.position = { m_x, m_y, m_z };
        cam.target   = {
            m_x + forward.x,
            m_y + forward.y,
            m_z + forward.z
        };

        cam.up = { 0.0f, 1.0f, 0.0f };

        cam.fov = 60.0f;
        cam.near_plane = 0.1f;
        cam.far_plane  = 100.0f;

        return cam;
    }
}
