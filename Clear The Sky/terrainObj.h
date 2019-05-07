#pragma once
/* terrain_object.h
   Example class to show how to create and render a height map
   Iain Martin November 2018
   James Mitchell Flaherty April 2019
Here i have just used the lab terrain provided in Graphics module and used it to create my own terrain.
Added differnt noise calculations for differnt scenes that means the terrain just needs recreated if the scene is changed
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class terrainObj
{
public:
	terrainObj(int octaves, GLfloat freq, GLfloat scale, GLfloat scene);
	~terrainObj();

	void calcNoise();
	void createTerrain(GLuint xp, GLuint yp, GLfloat xs, GLfloat ys, GLfloat sealevel = 0);
	void calcNormals();
	void stretchToRange(GLfloat min, GLfloat max);
	void setColour(glm::vec3 c);
	void setColourBasedOnHeight();
	void defineGrndZero(GLfloat s);
	float heightAtPosition(GLfloat x, GLfloat z);
	glm::vec2 getGridPos(GLfloat x, GLfloat z);
	void createTerrain();
	void drawTerrain();
	glm::vec3 *vertices;
	glm::vec3 *normals;
	glm::vec3 *colours;
	std::vector<GLuint> elements;
	GLfloat* noise;
	GLuint vboMeshVert;
	GLuint vboMeshNorms;
	GLuint vboMeshColours;
	GLuint iboMeshEl;
	GLuint xsize;
	GLuint zsize;
	GLfloat width;
	GLfloat height;
	GLuint perlinOctaves;
	GLfloat perlinFreq;
	GLfloat perlinScale;
	GLfloat heightScale;
	GLfloat groundZero;
	float heightMin, heightMax;	// range of terrain heights
	GLfloat Scene;
};
