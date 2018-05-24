#version 330

uniform mat4 ortho;
uniform sampler2D tex;

layout (location = 0 ) in vec2 xy;
layout (location = 1 ) in vec2 uv;
layout (location = 2 ) in vec4 color;

out VSH_OUT
{
    vec2 uv;
    vec4 color;

} vsh_out;

void main()
{
    vsh_out.uv = uv;
    vsh_out.color = color;

    gl_Position = ortho * vec4( xy, 0, 1 );
}

