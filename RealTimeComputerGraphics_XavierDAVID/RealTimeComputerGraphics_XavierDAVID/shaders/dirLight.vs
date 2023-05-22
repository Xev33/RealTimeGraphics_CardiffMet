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

//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{    
    TexCoord    = texCoord_VS;
    vec3 diffuse = vec3(0.1, 0.1, 0.1);

    vec4 vn = vec4(modelMatrix * vec4(vec3(vertexNormal_VS), 0.0));
    vec3 normal = vec3(vn.xyz);

    vec3 lightdir = normalize(direction_VS);

    float lambertian = max(dot(normal, lightdir), 0.0);
    float att = 1.0 / (attenuation_VS.x + attenuation_VS.y + attenuation_VS.z);

    Brightness = (color_VS * lambertian * att) + diffuse;

    gl_Position   = (projMatrix * viewMatrix * modelMatrix * vec4(vertexPos_VS, 1.0));
}