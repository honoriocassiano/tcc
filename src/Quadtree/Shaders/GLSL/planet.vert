varying vec3  planetNormal;
varying vec2  texCoord;
varying float dist;

void main(void)
{
  planetNormal = normalize(gl_Vertex.xyz);
  texCoord = gl_MultiTexCoord0.st;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  dist = gl_Position.z;
}
