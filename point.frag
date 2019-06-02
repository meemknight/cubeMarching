#version 400

in vec2 outPos;
out vec4 color;

void main()
{

	gl_PointSize = 4;
	color = vec4(0.5,0,0, 1);
	
}