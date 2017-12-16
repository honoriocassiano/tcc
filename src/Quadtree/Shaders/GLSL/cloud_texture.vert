void main(void)
{
  gl_TexCoord[0].st = 0.5 + 0.5 * gl_Vertex.xy;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
