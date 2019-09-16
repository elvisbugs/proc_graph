#version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vColor;

out vec4 vertexColor;

uniform mat4 transform;

void main()
{
   gl_Position = transform * vPosition;
   vertexColor = vec4(vColor,1.0);
}