add_modules_library(Core PIC)
add_modules_library(Shell)
add_modules_library(Shell/Null)
add_modules_library(Shell/Desktop)

target_link_libraries(Core PUBLIC Definitions Math IO Memory)
target_link_libraries(Shell PUBLIC Core)
# Shell backends: Null is headless (interface only); Desktop is SDL3-based.
# SDL3 is PUBLIC on Desktop because SDL types (SDL3Window::handle()) surface in
# its exported module interface, so consumers (incl. its tests) need SDL headers.
target_link_libraries(Shell_Null PUBLIC Shell)
target_link_libraries(Shell_Desktop PUBLIC Shell SDL3::SDL3-static)
