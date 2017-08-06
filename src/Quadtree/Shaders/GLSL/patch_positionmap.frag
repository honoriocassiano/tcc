uniform sampler2D permTex;

#define TEXEL_FULL 0.003906250
#define TEXEL_HALF 0.001953125
#define FADE(t) ((t) * (t) * (t) * ((t) * ((t) * 6.0 - 15.0) + 10.0))

float perlin(vec3 p)
{
  vec3 p0i = TEXEL_FULL * floor(p) + TEXEL_HALF;
  vec3 p1i = p0i + TEXEL_FULL;
  vec3 p0f = fract(p);
  vec3 p1f = p0f - 1.0;
  
  vec3 u = FADE(p0f);
  
  // Could using a single RGBA texture for the four permutation table lookups increase performance?
  float perm00 = texture2D(permTex, vec2(p0i.y, p0i.z)).a;
  float perm10 = texture2D(permTex, vec2(p1i.y, p0i.z)).a;
  float perm01 = texture2D(permTex, vec2(p0i.y, p1i.z)).a;
  float perm11 = texture2D(permTex, vec2(p1i.y, p1i.z)).a;
  
  float a = dot(texture2D(permTex, vec2(p0i.x, perm00)).rgb * 4.0 - 1.0, vec3(p0f.x, p0f.y, p0f.z));
  float b = dot(texture2D(permTex, vec2(p1i.x, perm00)).rgb * 4.0 - 1.0, vec3(p1f.x, p0f.y, p0f.z));
  float c = dot(texture2D(permTex, vec2(p0i.x, perm10)).rgb * 4.0 - 1.0, vec3(p0f.x, p1f.y, p0f.z));
  float d = dot(texture2D(permTex, vec2(p1i.x, perm10)).rgb * 4.0 - 1.0, vec3(p1f.x, p1f.y, p0f.z));
  float e = dot(texture2D(permTex, vec2(p0i.x, perm01)).rgb * 4.0 - 1.0, vec3(p0f.x, p0f.y, p1f.z));
  float f = dot(texture2D(permTex, vec2(p1i.x, perm01)).rgb * 4.0 - 1.0, vec3(p1f.x, p0f.y, p1f.z));
  float g = dot(texture2D(permTex, vec2(p0i.x, perm11)).rgb * 4.0 - 1.0, vec3(p0f.x, p1f.y, p1f.z));
  float h = dot(texture2D(permTex, vec2(p1i.x, perm11)).rgb * 4.0 - 1.0, vec3(p1f.x, p1f.y, p1f.z));
  
  vec4 lerpZ = mix(vec4(a, c, b, d), vec4(e, g, f, h), u.z);
  vec2 lerpY = mix(lerpZ.xz, lerpZ.yw, u.y);
  return mix(lerpY.x, lerpY.y, u.x);
}


// #pragma include perlin.frag

uniform float heightScale;
uniform int   octaves;
uniform float gain;
uniform float lacunarity;
uniform float offset;
uniform float h;
varying vec3  cubePosition;

// http://mathproofs.blogspot.com/2005/07/mapping-cube-to-sphere.html
vec3 cubeToSphere(vec3 pt)
{
  vec3 ptSq = pt * pt;
  return pt.xyz * sqrt(max(1.0 - (ptSq.yzx + ptSq.zxy) * 0.5 + ptSq.yzx * ptSq.zxy * 0.3333333, 0.0));
}

float ridgedMultifractal(vec3 dir)
{
  float frequency = lacunarity, signal, weight;
  
  // Get the base signal (absolute value to create the ridges; square for sharper ridges)
  signal = offset - abs(perlin(dir));
  signal *= signal;
  float result = signal;
  
  float exponentArraySum = 1.0;
  for (int i = 1; i < octaves; i++)
  {
    // This could be precalculated
    float exponentValue = pow(frequency, -h);
    exponentArraySum += exponentValue;
    frequency *= lacunarity;
    
    dir *= lacunarity;
    weight = clamp(signal * gain, 0.0, 1.0);
    
    // Get the next "octave" (only true octave if lacunarity = 2.0, right?)
    signal = offset - abs(perlin(dir));
    signal *= signal;
    signal *= weight;
    
    result += signal * exponentValue;
  }
  
  // Scale result to [0,1] (not true when offset != 1.0)
  result /= exponentArraySum;
  
  return result;
}

void main(void)
{
  // Transform from cube [-1,1] to unit sphere
  vec3 spherePosition = cubeToSphere(cubePosition);
  
  // Calculate height value
  float rmf0 = ridgedMultifractal(spherePosition);
  
  // Scale and bias to match CPU implementation [-1,1]
  float rmf1 = rmf0 * 2.0 - 1.0;
  float height = 1.0 + rmf1 * heightScale;
  //height *= radius;
  
  // RGBA with surface position (RGB) and RMF value (A)
  gl_FragColor = vec4(spherePosition * height, rmf0);
}
