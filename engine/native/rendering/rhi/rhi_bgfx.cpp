module;

import std;

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

module rendering.rhi;

namespace draco::rhi
{
    struct Buffer
    {
        bgfx::VertexBufferHandle vbh = BGFX_INVALID_HANDLE; // Stores vertex data, but it can be null if this buffer is an index buffer
        bgfx::IndexBufferHandle  ibh = BGFX_INVALID_HANDLE; // Stores index data, but it can be null if this buffer is a vertex buffer
        bool is_index = false; // Allows us to know which type of buffer this is without needing to check the handles
    };

    struct Pipeline
    {
        bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;
        uint64_t state = 0; // Stored as raw bgfx bitmask
    };

    static std::vector<Buffer>   g_buffers;
    static std::vector<Pipeline> g_pipelines;
    static std::vector<bgfx::UniformHandle> g_uniforms;

    static uint16_t g_width = 0;
    static uint16_t g_height = 0;

    bool init(void* display_type, void* window_handle, uint16_t width, uint16_t height)
    {
        g_width = width;
        g_height = height;

        bgfx::Init init{};

        // TODO: Replace this
        init.type = bgfx::RendererType::Count; // Auto-selects the renderer
        
        // Pass the handles directly into the init struct
        // So, ndt is the native display type & nwh is the native window handle
        init.platformData.ndt = display_type;
        init.platformData.nwh = window_handle;
        
        init.resolution.width  = width;
        init.resolution.height = height;
        init.resolution.reset  = BGFX_RESET_VSYNC;

        if (!bgfx::init(init)) {
            std::println("bgfx failed to init!");
            return false;
        }

        bgfx::setDebug(BGFX_DEBUG_TEXT);
        bgfx::setViewClear(0,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0x303030ff, 1.0f, 0);
            
        return true;
    }

    void shutdown()
    {
        for (auto& b : g_buffers)
        {
            // Destroy the buffers
            if (bgfx::isValid(b.vbh)) bgfx::destroy(b.vbh);
            if (bgfx::isValid(b.ibh)) bgfx::destroy(b.ibh);
        }

        for (auto& p : g_pipelines)
        {
            // Destroy the pipeline
            if (bgfx::isValid(p.program)) bgfx::destroy(p.program);
        }

        for (auto& u : g_uniforms)
        {
            // Destroy the uniforms
            if (bgfx::isValid(u)) bgfx::destroy(u);
        }

        // Clear everything so we don't have dangling handles
        g_buffers.clear();
        g_pipelines.clear();
        g_uniforms.clear();

        // Have bgfx destroy the context and everything it holds internally
        bgfx::shutdown();
    }

    void resize(uint16_t width, uint16_t height)
    {
        if(width == 0 || height == 0)
            return; // Minimized window safety

        if(width == g_width && height == g_height)
            return; // No need to resize
        
        g_width = width;
        g_height = height;

        bgfx::reset(width, height, BGFX_RESET_VSYNC);
    }

    uint64_t map_state(PipelineState state)
    {
        uint64_t bgfx_state = BGFX_STATE_NONE;
        // Cast to uint64_t so the 'if' can treat it as a boolean check
        // Otherwise, the bitwise check would fail since PipelineState is an enum class and doesn't implicitly convert to uint64_t
        if (static_cast<uint64_t>(state & PipelineState::WriteRGB)) bgfx_state |= BGFX_STATE_WRITE_RGB;
        if (static_cast<uint64_t>(state & PipelineState::WriteAlpha)) bgfx_state |= BGFX_STATE_WRITE_A;
        if (static_cast<uint64_t>(state & PipelineState::MSAA)) bgfx_state |= BGFX_STATE_MSAA;
        if (static_cast<uint64_t>(state & PipelineState::PrimitiveTriStrip)) bgfx_state |= BGFX_STATE_PT_TRISTRIP;
        return bgfx_state;
    }

    ShaderHandle create_shader(const void* data, uint32_t size)
    {
        // Check the input data before trying to create the shader
        // If it's invalid, print an error & return an invalid handle except for crashing
        if(!data || size == 0)
        {
            std::println("Error: Invalid shader data or size");
            return InvalidShader;
        }

        const bgfx::Memory* mem = bgfx::copy(data, size);
        return bgfx::createShader(mem).idx;
    }

    PipelineHandle create_pipeline(const PipelineDesc& desc)
    {
        // Check the input data before trying to create the pipeline
        // If it's invalid, print an error & return an invalid handle except for crashing
        if(!desc.vs || !desc.fs)
        {
            std::println("Error: Invalid shader handles in PipelineDesc");
            return InvalidPipeline;
        }


        bgfx::ShaderHandle vs{ desc.vs };
        bgfx::ShaderHandle fs{ desc.fs };

        bgfx::ProgramHandle prog = bgfx::createProgram(vs, fs, true);

        g_pipelines.push_back({ prog, map_state(desc.state) });
        return (PipelineHandle)(g_pipelines.size() - 1);
    }

