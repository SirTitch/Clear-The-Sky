/* tiny_loader_texture.h
Example class to demonstrate the use of TinyObjectLoader to load an obj (WaveFront)
object file with normals and texture coordinates, and copy the data into vertex, normal texture coordinate buffers.

Iain Martin November 2018
James Mitchell Flaherty April 2019
Changed to take more variables from main to effectively texture each object loaded through it
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class TinyObjLoader
{
public:
	TinyObjLoader();
	~TinyObjLoader();

	void load_obj(GLuint program3, std::string inputfile, std::string txtfile, int texloc, std::string texname, bool debugPrint = false);
	void drawObject(GLuint program3, glm::mat4 model, glm::mat4 projection, glm::mat4 viewlook);

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint positionBufferObject;
	GLuint normalBufferObject;
	GLuint texCoordsObject, texID;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_texcoord;

	//int drawmode;
	GLuint numVertices;
	GLuint numNormals;
	GLint  numTexCoords;
	GLuint numPIndexes;
	GLuint prog;
	GLuint objmodelID, objcolourID, objviewID, objprojectionID;
};
