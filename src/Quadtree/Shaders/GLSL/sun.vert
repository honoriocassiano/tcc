void main(void)
{
  gl_TexCoord[0].st = gl_MultiTexCoord0.st;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
