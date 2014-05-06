uniform sampler2D Trees;

varying vec3 LCposition[8];
varying vec2 TexCoord;


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

vec4 drawTree(void)
{
    return texture2D(Trees, TexCoord);
}

void main(void)
{
    vec4 color4 = drawTree();
    vec3 color3 = vec3(color4);

    // lighting
    float LightIntensity = 0.0;

    LightIntensity += lighting(LCposition[0]);
    LightIntensity += lighting(LCposition[1]);
    LightIntensity += lighting(LCposition[2]);
#if 0
    LightIntensity += lighting(LCposition[3]);
    LightIntensity += lighting(LCposition[4]);
    LightIntensity += lighting(LCposition[5]);
    LightIntensity += lighting(LCposition[6]);
    LightIntensity += lighting(LCposition[7]);
#endif
    color3 *= clamp(LightIntensity, 0.05, 1.0);
    gl_FragColor = vec4(color3, color4.w);
}
