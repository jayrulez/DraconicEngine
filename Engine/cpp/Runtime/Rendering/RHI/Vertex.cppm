module;

#include <cstdint>
#include <vector>

export module rendering.rhi.vertex;
import core.stdtypes;

export namespace draco::rendering::rhi {
    enum class Attrib { 
        Position, 
        Color0, 
        TexCoord0, 
        Normal, 
        Tangent 
    };

    enum class AttribType { 
        Float, 
        Uint8 
    };

    struct VertexElement {
        Attrib attrib;
        u16 count;
        AttribType type;
        bool normalized = false;
    };

    struct VertexLayoutDesc {
        std::vector<VertexElement> elements;
    };

    struct alignas(u32) TexturedVertex {
        float x, y, z;
        float u, v;
        u32 color;
    };
    static_assert(sizeof(TexturedVertex) == 24);

    // Helper to get the standard layout for the current vertex struct
    inline VertexLayoutDesc getTexturedVertexLayout() {
        return {
            .elements = {
                { Attrib::Position,  3, AttribType::Float },
                { Attrib::TexCoord0, 2, AttribType::Float },
                { Attrib::Color0,    4, AttribType::Uint8, true }
            }
        };
    }
}
