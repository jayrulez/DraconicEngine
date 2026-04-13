$input a_position, a_color0
$output v_color0

#include <bgfx_shader.sh>

// Note: For bgfx, uniforms are always vec4 or mat4.
uniform vec4 u_offset; 

void main()
{
    // Add the offset to the local position before transforming
    vec3 pos = a_position + u_offset.xyz;
    gl_Position = mul(u_modelViewProj, vec4(pos, 1.0));
    v_color0 = a_color0;
}