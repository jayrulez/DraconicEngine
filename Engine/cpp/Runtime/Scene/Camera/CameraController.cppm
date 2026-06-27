export module scene.camera.controller;

import core.stdtypes;
import rendering;

export namespace draco::scene
{
    struct CameraController
    {
        void init(f32 x = 0.0f, f32 y = 0.0f, f32 z = -2.0f);

        void update(f32 dt);

        [[nodiscard]] rendering::renderer::Camera getCamera() const;

    private:
        f32 x = 0.0f, y = 0.0f, z = 0.0f;
        f32 yaw = 0.0f;
        f32 pitch = 0.0f;
        f32 speed = 5.0f;
        f32 sensitivity = 0.1f;
    };
}