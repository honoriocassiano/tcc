uniform vec3      lightDir;
uniform sampler2D colormap;
uniform sampler2D heightmap;
uniform sampler2D normalmap;
uniform vec3      atmosphereColor;
varying vec3      planetNormal;
varying vec2      texCoord;
varying float     dist;

#define FOG_EXP 4.0
#define FOG_MAX 0.25
//#define AMBIENT 0.2

void main(void)
{
  // Get terrain properties
  float terrainHeight = texture2D(heightmap, texCoord).r;
  float terrainSlope = 0.0;
  vec3 normal = texture2D(normalmap, texCoord).rgb * 2.0 - 1.0;
  
  // Calculate incoming light
  float light = max(0.0, dot(lightDir, normal)); // * (1.0 - AMBIENT) + AMBIENT;
  
  // Sphere self-shadowing (light shouldn't appear to pass through the planet)
  float planetLight = max(0.0, dot(lightDir, planetNormal));
  
  // Color from colormap
  vec3 color = texture2D(colormap, vec2(terrainHeight, terrainSlope)).rgb;
  
  // Fog value
  float fogVal = max(0.0, (1.0 - exp(-FOG_EXP * dist)) * FOG_MAX);
  
  // Mixed color (color + fog)
  vec3 colorFog = mix(color, atmosphereColor, fogVal);
  
  // Set final color
  gl_FragColor = vec4(planetLight * colorFog, 1.0);
  gl_FragColor = vec4(light * colorFog, 1.0);
}
