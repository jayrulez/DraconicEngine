add_modules_library(Core PIC)
add_modules_library(Shell)
add_modules_library(Shell/Null)
add_modules_library(Shell/Desktop)
add_modules_library(Rendering/Shaders)
add_modules_library(Rendering/RHI PIC)
add_modules_library(Rendering/RHI/Null)
add_modules_library(Rendering/RHI/Validation)
add_modules_library(Rendering/RHI/Vulkan)
if(WIN32)
    add_modules_library(Rendering/RHI/DX12)
endif()
add_modules_library(Rendering/Shaders/System)
add_modules_library(Rendering/Graphics)
add_modules_library(Rendering/RenderGraph)
add_modules_library(Rendering/Image)
add_modules_library(Rendering/Image/IO)
add_modules_library(Rendering/Texture)

target_link_libraries(Core PUBLIC Definitions Math IO Memory)
target_link_libraries(Shell PUBLIC Core)
# Rendering: RHI is the backend-agnostic interface module; Null is the headless
# no-op backend (for tests/tools). GPU backends (Vulkan, DX12) come later.
# Shaders: HLSL compilation via DXC. DXC's headers come from Draco::DXCHeaders
# (which also carries DRACO_DXC_PATH); the runtime library is dlopen'd, so only
# the platform's dl library is linked.
target_link_libraries(Rendering_Shaders PUBLIC Core Draco::DXCHeaders PRIVATE ${CMAKE_DL_LIBS})
# ShaderSystem: variant compile-on-demand + cache. Needs the RHI (to create GPU
# modules) on top of the RHI-free shaders module. Its test uses the Null backend.
target_link_libraries(Rendering_Shaders_System PUBLIC Rendering_Shaders Rendering_RHI Rendering_RHI_Null)
target_link_libraries(Rendering_RHI PUBLIC Core)
target_link_libraries(Rendering_RHI_Null PUBLIC Rendering_RHI)
# Validation wraps any backend; its test wraps the Null backend, so Null is
# PUBLIC here to give the test the rhi.null module interface.
target_link_libraries(Rendering_RHI_Validation PUBLIC Rendering_RHI Rendering_RHI_Null)
# Vulkan GPU backend. The Vulkan loader is PRIVATE: VkIncludes.h/vulkan.h live in
# the module's global fragment, so importers of rhi.vk don't need the headers, but
# the static lib's loader symbols still propagate to the final link.
target_link_libraries(Rendering_RHI_Vulkan PUBLIC Rendering_RHI PRIVATE Vulkan::Vulkan)
# DX12 GPU backend (Windows only). Links the D3D12/DXGI system libraries privately;
# IID_PPV_ARGS uses __uuidof, which Clang flags as a language extension.
if(WIN32)
    target_link_libraries(Rendering_RHI_DX12 PUBLIC Rendering_RHI PRIVATE d3d12 dxgi dxguid d3dcompiler)
    target_compile_options(Rendering_RHI_DX12 PRIVATE $<$<CXX_COMPILER_ID:Clang>:-Wno-language-extension-token>)
endif()
# Shell backends: Null is headless (interface only); Desktop is SDL3-based.
# SDL3 is PUBLIC on Desktop because SDL types (SDL3Window::handle()) surface in
# its exported module interface, so consumers (incl. its tests) need SDL headers.
target_link_libraries(Shell_Null PUBLIC Shell)
target_link_libraries(Shell_Desktop PUBLIC Shell SDL3::SDL3-static)
# Graphics render host: one target holding the graphics / graphics.null / graphics.gpu
# modules. The core `graphics` interface imports only base RHI + Shell; graphics.gpu
# pulls the backends. Shell_Null is PUBLIC for the headless test. DX12 is added
# (with DRACO_HAS_DX12) only on Windows.
# RenderGraph: pass dependency resolution, transient aliasing, automatic barriers
# over the RHI. Backend-agnostic (base RHI only); Null is PUBLIC for its headless tests.
target_link_libraries(Rendering_RenderGraph PUBLIC Core Rendering_RHI Rendering_RHI_Null)
# Image: CPU-side image data + manipulation (no RHI). IO adds stb load/save; stb is
# PRIVATE (its headers live in the module's global fragment / the stb_impl TU).
target_link_libraries(Rendering_Image PUBLIC Core)
target_link_libraries(Rendering_Image_IO PUBLIC Rendering_Image Core PRIVATE stb_image)
# Texture: image->RHI format descriptors (bridges CPU image data and the GPU RHI).
target_link_libraries(Rendering_Texture PUBLIC Core Rendering_Image Rendering_RHI)
target_link_libraries(Rendering_Graphics PUBLIC
    Rendering_RHI Rendering_RHI_Null Rendering_RHI_Vulkan Rendering_RHI_Validation Shell Shell_Null)
if(WIN32)
    target_link_libraries(Rendering_Graphics PUBLIC Rendering_RHI_DX12)
    target_compile_definitions(Rendering_Graphics PUBLIC DRACO_HAS_DX12)
endif()
