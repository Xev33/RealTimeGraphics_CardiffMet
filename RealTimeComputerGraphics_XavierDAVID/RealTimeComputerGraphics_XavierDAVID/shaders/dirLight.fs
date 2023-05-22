#version 450 core

layout(binding = 0) uniform sampler2D ColTexture;

in vec2 TexCoord;
in vec3 Brightness;

out vec4 FragColor; // Color that will be used for the fragment

//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
   vec4 colour = texture(ColTexture, TexCoord.xy);

    FragColor = vec4(vec3(colour.xyz * Brightness), 1.0);
}