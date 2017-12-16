uniform vec3  lightDir;
uniform float cloudLayerRadius;
varying float light;

void main(void)
{
  // Set texture coordinates
  gl_TexCoord[0].st = gl_MultiTexCoord0.st;
  
  // The normal is simply xyz of the untransformed vertex position
  vec3 normal = gl_Vertex.xyz;
  
  // Calculate incoming light
  light = max(0.0, dot(lightDir, normal) + 0.4);
  
  // Scale by radius to get the true vertex position
  vec3 vert = normal * cloudLayerRadius;
  
  gl_Position = gl_ModelViewProjectionMatrix * vec4(vert, 1.0);
}
