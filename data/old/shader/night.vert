uniform mat4 WCtoLC;
uniform mat4 WCtoLCit;
uniform int numEnabledLights;

varying vec3 MCposition;
varying vec3 LCposition;
varying vec3 tnorm;

void main(void)
{
#if 0
    LCposition  = vec3(gl_ModelViewMatrix * gl_Vertex);
    tnorm       = normalize(gl_NormalMatrix * gl_Normal);
    MCposition  = gl_Vertex.xyz;
    gl_Position = ftransform();
#else
    LCposition  = vec3(WCtoLC * gl_Vertex);
    tnorm       = normalize(gl_NormalMatrix * gl_Normal);
    //tnorm       = normalize((WCtoLCit * vec4(gl_Normal, 0.0)).xyz);
    MCposition  = gl_Vertex.xyz;
    gl_Position = ftransform();
#endif
}
