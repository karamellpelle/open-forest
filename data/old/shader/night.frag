uniform vec3 BrickColor, MortarColor;
uniform vec2 BrickSize;
uniform vec2 BrickPct;
uniform int numEnabledLights;

varying vec3 MCposition;
varying vec3 LCposition;
varying vec3 tnorm;

float lighting(vec3 LCposition)
{
    vec3 lightPos = vec3(0.0, 0.0, 0.0);
    vec3 lightDir = vec3(0.0, 0.0, -1.0);
    vec3 lightVec = normalize(LCposition);

    // spotlight cone
    float LightIntensity = (-lightVec.z - 0.9)*3.0;
    // distance attenuation
    LightIntensity *= max(LCposition.z*0.1 + 1.0, 0.0);
    return LightIntensity;
}

float lighting2(vec3 LCposition)
{
    vec3 lightPos = vec3(0.0, 0.0, 0.0);
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
    
    float ambientLight = 0.05;
    if (LightIntensity < ambientLight)
		LightIntensity = ambientLight;

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
    color  = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);

#if 0
    float LightIntensity = lighting(LCposition);
#else
    float LightIntensity = lighting2(LCposition);
#endif

    color *= LightIntensity;
    gl_FragColor = vec4(color, 1.0);
}
