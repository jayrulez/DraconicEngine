module;

#include <vector>
#include <cmath>
#include <unordered_map>

module rendering.mesh;

import core.stdtypes;
import core.math.constants;
import core.memory;
import rendering.rhi;
import rendering.rhi.vertex;

namespace draco::rendering::mesh
{
    using namespace draco::rendering;

    static std::unordered_map<usize, MeshHandle> g_mesh_cache;
    static core::memory::HandleRegistry<Mesh, MeshTag> g_meshes;
    static rhi::LayoutHandle g_mesh_layout = rhi::InvalidLayout;

    static usize hashCombine(usize a, usize b)
    {
        return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
    }

    static usize hashMeshParams(int a, int b = 0, f32 c = 0.0f)
    {
        usize h1 = std::hash<int>{}(a);
        usize h2 = std::hash<int>{}(b);
        usize h3 = std::hash<f32>{}(c);

        return hashCombine(hashCombine(h1, h2), h3);
    }

    static void ensureMeshLayout()
    {
        if (g_mesh_layout != rhi::InvalidLayout)
            return;
        
        rhi::VertexLayoutDesc desc;
        desc.elements =
        {
            { rhi::Attrib::Position, 3, rhi::AttribType::Float },
            { rhi::Attrib::Normal,   3, rhi::AttribType::Float },
            { rhi::Attrib::TexCoord0,2, rhi::AttribType::Float }
        };

        g_mesh_layout = rhi::createVertexLayout(desc);
    }

    MeshHandle create(const void* vertex_data, u32 vertex_size, u32 vertex_count, const std::vector<u32>& indices, rhi::LayoutHandle layout)
    {
        Mesh mesh{};

        mesh.vbh = rhi::createVertexBuffer(vertex_data, vertex_size, layout);
        mesh.ibh = rhi::createIndexBuffer(indices.data(), static_cast<u32>(indices.size() * sizeof(u32)));

        mesh.layout = layout;

        mesh.vertexCount = vertex_count;
        mesh.indexCount = static_cast<u32>(indices.size());

        mesh.valid = (mesh.vbh != rhi::InvalidBuffer) && (mesh.ibh != rhi::InvalidBuffer);

        if (!mesh.valid)
        {
            if (mesh.vbh != rhi::InvalidBuffer) rhi::destroyBuffer(mesh.vbh);
            if (mesh.ibh != rhi::InvalidBuffer) rhi::destroyBuffer(mesh.ibh);
            return {};
        }
        return g_meshes.create(mesh);
    }

    MeshHandle createCube()
    {
        ensureMeshLayout();

        usize key = 1;

        if (auto it = g_mesh_cache.find(key); it != g_mesh_cache.end())
            return it->second;

        auto v = gen::cubeVertices();
        auto i = gen::cubeIndices();

        MeshHandle h = create(v.data(), v.size()*sizeof(Vertex), (u32)v.size(), i, g_mesh_layout);

        g_mesh_cache[key] = h;
        return h;
    }

    MeshHandle createPlane(f32 size)
    {
        ensureMeshLayout();

        usize key = hashMeshParams(1000, 0, size);

        if (auto it = g_mesh_cache.find(key); it != g_mesh_cache.end())
            return it->second;

        auto v = gen::planeVertices(size);
        auto i = gen::planeIndices();

        MeshHandle h = create(v.data(), v.size()*sizeof(Vertex), (u32)v.size(), i, g_mesh_layout);

        g_mesh_cache[key] = h;
        return h;
    }

    MeshHandle createSphere(int segments, int rings)
    {
        if (segments < 3 || rings < 2)
            return {};

        ensureMeshLayout();

        usize key = hashCombine(std::hash<int>{}(segments), std::hash<int>{}(rings));

        if (auto it = g_mesh_cache.find(key); it != g_mesh_cache.end())
            return it->second;

        auto v = gen::sphereVertices(segments, rings);
        auto i = gen::sphereIndices(segments, rings);

        MeshHandle h = create(v.data(), v.size()*sizeof(Vertex), (u32)v.size(), i, g_mesh_layout);

        g_mesh_cache[key] = h;
        return h;
    }

    MeshHandle createCylinder(int segments, f32 height)
    {
        if (segments < 3 || height < 0.0f)
            return {};
        
        ensureMeshLayout();

        usize key = hashMeshParams(2000, segments, height);

        if (auto it = g_mesh_cache.find(key); it != g_mesh_cache.end())
            return it->second;

        auto v = gen::cylinderVertices(segments, height);
        auto i = gen::cylinderIndices(segments);

        MeshHandle h = create(v.data(), v.size()*sizeof(Vertex), (u32)v.size(), i, g_mesh_layout);

        g_mesh_cache[key] = h;
        return h;
    }

