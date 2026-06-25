add_modules_library(Definitions PIC)
add_modules_library(Math)
add_modules_library(IO)
add_modules_library(Memory)

target_link_libraries(Math PUBLIC Definitions bx)
target_link_libraries(IO PUBLIC Definitions stb_image)
target_link_libraries(Memory PUBLIC Definitions Math)
