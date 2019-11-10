#version 330 core
in vec4 vertexColor;
out vec4 fColor;

uniform bool rectOn = true;

void main()
{
	if(rectOn)
		fColor = vertexColor;
	else
		fColor = vec4(0,0,0,0);
}