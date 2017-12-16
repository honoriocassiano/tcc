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


#define OCTAVES 8

void main(void)
{
  vec3 spherePos = normalize(gl_TexCoord[0].stp);
  
  // Make some noise
  float sum     = 0.0;
  float freq    = 4.0;
  float weight  = 1.0;
  for (int i = 0; i < OCTAVES; i++)
  {
    sum     += weight * perlin(freq * spherePos);
    freq    *= 2.0;
    weight  *= 0.5;
  }
  sum /= float(OCTAVES);
  float val = 0.5 + 0.5 * sum;
  val *= 1.8;
  val = pow(val, 12.0);
  //val = max(val, 0.4 * exp(2.0 * val) - 1.0);
  //val = max(val * 1.5 - 0.2, 0.0);
  //val = val * val;
  vec3 color0 = vec3(val * val * val, val * val, val) * 0.50;
  gl_FragColor = vec4(color0, 1.0);
}
