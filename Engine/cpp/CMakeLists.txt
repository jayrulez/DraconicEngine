add_subdirectory(ThirdParty SYSTEM)

add_modules_library(Runtime SHARED)
target_link_libraries(Runtime PUBLIC Platform Core Input Scene Rendering)

# TODO: binding library