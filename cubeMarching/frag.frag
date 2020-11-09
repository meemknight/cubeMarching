#version 330

in vec2 outPos;
out vec4 color;

uniform vec4 u_color;

void main()
{
	color = u_color;
	
}