//This is skybox a skybox header file
// Mitchell Flaherty 30/11/2018
#pragma once
#include "wrapper_glfw.h"
#include <vector>
#include <stdio.h>
#include <glm/glm.hpp>
#include "stb_image.h"



class Skybox
{
public:
	Skybox();
	~Skybox();

	bool TextureSB(GLuint texture, GLenum side_target, const char* file_name);
	void createSBO(GLuint program);
	void CreateSB(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube);
	void displaySB(GLuint program, glm::mat4 model,glm::mat4 projection, glm::mat4 viewlook, GLint cubeTexture);

	GLfloat skyboxVertexs;
	GLuint texture;
	GLuint program1, SkyArrayObj;
	GLuint SBmodelID, SBviewID, SBprojectionID;
	GLenum side_target;
	const char* file_name;
};
