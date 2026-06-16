add_modules_library(Definitions PIC)
add_modules_library(math)
add_modules_library(IO)
add_modules_library(memory)

target_link_libraries(math PUBLIC Definitions bx)
target_link_libraries(IO PUBLIC Definitions stb_image)
target_link_libraries(memory PUBLIC Definitions math)
