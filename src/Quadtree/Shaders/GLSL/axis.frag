varying vec3 normal;

void main(void)
{
  float light = max(0.0, normal.z);
  gl_FragColor = vec4(light, light, light, 1.0);
}
