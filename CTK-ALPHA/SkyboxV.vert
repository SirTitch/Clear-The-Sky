#version 330 core
//Basic shader
//James Mitchell Flaherty april 2019

// Define the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 texcoord;

// This is the output vertex colour
out vec4 fcolour;
out vec3 ftexcoord;

// These are the uniforms that are defined in the application
uniform mat4 model, view, projection;
uniform mat3 normalmatrix;

void main()
{
	vec4 position_h = vec4(position, 1.0);
	//fcolour = vec4(0.5,0.5,0.5,1.0);
	ftexcoord = position.xyz;
	gl_Position = projection * view * position_h;
	
	}