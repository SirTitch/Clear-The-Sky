//My shader file is a bit confusing, i use lightmode uniform as a way to tell the main program what type of
//Lighting to use.
//Mitchell Flaherty 30/11/2018
//James Mitchell Flaherty April 2019
//Edited to work with newer work, depreciated some of the limitmode stuff into seperate shaders


#version 420 core

// Define the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 texcoord;

// This is the output vertex colour
out vec4 fcolour;
out float fdistance;
out vec3 ftexcoord;

// These are the uniforms that are defined in the application
uniform mat4 model, view, projection;
uniform mat3 normalmatrix;
uniform uint  emitmode, lightmode;
uniform vec4 lightpos;

uniform uint attenuationmode;

// Global constants (for this vertex shader)
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);
vec3 global_ambient = vec3(0.5, 0.5, 0.5);
int  shininess = 8;
vec4 ambient = vec4(0.3, 0.3,0.3,1.0);
vec3 light_dir = vec3(0.0, 0.0, 8.0);

void main()
{
	vec3 emissive = vec3(0);				
	vec4 position_h = vec4(position, 1.0);	
	vec4 diffuse_albedo;					
	vec3 light_pos3 = lightpos.xyz;
	vec4 diffuse_colour = vec4(0.5,0.5,0,1.0);
	vec4 specular_colour = vec4(0.1, 0.1, 0.1, 1.0);
	
	if (lightmode == 0)
	{
	//In this lightmode im using the basic phong reflection shader and poslight for my first light
	//Which is used to go around the box as in the first assignment.
			diffuse_albedo = colour;

			vec3 ambient = diffuse_albedo.xyz *0.2;
			// Define our vectors to calculate diffuse and specular lighting
			mat4 mv_matrix = view * model;		
			vec4 P = mv_matrix * position_h;	
			vec3 N = normalize(normalmatrix * normal);		
			vec3 L = light_pos3 - P.xyz;		
			float distanceToLight = length(L);	
			L = normalize(L);				
			vec3 diffuse = abs(dot(N, L)) * diffuse_albedo.xyz;
			vec3 V = normalize(-P.xyz);	
			vec3 R = reflect(-L, N);
			vec3 specular = pow(max(dot(R, V), 0.0), shininess) * specular_albedo;
			float attenuation;
			if (attenuationmode != 1)
			{
				attenuation = 1.0;
			}
			else
			{
				float attenuation_k1 = 0.5;
				float attenuation_k2 = 0.5;
				float attenuation_k3 = 0.5;
				attenuation = 1.0 / (attenuation_k1 + attenuation_k2*distanceToLight + 
										   attenuation_k3 * pow(distanceToLight, 2));
			}


			if (emitmode == 1) emissive = vec3(1.0, 1.0, 0.8); 
			fcolour = vec4(attenuation*(diffuse + specular) + emissive + global_ambient, 1.0);
			gl_Position = projection * view * model * position_h;
			fdistance = length(P.xyz);		
	}
	else if (lightmode == 1)
	{
			diffuse_colour = colour;
			ambient = diffuse_colour * 0.2;

			mat4 mv_matrix = view * model;
			mat3 normalmatrix = mat3(mv_matrix);
			vec3 N = mat3(mv_matrix) * normal;
			N = normalize(N);
			light_dir = normalize(light_dir);

			vec3 diffuse = max(dot(N, light_dir), 0.0) * diffuse_colour.xyz;

			vec4 P = position_h * mv_matrix;
			vec3 half_vec = normalize(light_dir + P.xyz);
			vec4 specular = pow(max(dot(N, half_vec), 0.0), shininess) * specular_colour;

			// Define the vertex colour
			fcolour = vec4(diffuse,1.0) + ambient + specular;

			// Define the vertex position
			gl_Position = projection * view * model * position_h;
			fdistance = length(P.xyz);
		
	}
	else if (lightmode == 3)
	{
	//Here in the lightmode 3 im using the same idea for terrain but putting it on my tree's to give them colour.
		//diffuse_colour = vec4(0.8,0.6,0.2,1.0);
		diffuse_colour = colour;
		if (position.y <= 0)
		{
			diffuse_colour = vec4(0.7, 0.3, 0, 1.0);
		}
		else if (position.y < 1.0)
		{
			diffuse_colour = vec4(0.7, 0.3, 0, 1.0);
		}
		else if (position.y < 2.0)
		{
			diffuse_colour = vec4(0.7, 0.3, 0, 1.0);
		}
		else
		{
			diffuse_colour = vec4(0, 1, 0, 1.0);
		}
			ambient = diffuse_colour * 0.2;

			mat4 mv_matrix = view * model;
			mat3 normalmatrix = mat3(mv_matrix);
			vec3 N = mat3(mv_matrix) * normal;
			N = normalize(N);
			light_dir = normalize(light_dir);

			vec3 diffuse = max(dot(N, light_dir), 0.0) * diffuse_colour.xyz;

			vec4 P = position_h * mv_matrix;
			vec3 half_vec = normalize(light_dir + P.xyz);
			vec4 specular = pow(max(dot(N, half_vec), 0.0), shininess) * specular_colour;

			// Define the vertex colour
			fcolour = vec4(diffuse,1.0) + ambient + specular;

			// Define the vertex position
			gl_Position = projection * view * model * position_h;
			fdistance = length(P.xyz);
		
	}
	
	
}

