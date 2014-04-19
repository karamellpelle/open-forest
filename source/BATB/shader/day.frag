//
// Fragment shader for noise texturing
//

uniform float TerrainHigh;
uniform float TerrainLow;
uniform sampler3D Noise;
uniform sampler2D AltGrad;
//uniform sampler2D Trees;

uniform vec3 GrassColor;
//uniform vec3 DirtColor;
uniform vec3 RockColor;

varying float Gradient; 
varying vec3  MCposition;
varying vec3  MCnormal;
varying vec2  TexCoord;


vec3 terrain()
{
    float height    = (MCposition.z - TerrainLow)/(TerrainHigh - TerrainLow);
    //float Grad      = max(dot(normalize(vec3(0.0, 0.0, 100.0)), MCnormal), 0.0);
    vec2  coord     = vec2(Gradient, height);
    //vec2  coord     = vec2(Grad, height);
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
    vec3 color   = terrain();
    gl_FragColor = vec4(color, 1.0);
}
