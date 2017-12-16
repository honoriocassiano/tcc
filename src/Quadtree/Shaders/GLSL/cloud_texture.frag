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


//#pragma include perlin.frag

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
