uniform float elapsed;
uniform vec3 cameraPosition;

void main(void)
{
  // Vertex position on the cube [-1,1] is passed as texture coordinates
//  cubePosition = gl_MultiTexCoord0.xyz;
  
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
