/* Box.h
This is the header File For my box class
Mitchell Flaherty, 30/11/2018
*/

#include "Box.h"

using namespace std;

/* Here im defining the vertex attributes for vertex positions and normals.
 */
Box::Box()
{
	verticesNo = 10;	
}


Box::~Box()
{
}


/* Make a box from hard-coded vertex positions and normals  */
void Box::makeBox()
{
	/* vertices for my box in 10 triangles */
	GLfloat vertexPositions[] =
	{
		//Back
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,

		//RIGHT
		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		//Front
		0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		//LEFT
		-0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		//bottom
		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, 0.25f,

	};
	/* Create the vertex buffer for my box */
	glGenBuffers(1, &BoxPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BoxPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* colours for our box */
	float vertexColours[] =
	{
		
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

		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,
		1.f, 0.5f, 0.f, 1.0f,


	};
	/* Create the colours buffer for my box */
	glGenBuffers(1, &BoxcolourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BoxcolourBuffer);
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
		
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
	};
	
	/* Create the normals  buffer for my box */
	glGenBuffers(1, &BoxnormBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BoxnormBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/* Draw my Box by bining the vertexBuffers and drawing triangles */
void Box::drawBox()
{
	/* This binds the vertexPositions to the first pointer */
	glBindBuffer(GL_ARRAY_BUFFER, BoxPosBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* This then binds the colours to the 2nd pointer  */
	glBindBuffer(GL_ARRAY_BUFFER, BoxcolourBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* And finally we bind the normals the the 3rd pointer */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, BoxnormBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glPointSize(3.f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, verticesNo * 3);
	
}