module;

#include <unordered_map>
#include <string>
#include <functional>
#include <cstdint>

export module rendering.rhi.uniform_registry;

import rendering.rhi;

export namespace draco::rendering::rhi
{
    inline std::unordered_map<uint32_t, UniformHandle> g_uniform_map;

    inline uint32_t hashUniform(const std::string& name)
    {
        return static_cast<uint32_t>(std::hash<std::string>{}(name));
    }

    inline void registerUniform(uint32_t hash, UniformHandle h)
    {
        g_uniform_map[hash] = h;
    }

    inline void unregisterUniform(uint32_t hash, UniformHandle h)
    {
        auto it = g_uniform_map.find(hash);
        
        if (it != g_uniform_map.end() && it->second == h)
            g_uniform_map.erase(it);
    }

    inline void clearUniformRegistry()
    {
        g_uniform_map.clear();
    }

    inline UniformHandle getUniform(uint32_t hash)
    {
        auto it = g_uniform_map.find(hash);

        if (it == g_uniform_map.end())
            return InvalidUniform;
        
        return it->second;
    }
}
