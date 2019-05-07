// Im using the poslight shader and the fog lab to help me with this assignment
//Mitchell Flaherty 30/11/2018
//James Mitchell Flaherty April 2019


#version 420 core

in vec4 fcolour;
in float fdistance;
in vec3 ftexcoord;
out vec4 outputColor;

// Fog parameters
float fog_maxdist = 5.0;
float fog_mindist = 0.1;
vec4  fog_colour = vec4(0, 0, 0, 1.0);
const float fogDensity = 0.05;
uniform samplerCube tex1;
uniform uint lightmode;
void main()
{

	outputColor = fcolour;
	float fog_factor;
	float dist = abs(fdistance);
	// exponential fog
	fog_factor = 1.0 / exp(dist  * fogDensity);
	// Limit fog factor to range 0 to 1
	fog_factor = clamp(fog_factor, 0.0, 2.0);
	outputColor = mix(fog_colour, fcolour, fog_factor);
	if (lightmode == 2)
	{
	outputColor =texture(tex1, ftexcoord);
	}
}