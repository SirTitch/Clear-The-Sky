/* terrainObj.cpp
Iain Martin November 2018
James Mitchell Flaherty April 2019
Here i have just used the lab terrain provided in Graphics module and used it to create my own terrain.
Added differnt noise calculations for differnt scenes that means the terrain just needs recreated if the scene is changed
*/

#include "terrainObj.h"
#include <glm/gtc/noise.hpp>
#include "glm/gtc/random.hpp"
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace glm;

terrainObj::terrainObj(int octaves, GLfloat freq, GLfloat scale, GLfloat scene)
{
	xsize = 0;	
	zsize = 0;
	perlinOctaves = octaves;
	perlinFreq = freq;
	perlinScale = scale;
	heightScale = 1.f;
	Scene = scene;
}


terrainObj::~terrainObj()
{
	/* tidy up */
	if (vertices) delete[] vertices;
	if (normals) delete[] normals;
	if (colours) delete[] colours;
}


//Copy the vertices, normals and element indices into vertex buffers 
void terrainObj::createTerrain()
{
	/* Generate the vertex buffer object */
	glGenBuffers(1, &vboMeshVert);
	glBindBuffer(GL_ARRAY_BUFFER, vboMeshVert);
	glBufferData(GL_ARRAY_BUFFER, xsize * zsize * sizeof(vec3), &(vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	glGenBuffers(1, &vboMeshColours);
	glBindBuffer(GL_ARRAY_BUFFER, vboMeshColours);
	glBufferData(GL_ARRAY_BUFFER, xsize * zsize * sizeof(vec3), &(colours[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &vboMeshNorms);
	glBindBuffer(GL_ARRAY_BUFFER, vboMeshNorms);
	glBufferData(GL_ARRAY_BUFFER, xsize * zsize * sizeof(vec3), &(normals[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate a buffer for the indices
	glGenBuffers(1, &iboMeshEl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboMeshEl);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), &(elements[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


//Enable vertex attributes and draw object
void terrainObj::drawTerrain()
{
	int size;	// Used to get the byte size of the element (vertex index) array

	glBindBuffer(GL_ARRAY_BUFFER, vboMeshVert);
	glVertexAttribPointer(0,  3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboMeshColours);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vboMeshNorms);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboMeshEl);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Draw the triangle strips */
	for (GLuint i = 0; i < xsize - 1; i++)
	{
		GLuint location = sizeof(GLuint) * (i * zsize * 2);
		glDrawElements(GL_TRIANGLE_STRIP, zsize * 2, GL_UNSIGNED_INT, (GLvoid*)(location));
	}
	
}


/* Define the terrian heights */
/* Uses code adapted from OpenGL Shading Language Cookbook: Chapter 8 */
void terrainObj::calcNoise()
{
	/* Create the array to store the noise values */
	/* The size is the number of vertices * number of octaves */
	noise = new GLfloat[xsize * zsize * perlinOctaves];
	for (int i = 0; i < (xsize*zsize*perlinOctaves); i++) noise[i] = 0;

	GLfloat xfactor = 1.f / (xsize - 1);
	GLfloat zfactor = 1.f / (zsize - 1);
	GLfloat freq = perlinFreq;
	GLfloat scale = perlinScale;

	for (GLuint row = 0; row < zsize; row++)
	{
		for (GLuint col = 0; col < xsize; col++)
		{
			GLfloat x = xfactor * col;
			GLfloat z = zfactor * row;
			GLfloat sum = 0;
			GLfloat curentScale = scale;
			GLfloat currentFreq = freq;

			// Compute the sum for each octave
			for (GLuint oct = 0; oct < perlinOctaves; oct++)
			{
				vec2 p(x*currentFreq, z*currentFreq);
				GLfloat val = perlin(p) / curentScale;
				sum += val;
				GLfloat result = (sum + 1.f) / 2.f;

				// Store the noise value in our noise array
				noise[(row * xsize + col) * perlinOctaves + oct] = result;

				// Move to the next frequency and scale
				currentFreq *= 2.f;
				curentScale *= scale;
			}

		}
		//For each different scene we want to add things to the terrain
		if (Scene == 1)
		{
			//for scene two we create the base of a hill
			float sum2 = 0.01f;
			if (row > ((zsize/2) - 10) && row < ((zsize / 2) + 5))
			{
				for (GLuint col = ((zsize / 2) - 10); col < ((zsize/2) + 10); col++)
				{
					GLfloat x = xfactor * row;
					GLfloat z = zfactor * col;
					GLfloat sum = 0;
					GLfloat curentScale = scale;
					GLfloat currentFreq = freq;
					// Compute the sum for each octave
					for (GLuint oct = 0; oct < perlinOctaves; oct++)
					{
						sum += sum2;
						GLfloat result = (sum + 1.f) / 2.f;
						// Store the noise value in our noise array
						noise[(row * xsize + col) * perlinOctaves + oct] = result;
						// Move to the next frequency and scale
						currentFreq *= 2.f;
						curentScale *= scale;
						sum2 = sum2 - 0.0005f;
					}
					
				}
				for (GLuint col = 0; col < (zsize/2) -10; col++)
				{
					GLfloat x = xfactor * col;
					GLfloat z = zfactor * row;
					GLfloat sum = 0;
					GLfloat curentScale = scale;
					GLfloat currentFreq = freq;

					// Compute the sum for each octave
					for (GLuint oct = 0; oct < perlinOctaves; oct++)
					{
						sum += 0.01f;
						GLfloat result = (sum + 1.f) / 2.f;

						// Store the noise value in our noise array
						noise[(row * xsize + col) * perlinOctaves + oct] = result;

						// Move to the next frequency and scale
						currentFreq *= 2.f;
						curentScale *= scale;
					}

				}
				
			}
		}
		if (Scene == 2)
		{
			//For scene 3 we create a ridge right through the middle of the terrain, but defining rows in the middle and making them the all the same hight instead of random
			if (row > ((zsize / 2) - 5) && row < ((zsize / 2) + 5))
			{
				for (GLuint col = 0; col < xsize; col++)
				{
					GLfloat x = xfactor * col;
					GLfloat z = zfactor * row;
					GLfloat sum = 0;
					GLfloat curentScale = scale;
					GLfloat currentFreq = freq;

					// Compute the sum for each octave
					for (GLuint oct = 0; oct < perlinOctaves; oct++)
					{
						sum += 0.005f;
						GLfloat result = (sum + 1.f) / 2.f;

						// Store the noise value in our noise array
						noise[(row * xsize + col) * perlinOctaves + oct] = result;

						// Move to the next frequency and scale
						currentFreq *= 2.f;
						curentScale *= scale;
					}

				}
			}
		}
	}
}
		
	


/* Define the vertex array that specifies the terrain
   (xp, zp) specifies the pixel dimensions of the heightfield (x * y) vertices
   (xs, ys) specifies the size of the heightfield region in world coords
   */
void terrainObj::createTerrain(GLuint xp, GLuint zp, GLfloat xs, GLfloat zs, GLfloat sealevel)
{
	xsize = xp;
	zsize = zp;
	width = xs;
	height = zs;

	/* Scale heights in relation to the terrain size */
	heightScale = xs * 4.f;

	/* Create array of vertices */
	GLuint numVerts = xsize * zsize;
	vertices = new vec3[numVerts];
	normals = new vec3[numVerts];
	colours = new vec3[numVerts];

	/* First calculate the noise array which we'll use for our vertex height values */
	calcNoise();

	/* Define starting (x,z) positions and the step changes */
	GLfloat xpos = -width / 2.f;
	GLfloat xpos_step = width / GLfloat(xp);
	GLfloat zpos_step = height / GLfloat(zp);
	GLfloat zpos_start = -height / 2.f;

	/* Define the vertex positions and the initial normals for a flat surface */
	for (GLuint x = 0; x < xsize; x++)
	{
		GLfloat zpos = zpos_start;
		for (GLuint z = 0; z < zsize; z++)
		{
			GLfloat height = noise[(x*zsize + z) * perlinOctaves + perlinOctaves - 1];
			vertices[x*xsize + z] = vec3(xpos, (height - 0.5f)*heightScale, zpos);
			normals[x*xsize + z] = vec3(0, 1.0f, 0);		// Normals for a flat surface
			zpos += zpos_step;
		}
		xpos += xpos_step;
	}

	/* Define vertices for triangle strips */
	for (GLuint x = 0; x < xsize - 1; x++)
	{
		GLuint top = x * zsize;
		GLuint bottom = top + zsize;
		for (GLuint z = 0; z < zsize; z++)
		{
			elements.push_back(top++);
			elements.push_back(bottom++);
		}
	}

	// Define the range of terrina heights
	heightMax = xs / 2.f;
	heightMin = -heightMax;

	// Stretch the height values to a defined height range 
	stretchToRange(heightMin, heightMax);

	defineGrndZero(sealevel);

	// Calculate the normals by averaging cross products for all triangles 
	calcNormals();
}

/* Calculate normals by using cross products along the triangle strips
   and averaging the normals for each vertex */
void terrainObj::calcNormals()
{
	GLuint elementPos = 0;
	vec3 AB, AC, crossProduct;

	// Loop through each triangle strip  
	for (GLuint x = 0; x < xsize - 1; x++)
	{
		// Loop along the strip
		for (GLuint tri = 0; tri < zsize * 2 - 2; tri++)
		{
			// Extract the vertex indices from the element array 
			GLuint v1 = elements[elementPos];
			GLuint v2 = elements[elementPos + 1];
			GLuint v3 = elements[elementPos + 2];

			// Define the two vectors for the triangle
			AB = vertices[v2] - vertices[v1];
			AC = vertices[v3] - vertices[v1];

			// Calculate the cross product
			crossProduct = normalize(cross(AC, AB));

			// Add this normal to the vertex normal for all three vertices in the triangle
			normals[v1] += crossProduct;
			normals[v2] += crossProduct;
			normals[v3] += crossProduct;

			// Move on to the next vertex along the strip
			elementPos++;
		}

		// Jump past the last two element positions to reach the start of the strip
		elementPos += 2;
	}

	// Normalise the normals (this gives us averaged, vertex normals)
	for (GLuint v = 0; v < xsize * zsize; v++)
	{
		normals[v] = normalize(normals[v]);
	}
}

/* Stretch the height values to the range min to max */
void terrainObj::stretchToRange(GLfloat min, GLfloat max)
{
	/* Calculate min and max values */
	GLfloat cmin, cmax;
	cmin = cmax = vertices[0].y;
	for (GLuint v = 1; v < xsize*zsize; v++)
	{
		if (vertices[v].y < cmin) cmin = vertices[v].y;
		if (vertices[v].y > cmax) cmax = vertices[v].y;
	}

	// Calculate stretch factor
	GLfloat stretchFactor = (max - min) / (cmax - cmin);
	GLfloat stretchDiff = cmin - min;

	/* Rescale the vertices */
	for (GLuint v = 0; v < xsize*zsize; v++)
	{
		vertices[v].y = (vertices[v].y - stretchDiff) * stretchFactor;
	}
}


/* Calculate terrian colours */
void terrainObj::setColour(vec3 c)
{
	GLuint numVertices = xsize * zsize;

	// Loop through all vertices, set colours
	// You could set height relayed colout here if you want to or add in some random variationd
	for (GLuint i = 0; i < numVertices; i++)
	{
		vec3 heightNormal = normalize(vertices[i]);
		float height = vertices[i].y;
		colours[i] = c;
	}

}

/* Define a sea level in the terrain */
void terrainObj::defineGrndZero(GLfloat s)
{
	groundZero = s;
	for (int v = 0; v < xsize*zsize; v++)
	{
		if (vertices[v].y < groundZero)
		{
			vertices[v].y = groundZero;
		}
	}
}


/* Calculate terrian colours based on height with small random variations */
void terrainObj::setColourBasedOnHeight()
{
	GLuint numVertices = xsize * zsize;

	// Loop through all vertices, set colour based on height
	for (GLuint i = 0; i < numVertices; i++)
	{
		glm::vec3 height_normal = glm::normalize(vertices[i]);
		float height = vertices[i].y;

		// Scale height to range 0 to 1 to use to define colours
		height = (height - heightMin) / (heightMax - heightMin);
		float sea_norm = (groundZero - heightMin) / (heightMax - heightMin);

		// Some random values to use for colour selection
		float rand = glm::linearRand(0.0, 0.1);
		float rand2 = glm::linearRand(0.0, 0.05);
		float rand3 = glm::linearRand(0.0, 0.02);

		// Define colour based on normalised height (0 to 1)
		// with some random variations
		if (height <= sea_norm)
			colours[i] = glm::vec3(0.3 +rand2, 0.2+rand2,0.1);
		else if (height <= 0.52 + rand3)
			colours[i] = glm::vec3(0, 0.7 + rand2, 0);
		else if (height <= 0.6 + rand3)
			colours[i] = glm::vec3(0, 0.9 + rand, 0);
		else if (height <= 0.93 + rand3)
			colours[i] = glm::vec3(0.3 + rand2, 0.3 + rand2, 0.3 + rand2);
		else
			colours[i] = glm::vec3(0.3 + rand2, 0.3 + rand2, 0.3 + rand2);

	}

}

// Get height on terrain from world coordinates
// Rounds the floating point grid values to get the nearest (int) grid point
// Note that a more accurate algorithm would be a bilinear interpolation
// of the four nearest grid points
float terrainObj::heightAtPosition(GLfloat x, GLfloat z)
{
	// Get grid position in floating point
	vec2 gridPos = getGridPos(x, z);

	// Get integer grid position to
	int gx = round(gridPos.x);
	int gz = round(gridPos.y);

	// Get vertex number form inteegr grid position
	int vnum = gx * xsize + gz;

	// Check that the vertex number is in range
	// before getting the height value
	float gridHeight = 0;
	if (vnum > 0 && vnum < xsize*zsize)
	{
		gridHeight = vertices[vnum].y;
	}

	return gridHeight;
}

// Get a terrain height array gtid position from a world coordinate
// Note that this will only work if you DON'T scale and shift the terrain object
vec2 terrainObj::getGridPos(GLfloat x, GLfloat z)
{
	GLfloat Xgrid = ((x + (width / 2.f)) / width) * float(xsize);
	GLfloat Zgrid = ((z + (height / 2.f)) / height) * float(zsize);

	return vec2(Xgrid, Zgrid);
}
