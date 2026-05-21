add_modules_library(core SHARED)
add_modules_library(input)
add_modules_library(scene)

target_link_libraries(core PUBLIC definitions math io memory)
target_link_libraries(input PRIVATE SDL3::SDL3-static)
target_link_libraries(scene PUBLIC renderable)

add_subdirectory(thirdparty SYSTEM)
add_subdirectory(platform)
add_subdirectory(rendering)
