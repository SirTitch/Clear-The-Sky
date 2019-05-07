//Here im using the particle system provided in the labs and added the function to assign a shader as well as manipulate the particles 
//So that they mimic dust particles
//Mitchell Flaherty April 2019


#include "particleSys.h"
#include "glm/gtc/random.hpp"
#include "SOIL.h"
#pragma comment(lib, "soil.lib")

using namespace glm;
using namespace std;
/* Constructor, set initial parameters*/
particleSys::particleSys(GLuint number, GLfloat dist, GLfloat sp)
{
	numpoints = number;
	maxdist = dist;
	speed = sp;
}


particleSys::~particleSys()
{
	delete[] colours;
	delete[] vertices;
}

void particleSys::updateParams(GLfloat dist, GLfloat sp)
{
	maxdist = dist;
	speed = sp;
}


void  particleSys::createPart(GLuint program2)
{
	prog = program2;
	glUseProgram(prog);

	vertices = new vec3[numpoints];
	colours = new vec3[numpoints];
	velocity = new vec3[numpoints];

	/* Define random colour and vertical velocity + small random variation */
	for (int i = 0; i < numpoints; i++)
	{
		// create the particle at the initial position
		initpoint(i);
	}

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(vec3), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colour_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(vec3), colours, GL_STATIC_DRAW);

	parmodelID = glGetUniformLocation(prog, "model");
	parpointsizeID = glGetUniformLocation(prog, "size");
	parviewID = glGetUniformLocation(prog, "view");
	parprojectionID = glGetUniformLocation(prog, "projection");

	try
	{
		glActiveTexture(GL_TEXTURE1);

		/* load an image file directly as a new OpenGL texture */
		texID = SOIL_load_OGL_texture("./images/fog.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if (texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", "circle.jpg");
		}


		int loc = glGetUniformLocation(prog, "tex2");
		if (loc >= 0) glUniform1i(loc, 2);
	}
	catch (exception &e)
	{
		printf("\nImage file loading failed!!.");
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	int loc = glGetUniformLocation(prog, "tex2");
	if (loc >= 0) glUniform1i(loc, 2);

	glEnable(GL_PROGRAM_POINT_SIZE);
}


void particleSys::drawPart(glm::mat4 projection, glm::mat4 viewlook)
{
	mat4 model = mat4(1.0f);
	glUseProgram(prog);
	glUniformMatrix4fv(parmodelID, 1, GL_FALSE, &model[0][0]);
	glUniform1f(parpointsizeID, pointSize);
	glUniformMatrix4fv(parviewID, 1, GL_FALSE, &viewlook[0][0]);
	glUniformMatrix4fv(parprojectionID, 1, GL_FALSE, &projection[0][0]);
	/* Bind  vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Draw our points*/
	glDrawArrays(GL_POINTS, 0, numpoints);
}


void particleSys::animate()
{
	for (int i = 0; i < numpoints; i++)
	{
		// Shift vertex position by velocity vector
		vertices[i] += velocity[i];

		// Add a small random value to the velocity
		velocity[i] += vec3(ballRand(linearRand(0.f, speed / 40.f)));

		// Calculate distance to the origin
		GLfloat dist = length(vertices[i]);

		// If we are too far away then kill the particle by starting at the origin again
		if (dist > (maxdist - linearRand(0.f, 0.5f)))
		{
			// restart thee particle at the initial position
			initpoint(i);
		}
		else
		{
			// Add a gravity effect so that the particles appear to float across the screen
			//velocity[i].x += 0.001f;
			velocity[i].y -= 0.0001f;
			velocity[i].z -= 0.0001f;
		}
	}

	// Update the vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(vec3), vertices, GL_DYNAMIC_DRAW);
}


// Set the initial particle conditions
void particleSys::initpoint(int i)
{
	float z = linearRand(-50, 50);
	vertices[i] = vec3(0, 5, z);//vec3(-10, -2, 0);
	colours[i] = vec3(1, 1, 1);//vec3(linearRand(0.2f, 0.3f), linearRand(0.4f, 0.5f), linearRand(0.8f, 1.0f));
	velocity[i] = vec3(0, 0.05, 0) + vec3(ballRand(linearRand(0.f, speed)));
	velocity[i] = normalize(velocity[i]) / 500.f;
}