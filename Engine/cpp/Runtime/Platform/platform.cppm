module;

#include "impl/platform_impl.h"

export module platform;

export namespace draco::platform {

    using NativeWindowType = impl::NativeWindowType;
    using NativeWindowFrame = impl::NativeWindowFrame;

    NativeWindowFrame getNativeHandles(void* sdl_window_ptr) {
        return impl::getNativeHandles(sdl_window_ptr);
    }
}