    MeshHandle createCapsule(int segments, int rings, f32 height)
    {
        if (segments < 3 || rings < 2 || height < 0.0f)
            return {};
        
        ensureMeshLayout();
        usize key = hashCombine(hashCombine(std::hash<int>{}(segments), std::hash<int>{}(rings)), std::hash<f32>{}(height));

        if (auto it = g_mesh_cache.find(key); it != g_mesh_cache.end())
            return it->second;

        auto v = gen::capsuleVertices(segments, rings, height);
        auto i = gen::capsuleIndices(segments, rings);

        MeshHandle h = create(v.data(), v.size()*sizeof(Vertex), (u32)v.size(), i, g_mesh_layout);

        g_mesh_cache[key] = h;
        return h;
    } 

    void destroy(MeshHandle handle)
    {
        auto* mesh = g_meshes.get(handle);
        if (!mesh) return;

        rhi::destroyBuffer(mesh->vbh);
        rhi::destroyBuffer(mesh->ibh);

        // Remove from cache
        for (auto it = g_mesh_cache.begin(); it != g_mesh_cache.end(); )
        {
            if (it->second == handle)
                it = g_mesh_cache.erase(it);
            else
                ++it;
        }

        g_meshes.destroy(handle);
    }

    const Mesh* get(MeshHandle handle)
    {
        return g_meshes.get(handle);
    }
}

namespace draco::rendering::mesh::gen
{
    Vertex make(f32 px, f32 py, f32 pz, f32 nx, f32 ny, f32 nz, f32 u, f32 v)
    {
        return { px, py, pz, nx, ny, nz, u, v };
    }

    std::vector<Vertex> cubeVertices()
    {
        return {
            make(-1,-1, 1, 0,0,1, 0,0),
            make( 1,-1, 1, 0,0,1, 1,0),
            make( 1, 1, 1, 0,0,1, 1,1),
            make(-1, 1, 1, 0,0,1, 0,1),

            make( 1,-1,-1, 0,0,-1, 0,0),
            make(-1,-1,-1, 0,0,-1, 1,0),
            make(-1, 1,-1, 0,0,-1, 1,1),
            make( 1, 1,-1, 0,0,-1, 0,1),

            make(-1,-1,-1,-1,0,0, 0,0),
            make(-1,-1, 1,-1,0,0, 1,0),
            make(-1, 1, 1,-1,0,0, 1,1),
            make(-1, 1,-1,-1,0,0, 0,1),

            make( 1,-1, 1, 1,0,0, 0,0),
            make( 1,-1,-1, 1,0,0, 1,0),
            make( 1, 1,-1, 1,0,0, 1,1),
            make( 1, 1, 1, 1,0,0, 0,1),

            make(-1, 1, 1, 0,1,0, 0,0),
            make( 1, 1, 1, 0,1,0, 1,0),
            make( 1, 1,-1, 0,1,0, 1,1),
            make(-1, 1,-1, 0,1,0, 0,1),

            make(-1,-1,-1, 0,-1,0, 0,0),
            make( 1,-1,-1, 0,-1,0, 1,0),
            make( 1,-1, 1, 0,-1,0, 1,1),
            make(-1,-1, 1, 0,-1,0, 0,1),
        };
    }

    std::vector<u32> cubeIndices()
    {
        return {
            0,1,2, 2,3,0,
            4,5,6, 6,7,4,
            8,9,10, 10,11,8,
            12,13,14, 14,15,12,
            16,17,18, 18,19,16,
            20,21,22, 22,23,20
        };
    }

    std::vector<Vertex> planeVertices(f32 size)
    {
        f32 s = size * 0.5f;

        return {
            make(-s,0,-s, 0,1,0, 0,0),
            make( s,0,-s, 0,1,0, 1,0),
            make( s,0, s, 0,1,0, 1,1),
            make(-s,0, s, 0,1,0, 0,1),
        };
    }

    std::vector<u32> planeIndices()
    {
        return { 0,1,2, 2,3,0 };
    }

    std::vector<Vertex> sphereVertices(int segments, int rings)
    {
        std::vector<Vertex> v;

        for (int y = 0; y <= rings; y++)
        {
            f32 v01 = (f32)y / rings;
            f32 theta = v01 * draco::math::PI;

            for (int x = 0; x <= segments; x++)
            {
                f32 u01 = (f32)x / segments;
                f32 phi = u01 * 2.0f * draco::math::PI;

                f32 px = sinf(theta) * cosf(phi);
                f32 py = cosf(theta);
                f32 pz = sinf(theta) * sinf(phi);

                v.push_back(make(px,py,pz, px,py,pz, u01,v01));
            }
        }

        return v;
    }

