uniform float TerrainHigh;
uniform float TerrainLow;
uniform sampler3D Noise;
uniform sampler2D AltGrad;

uniform vec3 GrassColor;
uniform vec3 RockColor;

varying vec3 MCposition;
varying vec3 LCposition[8];

varying float Gradient; 
varying vec3  MCnormal;
varying vec2  TexCoord;

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

vec3 terrain()
{
    float height    = (MCposition.z - TerrainLow)/(TerrainHigh - TerrainLow);
    vec2  coord     = vec2(Gradient, height);
    vec4  altGradv  = texture2D(AltGrad, coord);
    float altGrad   = altGradv.x;

    vec4  noisevec  = texture3D(Noise, MCposition*0.1);
    float intensity = (noisevec[0] + noisevec[1] + 
                       noisevec[2] + noisevec[3] + 0.03125);
    vec3  color     = mix(GrassColor, RockColor, altGrad) * intensity;
    return color;
}

void main(void)
{
    //vec3 color = brickPattern();
    vec3 color   = terrain();

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
    color *= clamp(LightIntensity, 0.05, 1.0);
    gl_FragColor = vec4(color, 1.0);
}
