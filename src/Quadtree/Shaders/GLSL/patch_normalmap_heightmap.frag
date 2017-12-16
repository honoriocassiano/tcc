uniform sampler2D positionmap;
uniform float texelSize;

void main(void)
{
  // Sample positions
  vec3 posX0Y0 = texture2D(positionmap, gl_TexCoord[0].st).rgb;
  vec3 posXMY0 = texture2D(positionmap, gl_TexCoord[0].st - vec2(texelSize, 0.0)).rgb;
  vec3 posXPY0 = texture2D(positionmap, gl_TexCoord[0].st + vec2(texelSize, 0.0)).rgb;
  vec3 posX0YM = texture2D(positionmap, gl_TexCoord[0].st - vec2(0.0, texelSize)).rgb;
  vec3 posX0YP = texture2D(positionmap, gl_TexCoord[0].st + vec2(0.0, texelSize)).rgb;
  
  // Edges connecting the samples
  vec3 edgeXM = posXMY0 - posX0Y0;
  vec3 edgeXP = posXPY0 - posX0Y0;
  vec3 edgeYM = posX0YM - posX0Y0;
  vec3 edgeYP = posX0YP - posX0Y0;
  
  // Using only one of these normals is faster but not as accurate
  vec3 normalM = cross(edgeXM, edgeYM);
  vec3 normalP = cross(edgeXP, edgeYP);
  
  // Normalize the sum of both normals (averaging happens automatically)
  vec3 normal = normalize(normalM + normalP);
  gl_FragData[0] = 0.5 + 0.5 * vec4(normal, 1.0);
  gl_FragData[1] = vec4(texture2D(positionmap, gl_TexCoord[0].st).a);
}
