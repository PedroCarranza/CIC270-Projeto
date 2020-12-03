#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

out vec2 v_TexCoord;

uniform float yOffset;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //gl_Position = projection * view * model * vec4(position.xyz, 1.0);
    gl_Position = vec4(position.xyz, 1.0);
    v_TexCoord = textureCoord;
}