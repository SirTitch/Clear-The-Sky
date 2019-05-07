//Here im using the particle system provided in the labs and added the function to assign a shader as well as manipulate the particles 
//So that they mimic dust particles
//Mitchell Flaherty 30/11/2018

#pragma once

#include <glm/glm.hpp>
#include "wrapper_glfw.h"

class particleSys
{
public:
	particleSys(GLuint number, GLfloat dist, GLfloat sp);
	~particleSys();

	void createPart(GLuint program);
	void drawPart(glm::mat4 projection, glm::mat4 viewlook);
	void animate();
	void updateParams(GLfloat dist, GLfloat sp);
	void initpoint(int i);

	glm::vec3 *vertices;
	glm::vec3 *colours;
	glm::vec3 *velocity;
	GLfloat pointSize = 0.1f;
	GLuint prog;
	GLuint numpoints;		// Number of particles
	GLuint vertex_buffer;
	GLuint colour_buffer, texID;

	// Particle speed
	GLfloat speed;
	GLuint parmodelID, parcolourID, parpointsizeID, parviewID, parprojectionID;
	// Particle max distance fomr the origin before we change direction back to the centre
	GLfloat maxdist;
};

