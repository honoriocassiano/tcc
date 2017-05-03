varying vec3  normal;
varying float texCoord;

void main(void)
{
  texCoord = gl_MultiTexCoord0.t;
  
  // Normal rotated 90 degrees about the x axis (the way the disk is rendered)
  normal = normalize(vec3(gl_Vertex.x, -gl_Vertex.z, gl_Vertex.y));
  
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
