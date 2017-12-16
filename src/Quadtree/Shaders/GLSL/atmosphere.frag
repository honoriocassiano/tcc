uniform vec3  atmosphereColor;
varying float alpha;

void main(void)
{
  gl_FragColor = vec4(atmosphereColor, alpha);
}
