#version 330

uniform sampler2D tex;

in VSH_OUT
{
    vec2 uv;
    vec4 color;

} fsh_in;

out vec4 color;

void main()
{
    color = fsh_in.color * texture( tex, fsh_in.uv );
}

