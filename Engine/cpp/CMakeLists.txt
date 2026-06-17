add_subdirectory(thirdparty SYSTEM)

add_modules_library(runtime)
target_link_libraries(runtime PUBLIC Platform core Input scene rendering)

# TODO: binding library