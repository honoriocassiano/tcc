#pragma include perlin.frag

uniform int   octaves;
uniform float cloudCover;
uniform float cloudSharpness;
uniform float freq0;

#define PI 3.14159265

void main(void)
{
  // Calculate cartesian coordinates (direction, not position)
  float theta = gl_TexCoord[0].s * PI * 2.0;
  float phi   = gl_TexCoord[0].t * PI;
  vec3  dir0  = vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
  vec3  dir1  = vec3( dir0.x, dir0.z, -dir0.y);
  vec3  dir2  = vec3(-dir0.x, dir0.y,  dir0.z);
  
  // Make some noise (three channels; produces three different cloud textures)
  vec3  nSum    = vec3(0.0, 0.0, 0.0);
  vec3  freq    = vec3(freq0, freq0, freq0);
  vec3  lac     = vec3(2.1, 2.2, 2.3);
  float weight  = 1.0;
  float sumMax  = 0.0;
  for (int i = 0; i < octaves; i++)
  {
    // Add noise value
    nSum.x  += weight * perlin(freq.x * dir0);
    nSum.y  += weight * perlin(freq.y * dir1);
    nSum.z  += weight * perlin(freq.z * dir2);
    
    // The sum of all weights is used to normalize the results
    sumMax  += weight;
    
    // Update variables
    weight  *= 0.5;
    freq    *= lac;
  }
  nSum /= sumMax;           // [-1,1]
  nSum = 0.5 + 0.5 * nSum;  // [ 0,1]
  
  // http://freespace.virgin.net/hugo.elias/models/m_clouds.htm
  vec3 c   = max(nSum - cloudCover, 0.0);
  vec3 val = 1.0 - pow(vec3(cloudSharpness), c);
  
  //gl_FragColor = vec4(1.0, 1.0, 1.0, val);
  gl_FragColor = vec4(val, 1.0);
}
