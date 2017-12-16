uniform vec3  lightDir;
uniform float planetRadius;
uniform float atmosphereRadius;
uniform vec3  camPos;
uniform float camHeight;
varying float alpha;

// www.cs.umbc.edu/~alark1/cmsc435/lectures/RayCasting4.pdf
float getFirstRaySphereIntersection(vec3 rayOrigin, vec3 rayDir, float radius)
{
  float b = 2.0 * dot(rayOrigin, rayDir);
  float c = dot(rayOrigin, rayOrigin) - radius * radius;
  float discriminant = sqrt(max(0.0, b * b - 4.0 * c));
  float t0 = -0.5 * b - 0.5 * discriminant;
  //float t1 = -0.5 * b + 0.5 * discriminant;
  return t0;
}

void main(void)
{
  // Ray end is vertex position
  vec3 rayEnd = gl_Vertex.xyz * atmosphereRadius;
  
  // Ray direction from camera position to ray end
  vec3 rayDir = normalize(rayEnd - camPos);
  
  // Ray start is camera position if inside atmosphere, else first intersection
  vec3 rayStart = camPos;
  if (camHeight > atmosphereRadius)
    rayStart += rayDir * getFirstRaySphereIntersection(camPos, rayDir, atmosphereRadius);
  
  // Distance within atmosphere
  float rayLength = distance(rayEnd, rayStart);
  float atmosphereHeight = atmosphereRadius - planetRadius;
  alpha = rayLength / atmosphereRadius;
  //alpha *= alpha;
  
  #define STEPS 5
  alpha = 0.0;
  float stepSize = rayLength / float(STEPS);
  for (int i = 0; i < STEPS; i++)
  {
    vec3 rayStepPos = rayStart + float(i) * stepSize * rayDir;
    float rayStepPosHeight = length(rayStepPos);
    vec3 rayStepDir = rayStepPos / rayStepPosHeight;
    float density = 1.0 - (rayStepPosHeight - planetRadius) / atmosphereHeight;
    density -= 0.2;
    float light = max(0.0, 0.2 + dot(rayStepDir, lightDir));
    alpha += light * density;
  }
  alpha /= float(STEPS);
  
  // Enhance the alpha value
  alpha *= 1.6;
  
  gl_Position = gl_ModelViewProjectionMatrix * vec4(rayEnd, 1.0);
}
