uniform vec3 BrickColor, MortarColor;
uniform vec2 BrickSize;
uniform vec2 BrickPct;
uniform int numEnabledLights;

varying vec3 MCposition;
varying vec3 LCposition[8];

float lighting(vec3 LCposition)
{
    vec3 lightDir = normalize(vec3(0.0, 0.0, -1.0));
    vec3 lightVec = normalize(LCposition);

    float LightIntensity = smoothstep(0.95, 0.98, dot(lightVec, lightDir));
    // distance attenuation
    float distance = -LCposition.z;
    float constantAttenuation = 0.2;
    float linearAttenuation = 0.2;
    float quadraticAttenuation = 0.2;
    float attenuation = 1.0 / (constantAttenuation + 
                               linearAttenuation * distance + 
                               quadraticAttenuation * distance * distance);
    LightIntensity *= attenuation;
    
    return LightIntensity;
}

void main(void)
{
    vec3  color;
    vec2  position, useBrick;

    // brick pattern    
    position = MCposition.xy / BrickSize;

    if (fract(position.y * 0.5) > 0.5)
        position.x += 0.5;

    position = fract(position);
    useBrick = step(position, BrickPct);
    color = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);

	// ligting
    float LightIntensity = 0.0;
    for (int i = 0; i < 2; i++) {
		LightIntensity += lighting(LCposition[i]);
	}
	color *= clamp(LightIntensity, 0.05, 1.0);
    gl_FragColor = vec4(color, 1.0);
}
