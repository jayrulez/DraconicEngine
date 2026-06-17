add_modules_library(camera)
add_modules_library(transform_component)
add_modules_library(renderable)

target_link_libraries(camera PUBLIC core Input rendering bx)
target_link_libraries(transform_component PUBLIC core)
target_link_libraries(renderable PUBLIC core mesh transform_component material)
