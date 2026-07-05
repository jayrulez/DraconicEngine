add_modules_library(Core PIC)
add_modules_library(Shell)
add_modules_library(Shell/Null)
add_modules_library(Shell/Desktop)

target_link_libraries(Core PUBLIC Definitions Math IO Memory Logging)
target_link_libraries(Shell PUBLIC Core)
target_link_libraries(Shell_Null PUBLIC Shell)
target_link_libraries(Shell_Desktop PUBLIC Shell SDL3::SDL3-static)
