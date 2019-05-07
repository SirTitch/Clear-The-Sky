/* BoxLid.h
This is the header File For my boxlid class
Mitchell Flaherty, 4/11/2018
*/

#include "BoxLid.h"


using namespace std;

/* Here im defining the vertex attributes for vertex positions and normals.
 */
BoxLid::BoxLid()
{
	numvertices = 10;
	
}


BoxLid::~BoxLid()
{
}

/* Make a box from hard-coded vertex positions and normals  */
void BoxLid::makeBoxLid()
{
	/* vertices for my box in 10 triangles */
	GLfloat vertexPositions[] =
	{
		//Back
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.04f, -0.25f,
		0.25f, -0.04f, -0.25f,

		0.25f, -0.04f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,

		//RIGHT
		0.25f, -0.04f, -0.25f,
		0.25f, -0.04f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.04f, 0.25f,
		0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		//Front
		0.25f, -0.04f, 0.25f,
		-0.25f, -0.04f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.04f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		//LEFT
		-0.25f, -0.04f, 0.25f,
		-0.25f, -0.04f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.04f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		//Top
		-0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, 0.25f,

		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, -0.25f,
	};
	/* Create the vertex buffer for the cube */
	glGenBuffers(1, &BoxLidpositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BoxLidpositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the vertex buffer for my box */
	float vertexColours[] = {
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,

		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,

		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,

		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,

	
	};
	/* Create the colours buffer for the cube */
	glGenBuffers(1, &BoxLidcolourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BoxLidcolourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* normals for my Box */
	GLfloat normals[] =
	{
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	};

	/* Create the normals  buffer for my box */
	glGenBuffers(1, &BoxLidnormalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BoxLidnormalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Draw my Box by bining the vertexBuffers and drawing triangles */
void BoxLid::drawBoxLid()
{
	/* This binds the vertexPositions to the first pointer */
	glBindBuffer(GL_ARRAY_BUFFER, BoxLidpositionBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* This then binds the colours to the 2nd pointer  */
	glBindBuffer(GL_ARRAY_BUFFER, BoxLidcolourBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* And finally we bind the normals the the 3rd pointer */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, BoxLidnormalsBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glPointSize(3.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, numvertices * 3);
	
}