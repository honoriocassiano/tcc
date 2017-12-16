uniform sampler2D cloudTex;
uniform vec3      cloudTexWeights;
varying float     light;

void main(void)
{
  // Get the three cloud textures samples (one per texture channel)
  vec3 texSample = texture2D(cloudTex, gl_TexCoord[0].st).rgb;
  
  // Sum the weighted textures samples
  float alpha = dot(cloudTexWeights, texSample);
  
  // Enhance the alpha value
  alpha = smoothstep(0.3, 0.7, alpha);
  
  // Use light and alpha to set the final color
  gl_FragColor = vec4(light, light, light, alpha);
}
