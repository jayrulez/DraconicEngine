add_modules_library(Definitions)
add_modules_library(Math)
add_modules_library(IO)
add_modules_library(Memory)

target_link_libraries(Math PUBLIC Definitions)
target_link_libraries(IO PUBLIC Definitions)
target_link_libraries(Memory PUBLIC Definitions Math)
