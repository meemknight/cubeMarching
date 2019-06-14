#version 330

#define PI 3.141592
#define PRECISION 0.4
#define SIZE 0.1

layout(points) in;
layout(line_strip, max_vertices = 30) out;

void main() {    

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	for(float i=0; i<PI*2 + PRECISION; i+= PRECISION)
	{
		gl_Position = gl_in[0].gl_Position + vec4(sin(i) * SIZE, cos(i) * SIZE, 0, 0); 
		EmitVertex();

		i+= PRECISION;

		gl_Position = gl_in[0].gl_Position + vec4(sin(i) * SIZE, cos(i) * SIZE, 0, 0); 
		EmitVertex();

		gl_Position = gl_in[0].gl_Position;
		EmitVertex();

	}


	EndPrimitive();
}  