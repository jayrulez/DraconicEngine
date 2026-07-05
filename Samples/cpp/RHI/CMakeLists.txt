# Draconic RHI samples + their shared framework (GPU/display dependent — built,
# not run in CI).

# Shared sample framework: window bring-up (SDL3 shell), backend/device/swapchain,
# main loop, shader-compile helpers. Links every RHI backend + the shader compiler.
# DX12 is Windows-only (DRACO_HAS_DX12).
add_modules_library(Framework)
target_link_libraries(Framework PUBLIC
    Core
    Rendering_RHI
    Rendering_RHI_Vulkan
    Rendering_RHI_Validation
    Rendering_Shaders
    Shell
    Shell_Desktop)
if(WIN32)
    target_link_libraries(Framework PUBLIC Rendering_RHI_DX12)
    target_compile_definitions(Framework PUBLIC DRACO_HAS_DX12)
endif()

# Individual samples are added as they are ported.
