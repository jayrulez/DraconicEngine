add_modules_library(camera)
add_modules_library(transform)
add_modules_library(renderable)

target_link_libraries(camera PUBLIC input renderer bx)
target_link_libraries(transform PUBLIC bx)
target_link_libraries(renderable PUBLIC mesh transform material)