    std::vector<u32> sphereIndices(int segments, int rings)
    {
        std::vector<u32> i;

        for (int y = 0; y < rings; y++)
        {
            for (int x = 0; x < segments; x++)
            {
                int a = y * (segments + 1) + x;
                int b = a + segments + 1;

                i.push_back(a);
                i.push_back(b);
                i.push_back(a + 1);

                i.push_back(b);
                i.push_back(b + 1);
                i.push_back(a + 1);
            }
        }

        return i;
    }

    std::vector<Vertex> cylinderVertices(int segments, f32 height)
    {
        std::vector<Vertex> v;
        f32 half = height * 0.5f;

        // Side walls (Outward normals)
        for (int y = 0; y <= 1; y++) {
            f32 py = (y ? half : -half);
            for (int x = 0; x <= segments; x++) {
                f32 t = (f32)x / segments;
                f32 a = t * 2.0f * draco::math::PI;
                f32 cx = cosf(a);
                f32 cz = sinf(a);
                // Normal is strictly horizontal for side walls
                v.push_back(make(cx, py, cz, cx, 0, cz, t, (f32)y));
            }
        }

        // Top cap (Upward normals)
        // Center vertex
        v.push_back(make(0, half, 0, 0, 1, 0, 0.5f, 0.5f));
        for (int x = 0; x <= segments; x++) {
            f32 t = (f32)x / segments;
            f32 a = t * 2.0f * draco::math::PI;
            v.push_back(make(cosf(a), half, sinf(a), 0, 1, 0, (cosf(a)+1)*0.5f, (sinf(a)+1)*0.5f));
        }

        // Bottom cap (Downward Normals)
        // Center vertex
        v.push_back(make(0, -half, 0, 0, -1, 0, 0.5f, 0.5f));
        for (int x = 0; x <= segments; x++) {
            f32 t = (f32)x / segments;
            f32 a = t * 2.0f * draco::math::PI;
            v.push_back(make(cosf(a), -half, sinf(a), 0, -1, 0, (cosf(a)+1)*0.5f, (sinf(a)+1)*0.5f));
        }

        return v;
    }

    std::vector<u32> cylinderIndices(int segments)
    {
        std::vector<u32> i;
        int side_start = 0;
        int top_start = (segments + 1) * 2;
        int bottom_start = top_start + (segments + 2);

        // Sides
        for (int s = 0; s < segments; s++) {
            int b0 = s; int b1 = s + 1;
            int t0 = b0 + segments + 1; int t1 = b1 + segments + 1;
            i.push_back(b0); i.push_back(t0); i.push_back(t1);
            i.push_back(b0); i.push_back(t1); i.push_back(b1);
        }

        // Top Cap (Triangle Fan style)
        for (int s = 0; s < segments; s++) {
            i.push_back(top_start); // Center
            i.push_back(top_start + s + 2);
            i.push_back(top_start + s + 1);
        }

        // Bottom Cap
        for (int s = 0; s < segments; s++) {
            i.push_back(bottom_start); // Center
            i.push_back(bottom_start + s + 1);
            i.push_back(bottom_start + s + 2);
        }
        return i;
    }

    std::vector<Vertex> capsuleVertices(int segments, int rings, f32 height)
    {
        std::vector<Vertex> v;
        f32 half = height * 0.5f;

        // One continuous loop from bottom pole to top pole
        // Total rings for a capsule = rings (bottom cap) + rings (top cap)
        for (int r = 0; r <= rings; r++) {
            f32 v_uv = (f32)r / rings;
            f32 theta = v_uv * draco::math::PI; // 0 to draco::math::PI
            
            // Adjust Y for the cylinder section
            f32 y_offset = (theta < draco::math::PI * 0.5f) ? half : -half;

            for (int s = 0; s <= segments; s++) {
                f32 u_uv = (f32)s / segments;
                f32 phi = u_uv * 2.0f * draco::math::PI;

                f32 nx = sinf(theta) * cosf(phi);
                f32 ny = cosf(theta);
                f32 nz = sinf(theta) * sinf(phi);

                v.push_back(make(nx, ny + y_offset, nz, nx, ny, nz, u_uv, v_uv));
            }
        }
        return v;
    }

    std::vector<u32> capsuleIndices(int segments, int rings)
    {
        std::vector<u32> i;
        for (int r = 0; r < rings; r++) {
            for (int s = 0; s < segments; s++) {
                int a = r * (segments + 1) + s;
                int b = a + segments + 1;
                i.push_back(a); i.push_back(b); i.push_back(a + 1);
                i.push_back(b); i.push_back(b + 1); i.push_back(a + 1);
            }
        }
        return i;
    }
}
