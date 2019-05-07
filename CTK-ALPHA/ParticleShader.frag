//This is a basic frag shader for my particles that gets rid of the black bits on the texture
//Mitchell Flaherty 30/11/2018
#version 400

in vec4 fcolour;
out vec4 outputColor;

uniform sampler2D tex1;

void main()
{
	vec4 texcolour = texture(tex1, gl_PointCoord);

	/* Discard the black colours to avoid them overwritting other stars*/
	if (texcolour.r < 0.1 && texcolour.g < 0.1 && texcolour.b < 0.1) discard;
	

	outputColor = fcolour * texcolour;
}