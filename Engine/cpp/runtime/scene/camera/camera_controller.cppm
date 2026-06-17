export module scene.camera.controller;

import core.stdtypes;
import rendering;

export namespace draco::scene
{
    struct CameraController
    {
        void init(f32 x = 0.0f, f32 y = 0.0f, f32 z = -2.0f);

        void update(f32 dt);

        rendering::renderer::Camera get_camera() const;

    private:
        // Init with default values
        f32 m_x = 0.0f, m_y = 0.0f, m_z = 0.0f;
        f32 m_yaw = 0.0f;
        f32 m_pitch = 0.0f;
        f32 m_speed = 5.0f;
        f32 m_sensitivity = 0.1f;
    };
}