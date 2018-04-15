uniform float Scale;
//uniform mat4 WCtoLC[8];
//uniform int numEnabledLights;
uniform float horizontal[8];
uniform float vertical[8];
uniform vec3 position[8];

varying vec3 MCposition;
varying vec3 LCposition[8];

varying float Gradient;
//varying vec3  MCposition;
varying vec3  MCnormal;
varying vec2  TexCoord;

vec3 lightPosition(int i)
{
    float sa = sin(vertical[i]);
    float ca = cos(vertical[i]);
    float sb = sin(horizontal[i]);
    float cb = cos(horizontal[i]);
    float x = position[i][0];
    float y = position[i][1];
    float z = position[i][2];

    mat4 rotx = mat4(
     	     1,   0,   0, 0,
     	     0,  ca,  sa, 0,
    	     0, -sa,  ca, 0,
    	     0,   0,   0, 1
    );
    mat4 rotz = mat4(
     	    cb,  sb,   0, 0,
     	   -sb,  cb,   0, 0,
    	     0,   0,   1, 0,
    	     0,   0,   0, 1
    );
    mat4 translate = mat4(
     	     1,   0,   0, 0,
     	     0,   1,   0, 0,
    	     0,   0,   1, 0,
    	     x,   y,   z, 1
    );
    
    vec4 v4 = rotx * rotz * translate * gl_Vertex;
    vec3 v3 = v4.xyz;
    return v3;
}

void main(void)
{
    // directional light in eye coordinates
    vec4 upVec4     = vec4(0.0, 0.0, 1000.0, 0.0);
    vec3 upVec3     = vec3(gl_ModelViewMatrix * upVec4);
    MCposition      = vec3(gl_Vertex) * Scale;
    MCnormal        = normalize(vec3(gl_NormalMatrix * gl_Normal));
    Gradient        = max(dot(normalize(upVec3), MCnormal), 0.0);
    TexCoord        = gl_MultiTexCoord0.st;

    LCposition[0] = vec3(0.0, 0.0, 0.0);
    LCposition[1] = vec3(0.0, 0.0, 0.0);
    LCposition[2] = vec3(0.0, 0.0, 0.0);
    LCposition[3] = vec3(0.0, 0.0, 0.0);
    LCposition[4] = vec3(0.0, 0.0, 0.0);
    LCposition[5] = vec3(0.0, 0.0, 0.0);
    LCposition[6] = vec3(0.0, 0.0, 0.0);
    LCposition[7] = vec3(0.0, 0.0, 0.0);

    LCposition[0] = lightPosition(0);
    LCposition[1] = lightPosition(1);
    LCposition[2] = lightPosition(2);
#if 0
    LCposition[3] = lightPosition(3);
    LCposition[4] = lightPosition(4);
    LCposition[5] = lightPosition(5);
    LCposition[6] = lightPosition(6);
    LCposition[7] = lightPosition(7);
#endif

    //MCposition  = gl_Vertex.xyz;
    gl_Position = ftransform();
}