    BufferHandle create_vertex_buffer(const void* data, uint32_t size)
    {
        // Check the input data before trying to create the buffer
        // If it's invalid, print an error & return an invalid handle except for crashing
        if(!data || size == 0)
        {
            std::println("Error: Invalid vertex buffer data or size");
            return InvalidBuffer;
        }

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
            bgfx::copy(data, size),
            layout
        );

        g_buffers.push_back({ vbh, BGFX_INVALID_HANDLE, false });
        return (BufferHandle)(g_buffers.size() - 1);
    }

    BufferHandle create_index_buffer(const void* data, uint32_t size)
    {
        // Check the input data before trying to create the buffer
        // If it's invalid, print an error & return an invalid handle except for crashing
        if(!data || size == 0)
        {
            std::println("Error: Invalid index buffer data or size");
            return InvalidBuffer;
        }

        bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
            bgfx::copy(data, size)
        );

        g_buffers.push_back({ BGFX_INVALID_HANDLE, ibh, true });
        return (BufferHandle)(g_buffers.size() - 1);
    }

    static bgfx::UniformType::Enum map_uniform_type(UniformType type) {
        switch (type) {
            case UniformType::Sampler: return bgfx::UniformType::Sampler;
            case UniformType::Vec4:    return bgfx::UniformType::Vec4;
            case UniformType::Mat3:    return bgfx::UniformType::Mat3;
            case UniformType::Mat4:    return bgfx::UniformType::Mat4;
        }
        return bgfx::UniformType::Count;
    }

    UniformHandle create_uniform(const char* name, UniformType type, uint16_t num) {
        bgfx::UniformHandle handle = bgfx::createUniform(name, map_uniform_type(type), num);
        g_uniforms.push_back(handle);
        return static_cast<UniformHandle>(g_uniforms.size() - 1);
    }

    void set_uniform(UniformHandle handle, const void* value, uint16_t num) {
        // Check for null/invalid handles
        if (handle == InvalidUniform) return; 

        // Check for out of bound handles
        if (handle >= g_uniforms.size()) {
            std::println("Error: Uniform handle out of bounds!");
            return;
        }

        bgfx::setUniform(g_uniforms[handle], value, num);
    }

    void destroy_uniform(UniformHandle handle) {
        if (handle < g_uniforms.size() && bgfx::isValid(g_uniforms[handle])) {
            bgfx::destroy(g_uniforms[handle]);
            // We don't remove from vector to keep indices stable, 
            // just invalidate the handle
            g_uniforms[handle] = BGFX_INVALID_HANDLE;
        }
    }

    void identity_matrix(float* _mtx)
    {
        bx::mtxIdentity(_mtx);
    }

    void submit(const RenderPacket& p, ViewID view)
    {
        // Check for null/invalid handles
        if (p.pipeline == InvalidPipeline || p.vertex_buffer == InvalidBuffer) {
            std::println("Error: Attempted to submit RenderPacket with unitialized handles.");
            return;
        }

        // Check for out of bounds handles
        if (p.pipeline >= g_pipelines.size() || p.vertex_buffer >= g_buffers.size()) {
            std::println("Error: Handle out of bounds! The resource may have been destroyed already or it was never created. Pipeline Handle: {}, Vertex Buffer Handle: {}", p.pipeline, p.vertex_buffer);
            return;
        }

        Pipeline& pipeline = g_pipelines[p.pipeline];
        Buffer& vb = g_buffers[p.vertex_buffer];

        bgfx::setTransform(p.model);
        bgfx::setVertexBuffer(0, vb.vbh);

        if (p.index_buffer != InvalidBuffer)
        {
            if (p.index_buffer >= g_buffers.size()) {
                std::println("Error: Invalid index buffer handle in RenderPacket");
                return;
            }
            Buffer& ib = g_buffers[p.index_buffer];
            if (ib.is_index)
                bgfx::setIndexBuffer(ib.ibh);
        }

        if (p.uniform_handle != InvalidUniform && p.uniform_handle < g_uniforms.size())
        {
            bgfx::setUniform(g_uniforms[p.uniform_handle], p.uniform_data, 1);
        }

        bgfx::setState(pipeline.state);
        bgfx::submit(view, pipeline.program);
    }

    void begin_frame()
    {
        bgfx::setViewRect(0, 0, 0, g_width, g_height);

        float view[16];
        bx::mtxIdentity(view);

        float proj[16];
        bx::mtxOrtho(
            proj,
            -1.0f, 1.0f,
            -1.0f, 1.0f,
            0.0f, 100.0f,
            0.0f,
            false
        );

        bgfx::setViewTransform(0, view, proj);
        bgfx::touch(0);
    }

    void end_frame()
    {
        bgfx::frame();
    }
}