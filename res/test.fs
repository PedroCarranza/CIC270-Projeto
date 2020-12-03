#version 330 core
in vec2 v_TexCoord;

out vec4 FragColor;

uniform bool useTex;
uniform sampler2D u_Texture;

void main()
{
    FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}