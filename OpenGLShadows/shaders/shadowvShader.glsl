

#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

uniform mat4 cameraMatrix;
uniform mat4 worldMatrix;
uniform mat4 lightSpaceMatrix;

out vec3 fragPosition;
out vec2 uv;
out vec3 normal;
out vec4 fragLSPosition;

void main()
{
    //vertex position in world space
    fragPosition= vec3(worldMatrix*vec4(position,1.0));

    //Vertex normal in world space
    normal = transpose(inverse(mat3(worldMatrix))) * in_normal;
    uv=in_uv;

    //Vertex position from light perspective
    fragLSPosition = lightSpaceMatrix * vec4(fragPosition, 1.0);

    //Vertex position in camera space (pixel position)
    gl_Position = cameraMatrix * worldMatrix * vec4(position, 1.0);
}  