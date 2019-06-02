#version 330

in layout(location = 0) vec3 pos;

uniform mat4 modelToWorldToview;

void main()
{
	gl_Position = modelToWorldToview * vec4(pos,1);
}