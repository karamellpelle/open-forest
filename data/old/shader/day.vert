uniform float Scale;
//uniform int drawing;

varying float Gradient;
varying vec3  MCposition;
varying vec3  MCnormal;
varying vec2  TexCoord;

void main(void)
{
    // directional light in eye coordinates
    vec4 upVec4     = vec4(0.0, 0.0, 1000.0, 0.0);
    vec3 upVec3     = vec3(gl_ModelViewMatrix * upVec4);
    MCposition      = vec3(gl_Vertex) * Scale;
    MCnormal        = normalize(vec3(gl_NormalMatrix * gl_Normal));
    Gradient        = max(dot(normalize(upVec3), MCnormal), 0.0);
    TexCoord        = gl_MultiTexCoord0.st;
    gl_Position     = ftransform();
}
