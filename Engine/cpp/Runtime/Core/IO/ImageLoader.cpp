module;

#include <vector>
#include <filesystem>
#include <limits>
#include <print>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

module core.io.loader.image;

import core.stdtypes;

// TODO: I'm too lazy to write code so we need somethin' better

namespace draco::core::io::loader::image
{
    ImageData loadImage(const std::filesystem::path& path)
    {
        ImageData result;

        std::error_code ec;
        if (!std::filesystem::exists(path, ec) || ec) {
            std::println("Error: Image path does not exist: {}", path.string());
            return result;
        }

        int width, height, channels;
        // STBI_rgb_alpha forces the output to be 4 bytes per pixel (RGBA)
        unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (!data) {
            std::println("Error: Failed to decode image: {}", path.string());
            return result;
        }

        if (width <= 0 || height <= 0) {
            stbi_image_free(data);
            return result;
        }

        const usize w = static_cast<usize>(width);
        const usize h = static_cast<usize>(height);
        if (w > (std::numeric_limits<usize>::max() / 4) / h) {
            stbi_image_free(data);
            return result;
        }
        
        usize size = w * h * 4;
        
        result.pixels.assign(data, data + size);
        result.width = static_cast<u16>(width);
        result.height = static_cast<u16>(height);
        result.channels = 4;
        result.isValid = true;

        // Free the memory allocated by stb
        stbi_image_free(data);

        return result;
    }
}