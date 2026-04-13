$input v_color0 
#include <bgfx_shader.sh> 

uniform vec4 u_tint;

void main() { 
    // Multiply the vertex color by our uniform tint
    gl_FragColor = v_color0 * u_tint; 
}