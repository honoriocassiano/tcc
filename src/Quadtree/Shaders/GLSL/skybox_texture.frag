#pragma include perlin.frag

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
