uniform float Scale;

varying float LightIntensity;
varying vec3  MCposition;
varying vec3  MCnormal;
varying vec2 TexCoord;

void main(void)
{
    // directional light in eye coordinates
    vec4 upVector   = vec4(-100.0, -100.0, 1000.0, 0.0);
    vec3 ECLightPos = vec3(gl_ModelViewMatrix * upVector);
    MCposition      = vec3(gl_Vertex) * Scale;
    MCnormal        = normalize(vec3(gl_NormalMatrix * gl_Normal));
    LightIntensity  = max(dot(normalize(ECLightPos), MCnormal), 0.0);
    TexCoord        = gl_MultiTexCoord0.st;
    gl_Position     = ftransform();
}
