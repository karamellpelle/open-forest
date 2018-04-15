uniform mat4 WCtoLC[8];
uniform int numEnabledLights;

varying vec3 MCposition;
varying vec3 LCposition[8];

void main(void)
{
	LCposition[0] = vec3(0.0, 0.0, 0.0);
	LCposition[1] = vec3(0.0, 0.0, 0.0);
	LCposition[2] = vec3(0.0, 0.0, 0.0);
	LCposition[3] = vec3(0.0, 0.0, 0.0);
	LCposition[4] = vec3(0.0, 0.0, 0.0);
	LCposition[5] = vec3(0.0, 0.0, 0.0);
	LCposition[6] = vec3(0.0, 0.0, 0.0);
	LCposition[7] = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < 2; i++)
	{
		vec4 v4 = WCtoLC[i] * gl_Vertex;
		vec3 v3 = v4.xyz;
		if (i == 0)
			LCposition[0] = v3;
		else if (i == 1)
			LCposition[1] = v3;
	}
    MCposition  = gl_Vertex.xyz;
    gl_Position = ftransform();
}
