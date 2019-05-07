//This shader checks what object is using it at one time and maps the right texture to it
//James Mitchell Flaherty april 2019

//

#version 420 core

in vec4 fcolour;
in float fdistance;
in vec2 ftexcoord;
out vec4 outputColor;
uniform uint objtextureID;
// Fog parameters
float fog_maxdist = 5.0;
float fog_mindist = 0.1;
vec4  fog_colour = vec4(0, 0, 0, 1.0);
const float fogDensity = 0.5;

layout (binding=2) uniform sampler2D tex1;
layout (binding=3) uniform sampler2D tex2;
layout (binding=4) uniform sampler2D tex3;
layout (binding=5) uniform sampler2D tex4;
layout (binding=6) uniform sampler2D tex5;


void main()
{
	outputColor = fcolour;
	vec4 texcolour = texture(tex1, ftexcoord);
	vec4 texcolour2 = texture(tex2, ftexcoord);
	vec4 texcolour3 = texture(tex3, ftexcoord);
	vec4 texcolour4 = texture(tex4, ftexcoord);
	vec4 texcolour5 = texture(tex5, ftexcoord);
	float fog_factor;
	float dist = abs(fdistance);
	// exponential fog
	fog_factor = 1.0 / exp(dist  * fogDensity);
	// Limit fog factor to range 0 to 1
	fog_factor = clamp(fog_factor, 0.0, 2.0);
	if (objtextureID == 0)
	{
	outputColor = (mix(fog_colour, fcolour, fog_factor)) *(texcolour);
	}
	if (objtextureID == 1)
	{
	outputColor = (mix(fog_colour, fcolour, fog_factor)) *(texcolour2);
	}
	if (objtextureID == 2)
	{
	outputColor =(mix(fog_colour, fcolour, fog_factor)) * (texcolour3);
	}
	if (objtextureID == 3)
	{
	outputColor = (mix(fog_colour, fcolour, fog_factor)) *(texcolour4);
	}
	if (objtextureID == 4)
	{
	outputColor = (mix(fog_colour, fcolour, fog_factor)) *(texcolour5);
	}
	//outputColor = fcolour * texcolour * texcolour2;
	//outputColor = fcolour * texcolour;

	//outputColor = texcolour;
}