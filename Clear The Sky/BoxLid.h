/* BoxLid.h
This is the header File For my boxlid class
Mitchell Flaherty, 4/11/2018
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class BoxLid
{
public:
	BoxLid();
	~BoxLid();

	void makeBoxLid();
	void drawBoxLid();
	GLuint BoxLidpositionBuffer;
	GLuint BoxLidcolourBuffer;
	GLuint BoxLidnormalsBuffer;
	int numvertices;
};
