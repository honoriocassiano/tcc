uniform sampler1D ringTex;
uniform vec3      lightDir;
varying vec3      normal;
varying float     texCoord;

void main(void)
{
  float light = dot(normal, lightDir);
  light = clamp((light + 0.8) * 4.0, 0.0, 1.0);
  vec4  color = texture1D(ringTex, texCoord);
  gl_FragColor = vec4(light * color.rgb, color.a);
}
