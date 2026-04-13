import std;

#include <SDL3/SDL.h>

import core.filesystem;

import rendering.rhi;
import rendering.rhi.vertex;

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::println("SDL init failed: {}", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Draconic Engine",
        1280, 720,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::println("Failed to create window: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    const char* driver = SDL_GetCurrentVideoDriver();
    std::println("Driver: {}", driver ? driver : "Unknown");

    void* nwh = nullptr; // Native window handle
    void* ndt = nullptr; // Native display type

#if defined(__linux__)

    SDL_PropertiesID props = SDL_GetWindowProperties(window);

    if (driver && std::string_view(driver) == "x11")
    {
        ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr); // Get the X11 display pointer
        nwh = (void*)(uintptr_t)SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0); // Get the X11 window number and cast it to a pointer
    }
    else if (driver && std::string_view(driver) == "wayland")
    {
        ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr); // Get the Wayland display pointer
        nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr); // Get the Wayland surface pointer
    }
    else {
        std::println("Unsupported video driver: {}", driver);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
#endif

    if (!nwh) {
        std::println("Failed to get native window handle");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if(!ndt) {
        std::println("Failed to get native display type");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Init the RHI with the native window handle and initial size
    if (!draco::rhi::init(ndt, nwh, 1280, 720)) {
        std::println("Failed to initialize RHI");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Geometry data for a triangle to test rendering
    // It includes both positions & colors
    draco::rhi::PosColorVertex triangle[] = {
        { 0.0f, 0.5f, 0.0f, 0xff0000ff }, 
        { 0.5f, -0.5f, 0.0f, 0xff00ff00 },
        { -0.5f, -0.5f, 0.0f, 0xffff0000}
    };

    auto vbh = draco::rhi::create_vertex_buffer(triangle, sizeof(triangle));

    // Load the vertex & fragment shaders
    auto vs_data = draco::filesystem::load_binary("vs_triangle.bin");
    auto fs_data = draco::filesystem::load_binary("fs_triangle.bin");

    // If the path is empty, return an error
    if (vs_data.empty() || fs_data.empty()) {
        std::println("Failed to load shaders");
        std::println("Working dir: {}", std::filesystem::current_path().string());
        draco::rhi::shutdown();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    auto vsh = draco::rhi::create_shader(vs_data.data(), (uint32_t)vs_data.size());
    auto fsh = draco::rhi::create_shader(fs_data.data(), (uint32_t)fs_data.size());
    
    auto pipeline = draco::rhi::create_pipeline({
    vsh, 
    fsh, 
    draco::rhi::PipelineState::WriteRGB | 
    draco::rhi::PipelineState::WriteAlpha | 
    draco::rhi::PipelineState::MSAA | 
    draco::rhi::PipelineState::PrimitiveTriStrip
    });

    // Create a uniform for a color tint
    auto u_tint = draco::rhi::create_uniform("u_tint", draco::rhi::UniformType::Vec4);

    // Create a uniform for an offset
    auto u_offset = draco::rhi::create_uniform("u_offset", draco::rhi::UniformType::Vec4);

    bool running = true;

    float time = 0.0f;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        int w, h;
        SDL_GetWindowSize(window, &w, &h);

        draco::rhi::resize(uint16_t(w), uint16_t(h));

        time += 0.01f;

        draco::rhi::begin_frame();

        // Prepare uniform data (Must be float arrays for Vec4)
        float tint[4] = { 1.0f, 0.5f, 0.2f, 1.0f }; // Warm orange
        float offset[4] = { std::sin(time), std::cos(time), 0.0f, 0.0f }; // Circular motion

        // Upload to the RHI (This prepares the state for the next submit)
        draco::rhi::set_uniform(u_tint, tint);
        draco::rhi::set_uniform(u_offset, offset);

        draco::rhi::RenderPacket packet{};
        packet.vertex_buffer = vbh;
        packet.index_buffer = draco::rhi::InvalidBuffer;
        packet.pipeline = pipeline;
        
        draco::rhi::identity_matrix(packet.model);

        // Submit uses the currently bound uniforms
        draco::rhi::submit(packet, 0);

        draco::rhi::end_frame();
    }

    draco::rhi::shutdown();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
