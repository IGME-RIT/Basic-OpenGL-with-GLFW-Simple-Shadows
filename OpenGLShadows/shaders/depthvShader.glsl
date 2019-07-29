//Since we need to view the geometry from the perspective of the light,
//we input the light space matrix instead of the regular camera matrix

#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 worldMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * worldMatrix * vec4(position, 1.0);
    
}  