void main(void)
{
  gl_TexCoord[0].stp = gl_MultiTexCoord0.stp;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
