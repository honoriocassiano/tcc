uniform sampler2D starTex;

void main(void)
{
  float alpha = texture2D(starTex, gl_TexCoord[0].st).r;
  gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
}
