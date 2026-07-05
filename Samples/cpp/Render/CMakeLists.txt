#
# Render Sample - shell-only placeholder (no renderer yet)
#
add_executable(RenderSample RenderSample.cpp)

# The SDL3 desktop shell backend owns windowing + input; it pulls in SDL3 (PUBLIC).
target_link_libraries(RenderSample PRIVATE Shell_Desktop)

# SDL is statically linked via Shell_Desktop; force-load the whole archive so its
# constructor-registered video/input drivers are not stripped by the linker.
if(UNIX AND NOT APPLE)
    target_link_libraries(RenderSample PRIVATE Threads::Threads dl)
    target_link_options(RenderSample PRIVATE -Wl,--whole-archive $<TARGET_FILE:SDL3::SDL3-static> -Wl,--no-whole-archive)
elseif(APPLE)
    target_link_options(RenderSample PRIVATE -Wl,-force_load,$<TARGET_FILE:SDL3::SDL3-static>)
elseif(MSVC)
    target_link_options(RenderSample PRIVATE /WHOLEARCHIVE:$<TARGET_FILE:SDL3::SDL3-static>)
endif()
