/* Box.h
This is the header File For my box class
Mitchell Flaherty, 4/11/2018
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Box
{
public:
	Box();
	~Box();

	void makeBox();
	void drawBox();
	GLuint BoxPosBuffer;
	GLuint BoxcolourBuffer;
	GLuint BoxnormBuffer;
	int verticesNo;
	
};
