uniform sampler2D starTex;

void main(void)
{
  float alpha = texture2D(starTex, gl_TexCoord[0].st).r;
  alpha += smoothstep(0.7, 0.8, alpha);
  gl_FragColor = vec4(1.0, 1.0, 0.8 + 0.2 * alpha, alpha);
}
