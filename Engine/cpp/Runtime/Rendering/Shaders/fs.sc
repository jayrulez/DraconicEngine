$input v_normal, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0);

uniform vec4 u_tint;

void main()
{
    vec3 lightDir = normalize(vec3(0.4, 1.0, 0.2));

    float NdotL = max(dot(normalize(v_normal), lightDir), 0.2);

    vec4 texColor = texture2D(s_texColor, v_texcoord0);

    gl_FragColor = texColor * u_tint * NdotL;
}