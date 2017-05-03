#pragma include perlin.frag

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
