#version 330 core

out vec4 fColor;

uniform vec4 ourColor;

void main()
{
    fColor = ourColor;
}