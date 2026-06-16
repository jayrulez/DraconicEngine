module;

#include <vector>
#include <filesystem>

export module core.io.loader.image;

import core.stdtypes;

export namespace draco::core::io::loader::image
{
    struct ImageData
    {
        std::vector<u8> pixels{};
        u32 width = 0;
        u32 height = 0;
        u8 channels = 0;
        bool isValid = false;
    };

    // Load an image file (PNG, JPG, etc.) & decode it to raw RGBA8
    ImageData loadImage(const std::filesystem::path& path);
}