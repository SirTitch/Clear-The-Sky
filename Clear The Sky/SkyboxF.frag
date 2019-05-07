#version 330 core
//Basic shader
//James Mitchell Flaherty april 2019
in vec4 fcolour;
in vec3 ftexcoord;
out vec4 outputColor;

uniform samplerCube tex1;

void main()
{
	outputColor = texture(tex1, ftexcoord);
}