// This is the skybox cpp file which has parts taken from Cube Maps: Sky Boxes and Environment Mapping Anton Gerdelan.Last Updated 2 October 2016
// With added functionality to assign a shader 
// Mitchell Flaherty 30/11/2018
//James Mitchell Flaherty april 2019
//Added fucntionality to take textures from Main() so they can be changed mid run
//Plus exectuted differently
#pragma once 
#include "skybox.h"
#include <iostream>
#include <stdio.h>

#define FRONT "posFront.jpg"
#define BACK "posBack.jpg"
#define TOP "posy.jpg"
#define BOTTOM "posBot.jpg"
#define LEFT "posLeft.jpg"
#define RIGHT "posRight.jpg"

using namespace std;
using namespace glm;

/* Define buffer object indices */
GLuint quadVbo;

/* Define textureID*/
GLuint textureID;

//GLuint cubeTexture;
GLuint texCube;

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}
//here is set the shade and then create the vertex arrays
void Skybox::createSBO(GLuint program)
{
	glGenVertexArrays(1, &SkyArrayObj);
	glBindVertexArray(SkyArrayObj);
	program1 = program;
	glUseProgram(program1);
	static const GLfloat SkyboxVertexs[] =
	{
		// Vertex positions
	  -40.0f,  40.0f, -40.0f,
	  -40.0f, -40.0f, -40.0f,
	   40.0f, -40.0f, -40.0f,

	   40.0f, -40.0f, -40.0f,
	   40.0f,  40.0f, -40.0f,
	  -40.0f,  40.0f, -40.0f,

	  -40.0f, -40.0f,  40.0f,
	  -40.0f, -40.0f, -40.0f,
	  -40.0f,  40.0f, -40.0f,
	  -40.0f,  40.0f, -40.0f,
	  -40.0f,  40.0f,  40.0f,
	  -40.0f, -40.0f,  40.0f,

	   40.0f, -40.0f, -40.0f,
	   40.0f, -40.0f,  40.0f,
	   40.0f,  40.0f,  40.0f,
	   40.0f,  40.0f,  40.0f,
	   40.0f,  40.0f, -40.0f,
	   40.0f, -40.0f, -40.0f,

	  -40.0f, -40.0f,  40.0f,
	  -40.0f,  40.0f,  40.0f,
	   40.0f,  40.0f,  40.0f,
	   40.0f,  40.0f,  40.0f,
	   40.0f, -40.0f,  40.0f,
	  -40.0f, -40.0f,  40.0f,

	  -40.0f,  40.0f, -40.0f,
	   40.0f,  40.0f, -40.0f,
	   40.0f,  40.0f,  40.0f,
	   40.0f,  40.0f,  40.0f,
	  -40.0f,  40.0f,  40.0f,
	  -40.0f,  40.0f, -40.0f,

	  -40.0f, -40.0f, -40.0f,
	  -40.0f, -40.0f,  40.0f,
	   40.0f, -40.0f, -40.0f,
	   40.0f, -40.0f, -40.0f,
	  -40.0f, -40.0f,  40.0f,
	   40.0f, -40.0f,  40.0f
	   
	};

	//Here i send my data to the buffers and pass through my uniforms for my shader.
	glGenBuffers(1, &quadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertexs), &SkyboxVertexs, GL_STATIC_DRAW);
	SBmodelID = glGetUniformLocation(program1, "model");
	SBviewID = glGetUniformLocation(program1, "view");
	SBprojectionID = glGetUniformLocation(program1, "projection");
	//CreateSB(FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT, &cubeTexture);
}
/* Taken from Cube Maps: Sky Boxes and Environment Mapping
   Anton Gerdelan. Last Updated 2 October 2016*/

bool Skybox::TextureSB(GLuint texture, GLenum side_target, const char* file_name)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	int x, y, n;
	int force_channels = 4;
	unsigned char*  image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) 
		{
			fprintf(stderr, "ERROR: could not load %s\n", file_name);
			return false;
		}
	// copy the image file loaded in, onto the right side of the cube
	glTexImage2D(side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	free(image_data);
	return true;
}

void Skybox::CreateSB(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube)
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, tex_cube);
		// load each image and copy into a side of the cube-map texture
		Skybox::TextureSB(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
		Skybox::TextureSB(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
		Skybox::TextureSB(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
		Skybox::TextureSB(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
		Skybox::TextureSB(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
		Skybox::TextureSB(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
		// format cube map texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		int loc = glGetUniformLocation(program1, "tex1");
		if (loc >= 0) glUniform1i(loc, 0);
	}

//This displays the Skybox in conjunction with the projection matrix.
void Skybox::displaySB(GLuint program, glm::mat4 model, glm::mat4 projection, glm::mat4 viewlook, GLint cubeTexture)
{
	program1 = program;

	glClearColor(0.1f, 0.9f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program1);
	glUniformMatrix4fv(SBmodelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(SBviewID, 1, GL_FALSE, &viewlook[0][0]);
	glUniformMatrix4fv(SBprojectionID, 1, GL_FALSE, &projection[0][0]);
	viewlook = mat3(viewlook);
	viewlook = mat4(viewlook);

	//Draw our textured quad
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
	glDrawArrays(GL_TRIANGLES, 0, 128);
	glDisable(GL_TEXTURE_CUBE_MAP);
}

