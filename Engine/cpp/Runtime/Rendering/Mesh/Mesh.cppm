module;

#include <vector>

export module rendering.mesh;

import core.stdtypes;
import core.memory;
import rendering.rhi;

export namespace draco::rendering::mesh
{
    struct MeshTag {};
    
    using MeshHandle = core::memory::Handle<MeshTag>;

    struct Vertex
    {
        f32 px, py, pz;
        f32 nx, ny, nz;
        f32 u, v;
    };

    struct Mesh
    {
        rhi::BufferHandle vbh;
        rhi::BufferHandle ibh;

        rhi::LayoutHandle layout;

        u32 vertexCount = 0;
        u32 indexCount = 0;

        bool valid = false;
    };

    MeshHandle create(
        const void* vertexData,
        u32 vertexSize,
        u32 vertexCount,
        const std::vector<u32>& indices,
        rhi::LayoutHandle layout
    );

    MeshHandle createCube();
    MeshHandle createPlane(float size);
    MeshHandle createSphere(int segments, int rings);
    MeshHandle createCylinder(int segments, float height);
    MeshHandle createCapsule(int segments, int rings, float height);

    void destroy(MeshHandle mesh);
    const Mesh* get(MeshHandle mesh);
}

export namespace draco::rendering::mesh::gen
{
    std::vector<Vertex> cubeVertices();
    std::vector<u32> cubeIndices();

    std::vector<Vertex> planeVertices(float size);
    std::vector<u32> planeIndices();

    std::vector<Vertex> sphereVertices(int segments, int rings);
    std::vector<u32> sphereIndices(int segments, int rings);

    std::vector<Vertex> cylinderVertices(int segments, float height);
    std::vector<u32> cylinderIndices(int segments);

    std::vector<Vertex> capsuleVertices(int segments, int rings, float height);
    std::vector<u32> capsuleIndices(int segments, int rings);
}
