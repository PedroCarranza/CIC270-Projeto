#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normalI;
layout (location = 2) in vec2 textureCoord;

out vec3 fragPosition;
out vec3 normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position.xyz, 1.0);
    //normal = (model * vec4(normalI,1)).xyz;
    //normal = normalI;
    normal = mat3(transpose(inverse(model))) * normalI; 
    fragPosition = vec3(model * vec4(position, 1.0));
    TexCoord = textureCoord;
}