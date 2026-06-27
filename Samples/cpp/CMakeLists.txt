#
# Rendering Sample
#
add_executable(RenderSample RenderSample.cpp)

target_link_libraries(RenderSample
        PRIVATE
        Runtime
        bgfx
        bx
        bimg
)

if(UNIX AND NOT APPLE)
    target_link_libraries(RenderSample PRIVATE SDL3::SDL3-static Threads::Threads dl)
    target_link_options(RenderSample PRIVATE -Wl,--whole-archive $<TARGET_FILE:SDL3::SDL3-static> -Wl,--no-whole-archive)
elseif(APPLE)
    target_link_libraries(RenderSample PRIVATE SDL3::SDL3-static)
    target_link_options(RenderSample PRIVATE -Wl,-force_load,$<TARGET_FILE:SDL3::SDL3-static>)
else()
    target_link_libraries(RenderSample PRIVATE SDL3::SDL3-static)
endif()

compile_shaders(RenderSample)

set(TEST_IMAGE
        "${CMAKE_SOURCE_DIR}/Docs/assets/draconic_logo_no_text.png"
)

add_custom_command(
        TARGET RenderSample POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TEST_IMAGE}
        $<TARGET_FILE_DIR:RenderSample>/test.png

        COMMENT "Copying test texture..."
)
