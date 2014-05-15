uniform sampler2D Trees;

varying vec2  TexCoord;

vec4 drawTree(void)
{
    return texture2D(Trees, TexCoord);
}

void main(void)
{
    gl_FragColor = drawTree();
}
