#version 450 core

layout (location = 0) in vec3               vertexPos_VS;	  	// Position in attribute location 0
layout (location = 1) in vec2               texCoord_VS;
layout (location = 2) in vec3               vertexNormal_VS;

layout (location = 3) uniform mat4          modelMatrix;
layout (location = 4) uniform mat4          viewMatrix;
layout (location = 5) uniform mat4          projMatrix;
layout (location = 6) uniform vec3          lightPos_VS;
layout (location = 7) uniform vec3          color_VS;
layout (location = 8) uniform vec3          attenuation_VS;
layout (location = 9) uniform vec3          direction_VS;

out vec2 TexCoord;
out vec3 LightPos;
out vec3 Brightness;

vec3 calcLightContributionForPointLight(vec3 vertexPos, vec3 vertexNormal, vec3 lightPos, vec3 Lcolour, vec3 Lk) 
{
  vec3 LV = lightPos - vertexPos;
  //vec3 LV = lightPos;
  float lengthLV = length(LV);
  // convert LV to unit length
  LV = normalize(LV);

  // Attenuation based on distance value lengthD
  float att = 1.0 / (Lk.x + Lk.y * lengthLV + Lk.z * lengthLV * lengthLV);

  // Lambertian
  float lambertian = clamp(dot(LV, vertexNormal), 0.0, 1.0);

  // Return final brightness for light source L
  return lambertian * att * Lcolour;
}

//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{    
    TexCoord = texCoord_VS;
    vec3 diffuse = vec3(0.1, 0.1, 0.1);
    
    LightPos = lightPos_VS;
    
    vec4 vn = vec4(modelMatrix * vec4(vec3(vertexNormal_VS), 0.0));
    vec3 normal = vec3(vn.xyz);

    Brightness = diffuse + calcLightContributionForPointLight(vertexPos_VS, normal, lightPos_VS, color_VS, attenuation_VS);

    gl_Position   = (projMatrix * viewMatrix * modelMatrix * vec4(vertexPos_VS, 1.0));
}

