/*
Main.cpp
This is the main program for my game, it includes all the main fucntions such as display() and Main().
Here is where we load in all the objects and terrains, and render the scene for every frame.
Mitchell Flaherty 21/04/2019
*/

#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#pragma comment(lib, "glloadD.lib")
#else
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glload.lib")
#endif
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "audiere.lib")
#include <AntTweakBar.h>

#include "wrapper_glfw.h"
#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//Include Audiere to load in sounds
#include <audiere.h>
using namespace audiere;
// needed to make stack or matrices
#include <stack>

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/random.hpp"

// Include headers for objects
#include "Box.h"
#include "BoxLid.h"
#include "terrainObj.h"
#include "skybox.h"
#include "particleSys.h"
#include "SOIL.h"
#include "tiny_loader_texture.h"

//Here I load in all of my images for all of the skyboxes.
const char* FRONT1 = "./images/posFront.jpg";
const char* BACK1 = "./images/posBack.jpg";
const char* TOP1 = "./images/posY.jpg";
const char* BOTTOM1 = "./images/posBot.jpg";
const char* LEFT1 = "./images/posLeft.jpg";
const char* RIGHT1 = "./images/posRight.jpg";

const char* FRONT = "./images/siege_ft.tga";
const char* BACK = "./images/siege_bk.tga";
const char* TOP = "./images/siege_up.tga";
const char* BOTTOM = "./images/siege_dn.tga";
const char* LEFT = "./images/siege_lf.tga";
const char* RIGHT = "./images/siege_rt.tga";

const char* FRONT2 = "./images/grave_ft.tga";
const char* BACK2 = "./images/grave_bk.tga";
const char* TOP2 = "./images/grave_up.tga";
const char* BOTTOM2 = "./images/grave_dn.tga";
const char* LEFT2 = "./images/grave_lf.tga";
const char* RIGHT2 = "./images/grave_rt.tga";

//I make a cubmap texture for every skybox
GLuint cubeTexture;
GLuint cubeTexture2;
GLuint cubeTexture3;

/* Define textureID*/
GLuint texID;

//Here i am defining AntTweakBar's for the GUI
TwBar *bar;
TwBar *loadScreen;
TwBar *deathScreen;
TwBar *TimeBar;
TwBar *StartMenu;

double seconds, currentTime, td; //Used to store seconds to time animations
const int NUM_PROGRAMS = 2;

GLuint program[NUM_PROGRAMS];		/* Identifiers for the shader prgorams */
GLuint current_program;
//All of my shader objects are loaded into teh following variables
GLuint skyboxShader, paraShader, objShader, objShader2, objShader3, objShader4, objShader5; 

GLuint vao;		//Contains Our buffer Objects

//Define all of these for lightning effects
GLuint emitmode;
GLuint attenuationmode;
GLuint lightmode;

GLfloat speed;				// movement increment

GLfloat lightX, lightY, lightZ; //the light positons to be able to manipulate it

//Here im createing th global and local transformation variables
GLfloat  GlobalModelScale, GlobalZpos, GlobalYpos, GlobalXpos, GemZ, ViewX, ViewY, ViewZ, ViewZ2;
GLfloat  GemX, GemY, boxOpen, boxClosed, go, BoxXpos, BoxYpos, BoxZpos, scaler;
GLfloat TreeX, TreeY, TreeZ;
GLfloat paraX, paraY, paraZ;
GLfloat pedestalX, pedestalY, pedestalZ;
GLuint numlats, numlongs;	//Define the resolution of the sphere object

//These are defined to be able to change the size of each particle in the particle effect
GLfloat pointSizeID[NUM_PROGRAMS];


//uniforms to be passed through to the shader
GLuint parmodelID[NUM_PROGRAMS], parviewID[NUM_PROGRAMS], paraprojectionID[NUM_PROGRAMS];
GLuint modelID[NUM_PROGRAMS], viewID[NUM_PROGRAMS], projectionID[NUM_PROGRAMS], lightposID[NUM_PROGRAMS], lightpos2ID, normalmatrixID[NUM_PROGRAMS];
GLuint emitmodeID[NUM_PROGRAMS], attenuationmodeID[NUM_PROGRAMS], lightmodeID[NUM_PROGRAMS];
GLuint objmodelID, objcolourID, objviewID, objprojectionID, objnormalmatrix, objtextureID;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/

GLfloat startZpos, startYpos, startXpos;

//Here i init my objects
Box BoxBase;
BoxLid boxLid;
Skybox skybox;
Skybox skybox2;
Skybox skybox3;

//My loaded in objects.
TinyObjLoader TREE1;
TinyObjLoader TREETop;
TinyObjLoader pilla;
TinyObjLoader pillaBtn;
TinyObjLoader gem1;

//These are just variables used for animation
float btnpress = 0;
float btnAnim = 0;
float pInsp = 0;
float k = 0;
float s = 0;
float ly = 0;
float ly2 = 0;
float lz = 0;

//Here i define the variables need for my fps camera
float yaw1 = -90.0f;	
float pitch1 = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

//Loading in Terrain attributes
terrainObj *heightfield;
int octaves;
GLfloat Scale, Frequency;
GLfloat lSize;
GLfloat GroundZero = 0;

//Loading in particle attribs
particleSys *paraAnim;
GLfloat maxDist;
GLfloat pointSize;

//These are my state handlers
GLint storyCounter = 0; //Story counter is used to iterate through the story  
GLint MovePlayer = 0; //This is set to 1 when there should be animations to move the player
bool itemHandler = true; // This is used to move the gem
bool freeCamera = true; //was used to stop the player from moving the camera
GLint panCamera = 0;//This is set to 1 when there should be animations to move the player camera
bool camInput = true; //was used to stop the player from moving the camera as well with freecamera
bool pInput = true; //Used to stop player pressing buttons and messing up animations
bool gameStart = false; //used to define when the game starts and stops.
bool restart = false;  //Used to restart the game state
GLfloat Scene = 0; //Used to define the scene state
//These two are used to make sure the right animation plays in the first scene and not both
bool treelook = false; 
bool rInsp = false;  

//These are varibles for the fps camera
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
bool firstMouse = true;
glm::vec3 camlook;

//This is used to change the players scene
GLint changeScene = 0;

//These are used to work with the windowning system and anttweakbar
int height;
int width;
//This is the death timer
float dt = 50;

using namespace std;
using namespace glm;

//This defiens the story for the game to be loaded into the antweakbar
std::string storyPoint[32] = {
"This land is worn by the weather summoned by evil. You must find the great gem to dispel the weather.",//1
" ",//space 
"To get to the great gem it is said that you must find a way to raise the land",//3
" ",//spcae
"Before you lies's the key to do this, what will you do?",//5
" ",//space
"You are in front of the pedestal, what will you do?",//7
//SCENE 1
" ",
"Well done, you have raised the land!",//9
" ",//space 
"The great gem is said to be on top of the land you just managed to raise.",//11
" ",//spcae
"What will you do?",//13
" ",//space
"Will you go up the hill?",//15
//SCENE 2
" ",//space
"You have arrivied at the top of the hill ",//17
" ",//space
"There is a mysterious chest up ahead, what will you do? ",//19
" ",//space
"You are now infront of the chest, what will you do?" ,//21
" ",//space
"You have opened the chest and obtained the gem well done! you should return to the pedestal.",//23
//SCENE 4
" ",//space
" " ,//29
" ",//space//0
"You have arrived at the bottom of the hill and the land returns to what it was before. What will you do next?",//25
" ",//space
" Well done, you have placed the gem on the pedestal and returend the land to normal and dispelled the evil. Press 3",//27
" ",//space

"Well done you have completed the Clear The Sky Demo, thanks for playing!"
};

//all of these define the variable used to display information on the anttweakbar, 
//MainBar
std::string sp = "This land is worn by the weather summoned by evil. You must find the great gem to dispel the weather.";
std::string optionPoint1 = "Next";
std::string optionPoint2 = "";
std::string optionPoint3 = ""; 
std::string optionPoint4 = ""; 
std::string space = "";
//DeathSCreen
std::string deathMsg = "You Died";
//StartMenu
std::string Start = "To play this game";
std::string smd1 = "Use keys '1-4' to";
std::string smd2 = "select your option";
std::string smd3 = "from the action menu."; 
std::string smd4 = "Press 'S' to Start";
std::string smd5 = "Press 'ESC' to Exit";

//Creating an audio device
AudioDevicePtr device(OpenDevice());
//Defining all sounds for the game, all sounds are either my own or CCO licesed from opengameart.com
OutputStreamPtr openDoor (OpenSound(device, "./sounds/qubodup-DoorOpen05.ogg", false));
OutputStreamPtr BGM(OpenSound(device, "./sounds/scenebgm.ogg", true));
OutputStreamPtr BtnClick(OpenSound(device, "./sounds/ButtonClick.ogg", true));
OutputStreamPtr UiFeedback(OpenSound(device, "./sounds/morgantj turnsignal click2.wav", true));
OutputStreamPtr Rumble(OpenSound(device, "./sounds/rockRising.ogg", true));
OutputStreamPtr EGM(OpenSound(device, "./sounds/FadingPast.ogg", false));
OutputStreamPtr Win(OpenSound(device, "./sounds/Win sound.wav", false));

//Defines the mousecallback function
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Here init all of my positon variable and print out the commands
void init(GLWrapper *glw)
{
	//postion variables
	GlobalXpos = 20;
	GlobalYpos = 0;
	GlobalZpos = 5;

	//Anim variables
 	lz =0;
	ly2 = 0;
	ly = 0;
	s = 0;
	seconds = glfwGetTime();
	k = 1 + (float)(seconds*(0.5f + 0.5f));//For animation
	speed = 0.05f;
	//Global coords to be able to rotate and move the whole scene
	ViewZ2 = 0;

	GlobalModelScale = 1.f;
	TreeX = TreeY = TreeZ = 0;
	pedestalX = pedestalY = pedestalZ = 0;
	
	//The box and Box lid coords to help manipulate it
	boxOpen = 0;
	boxClosed = 0;
	go = 0;
	//View Coords
	ViewX = 0; ViewY = 0, ViewZ = 0;

	//Light Coords
	lightX = 0; lightY = 10.4f; lightZ = 0;

	//Gem Coords
	GemX = 0;
	GemY = 0;
	GemZ = 0;

	//Determien where the camera points and variables
	cameraFront.x = -0.983714;
	cameraFront.y = -0.0532074;
	cameraFront.z = 0.171686;
	yaw1 = -90.0f;
	pitch1 = 0.0f;
	lastX = 1024.0f / 2.0;
	lastY = 768.0f / 2.0;

	//Defines the particle positions
	paraX = 0;
	paraY = 0;
	paraZ = 0;
	//Used to decrement from the buttons position to pull off animation
	btnAnim = 10;
	 //Used for the resizing of the window
	 height = 1024;
	 width = 768;

	//Shader values im using the lightPos shader and built off of that
	const float roughness = 0.1;
	aspect_ratio = 1.3333f;	// Initial aspect ratio from window size (variables would be better!)
	emitmode = 0;		// 0=use colour attrib buffer, 1=colour defined in shaders
	attenuationmode = 1;

	//More from the sphere example
	numlats = 40;		// Number of latitudes in our sphere
	numlongs = 40;		// Number of longitudes in our sphere
	
	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);
	//Here i loade in my gem and tree
	
	// Create the heightfield object 
	octaves = 5.f;
	Scale = 2.f;
	Frequency = 0.9f;
	lSize = 50.f;
	heightfield = new terrainObj(octaves, Frequency, Scale, Scene);
	heightfield->createTerrain(50, 50, lSize, lSize);
	heightfield->setColourBasedOnHeight();
	heightfield->createTerrain();


	//Here i load in the vertex and fragment shader
	try
	{
			program[0] = glw->LoadShader("VertexShader.vert", "fragmentShader.frag");
			paraShader = glw->LoadShader("ParticleShader.vert", "ParticleShader.frag");
			skyboxShader = glw->LoadShader("SkyboxV.vert", "SkyboxF.frag");
			objShader = glw->LoadShader("object_loader_texture.vert", "object_loader_texture.frag");
			objShader2 = glw->LoadShader("object_loader_texture.vert","object_loader_texture.frag");
			objShader3 = glw->LoadShader("object_loader_texture.vert", "object_loader_texture.frag");
			objShader4 = glw->LoadShader("object_loader_texture.vert", "object_loader_texture.frag");
			objShader5 = glw->LoadShader("object_loader_texture.vert", "object_loader_texture.frag");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}
	for (int i = 0; i < NUM_PROGRAMS; i++)
	{
		//Here i define my uniforms to send to both shaders	
		glUseProgram(program[i]);
		modelID[i] = glGetUniformLocation(program[i], "model");
		emitmodeID[i] = glGetUniformLocation(program[i], "emitmode");
		attenuationmodeID[i] = glGetUniformLocation(program[i], "attenuationmode");
		viewID[i] = glGetUniformLocation(program[i], "view");
		projectionID[i] = glGetUniformLocation(program[i], "projection");
		lightposID[i] = glGetUniformLocation(program[i], "lightpos");
		normalmatrixID[i] = glGetUniformLocation(program[i], "normalmatrix");
		lightmodeID[i] = glGetUniformLocation(program[i], "lightmode");
	}

	objtextureID = glGetUniformLocation(objShader, "objtextureID");
	objtextureID = glGetUniformLocation(objShader2, "objtextureID");
	objtextureID = glGetUniformLocation(objShader3, "objtextureID");
	objtextureID = glGetUniformLocation(objShader4, "objtextureID");
	objtextureID = glGetUniformLocation(objShader5, "objtextureID");
	current_program = 0;

	//Here i create the light sphere which uses the example sphere, and make my own box, box lid and gem as well as terraina and skyboxes
	BoxBase.makeBox();
	boxLid.makeBoxLid();
	heightfield->drawTerrain();

	skybox.createSBO(skyboxShader);
	skybox.CreateSB(FRONT1, BACK1, TOP1, BOTTOM1, LEFT1, RIGHT1, &cubeTexture);

	skybox2.createSBO(skyboxShader);
	skybox2.CreateSB(FRONT2, BACK2, TOP2, BOTTOM2, LEFT2, RIGHT2, &cubeTexture2);

	skybox3.createSBO(skyboxShader);
	skybox3.CreateSB(FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT, &cubeTexture3);

	//Print out controls to console
	printf("Press 'S' to start or 'ESC' to exit");
	printf("Press keys 1 - 4 to choose the corresponding action on the menu");
	
	//Here i load in my objects and assign there textures
	pilla.load_obj(objShader, "pilla.obj", "./images/stone.jpg", 1, "tex1", true);
	pillaBtn.load_obj(objShader2, "pillabtn.obj", "./images/Stone.png", 2, "tex2", true);
	TREE1.load_obj(objShader3, "tree03.obj", "./images/barku.png", 3, "tex3", true);
	TREETop.load_obj(objShader4, "treetop.obj", "./images/lea.png", 4, "tex4", true);
	gem1.load_obj(objShader5, "gem.obj", "./images/Beryl.jpg", 0, "tex5", true);
	
	//Define particle animation variables
	speed = 1.f;
	maxDist = 100.f;
	paraAnim = new particleSys(1000, maxDist, speed);
	paraAnim->createPart(paraShader);
	pointSize = 1.f; 
	
	// Enable gl_PointSize
	glEnable(GL_PROGRAM_POINT_SIZE);

	//Here i make sure the gem and the particles are at ground level
	vec2 pos = heightfield->getGridPos(GemX, GemX);
	GemY = heightfield->heightAtPosition(GemX, GemY) - 4.5;
	vec2 pos2 = heightfield->getGridPos(paraX, paraX);
	paraY = heightfield->heightAtPosition(paraX, paraY) - 4.5;
	
}

//Called to update the display
void display()
{

	//glDepthMask(GL_FALSE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//Get Background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear buffers
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program[current_program]);//assign the shader

	// Projection matrix : 30° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	mat4 projection = perspective(radians(30.0f), aspect_ratio, 0.1f, 100.0f);
	mat3 normalmatrix;

	// View
	mat4 view = lookAt
	(
		vec3(GlobalXpos, GlobalYpos, GlobalZpos), //Camera is at in world space
		vec3(GlobalXpos, GlobalYpos, GlobalZpos) + cameraFront, // Camera looks at
		vec3(0, 1, 0) // Head is up
	);

	// Here i make sure that the player's camera is always at ground level
	vec2 pos = heightfield->getGridPos(GlobalXpos, GlobalZpos);
	GlobalYpos = heightfield->heightAtPosition(GlobalXpos, GlobalZpos) - 3;

	//Here i check if an animation is to play
	if (Scene == 0)
	{
		//First will move the player if they are in the first scene
		if (MovePlayer > 1)
		{
			td = glfwGetTime() - currentTime;//Time delta used to animate
			if (td < 3.1)
			{
				GlobalXpos = startXpos - ((td * 13) / 3);
				pInput = false; //This is used to stop the player from interupting the animation
			}
			else if (td > 3.1 && td <3.4) //Here i make sure the player can use the controls after the animation 
			{
				pInput = true;
			}
		}
		//THis will pan the camera
		if (panCamera > 0)
		{
			td = glfwGetTime() - currentTime;
			if (td < 5.1)
			{
				//Pan the camera to the pedestal
				if (pInsp == true)
				{
					cameraFront.x = cameraFront.x + (td*(-0.299388) / 5);
					cameraFront.y = cameraFront.y + (td*(-0.140037) / 5);
					cameraFront.z = cameraFront.z + (td*(0.943799) / 5);
					pInput = false;//Make sure the user can't interupt
				}
				//Pan the camera to the tress
				else if (treelook == true)
				{
					cameraFront.x = cameraFront.x + (td*(-0.712206) / 5);
					cameraFront.y = cameraFront.y + (td*(-0.0540788) / 5);
					cameraFront.z = cameraFront.z + (td*(-0.699885) / 5);
					pInput = false;
				}
			}
			else
			{
				pInput = true;
			}
		}
	}
	//If in scene 1
	else if (Scene == 1)
	{
		//Pan the camera up hill
		if (panCamera > 0)
		{
			td = glfwGetTime() - currentTime;
			if (td < 5.1)
			{
				cameraFront.x = cameraFront.x + (td*(-0.696224) / 5);
				cameraFront.y = cameraFront.y + (td*(0.372178) / 5);
				cameraFront.z = cameraFront.z + (td*(-0.613805) / 5);
				pInput = false;
			}
			else if (td > 5.1 && td < 5.2)
			{
				pInput = true;
			}
		}
	}
	else if (Scene == 2)
	{
		//if scene 2, move the player to the chest
		if (MovePlayer > 1)
		{
			td = glfwGetTime() - currentTime;
			if (td < 3.1)
			{
				GlobalZpos = startZpos + ((td * 15) / 3);
				pInput = false;
			}
			else if (td > 3.1 && td < 3.2)
			{
				pInput = true;
			}
		}
		if (panCamera > 0)
		{
			td = glfwGetTime() - currentTime;
			if (td < 3.1)
			{
				cameraFront.x = cameraFront.x + (td*(0.0161866) / 3);
				cameraFront.y = cameraFront.y + (td*(-0.373797) / 3);
				cameraFront.z = cameraFront.z + (td*(0.927369) / 3);
			}
		}
	}
	else if (Scene == 3)
	{
		//Lastly if last scene then pan the camera to show the cleared sky
		if (panCamera > 0)//Next i check if my go variable is active and if it is i'll animate the light and sphere so it mimics a fairy or bug flying around by using time.
		{
			td = glfwGetTime() - currentTime;
			if (td < 3.1)
			{
				cameraFront.x = cameraFront.x + (td*(0.529419) / 3);
				cameraFront.y = cameraFront.y + (td*(0.387516) / 3);
				cameraFront.z = cameraFront.z + (td*(-0.754683) / 3);
			}
		}
	}
	//animates the button if in that state
	if (btnpress > 0)
	{
		td = glfwGetTime() - currentTime;

		if (td < 0.9)
		{
			btnAnim = btnAnim + (td*(0.02f) / 0.8);
		}
		else if (td > 1.2)
		{
			Rumble->play(); //play rumble and set timer
			if (dt <= 50 && dt >= -5)
			{
				dt = dt - 0.5;
				cout << "DT: " << dt << endl;
			}
		}
	}
	//IF the death timer hits 0 then enter death screen and allow user to reset
	if (dt <= 0)
	{
		//showing the death svree using anttweakbar
		TwDefine(" timeBar visible = false");
		TwDefine(" DeathMENU visible = true");
		deathMsg = "The ground was shaking and a rock fell on your head and knocked you out, Press 5 to restart.";
		//Stop all music then start end music
		BGM->stop();
		Rumble->stop();
		EGM->play();
		//Allow player to reset game 
		gameStart = false;
		GlobalXpos = 20;
		GlobalYpos = 0;
		GlobalZpos = 8;
		cameraFront.x = -0.991091;
		cameraFront.y = -0.070627;
		cameraFront.z = 0.112921;
	}

	//Here im placign the light so its in view as soon as you get into the program
	vec4 lightpos = view * vec4(lightX, lightY, lightZ, 1.0);

	// Send projection and view uniforms to the shader	
	glUniformMatrix4fv(viewID[current_program], 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionID[current_program], 1, GL_FALSE, &projection[0][0]);

	glUniform4fv(lightposID[current_program], 1, &lightpos[0]);
	glUniform1ui(attenuationmodeID[current_program], attenuationmode);
	glUniform1ui(lightmodeID[current_program], lightmode);
	// Define our model transformation in a stack and 
	// push the identity matrix onto the stack

	stack<mat4> model;
	model.push(mat4(1.0f));
	//Draws the terrain
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(0, -5.f, 0));
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);
		lightmode = 1;
		glUniform1ui(lightmodeID[current_program], lightmode);
		lightmode = 3;
		heightfield->drawTerrain();
		heightfield->setColourBasedOnHeight();
		lightmode = 0;
		glUniform1ui(lightmodeID[current_program], lightmode);
		heightfield->setColourBasedOnHeight();
	}
	model.pop();
	//Checks what scene it is and then loads the objects and texture for that particular scene
	if (Scene == 0)
	{
		//Loads in the pedestal and button
		model.push(model.top());
		{
		vec2 pos3 = heightfield->getGridPos(pedestalX, pedestalZ);
		pedestalY = heightfield->heightAtPosition(pedestalX, pedestalZ) - 5.f;
		model.top() = translate(model.top(), vec3(pedestalX + 5, pedestalY, pedestalZ + 10));

		glUniform1ui(objtextureID,0);
		pilla.drawObject(objShader, model.top(), projection, view);		
		}
		model.pop();

		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(pedestalX, pedestalZ);
			pedestalY = heightfield->heightAtPosition(pedestalX, pedestalZ) - 5.f;
			model.top() = translate(model.top(), vec3(pedestalX + 5, pedestalY, pedestalZ+0.5 + btnAnim));

			glUniform1ui(objtextureID, 1);
			pillaBtn.drawObject(objShader2, model.top(), projection, view);
		}
		model.pop();
		//Then loads in all the tree's
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 5.1f;
			model.top() = translate(model.top(), vec3(TreeX + 10, TreeY, TreeZ));

			glUniform1ui(objtextureID, 2);
			TREE1.drawObject(objShader3, model.top(), projection, view);
			glUniform1ui(objtextureID, 3);
			TREETop.drawObject(objShader4, model.top(), projection, view);

		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 5.1f;
			model.top() = translate(model.top(), vec3(TreeX + 15, TreeY, TreeZ));
			TREE1.drawObject(objShader3, model.top(), projection, view);
			TREETop.drawObject(objShader4, model.top(), projection, view);
		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 5.1f;
			model.top() = translate(model.top(), vec3(TreeX + 15, TreeY, TreeZ - 5));
			TREE1.drawObject(objShader3, model.top(), projection, view);
			TREETop.drawObject(objShader4, model.top(), projection, view);
			glUniform1ui(objtextureID, 4);
		}
		model.pop();
	}
	if (Scene == 1)
	{
		//If second scene draw everything that was drawn last scene with animations applied
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(pedestalX, pedestalZ);
			pedestalY = heightfield->heightAtPosition(pedestalX, pedestalZ) - 12.f;
			model.top() = translate(model.top(), vec3(pedestalX + 5, pedestalY, pedestalZ + 10));
			glUniform1ui(objtextureID, 0);
			pilla.drawObject(objShader, model.top(), projection, view);
		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(pedestalX, pedestalZ);
			pedestalY = heightfield->heightAtPosition(pedestalX, pedestalZ) - 12.f;
			model.top() = translate(model.top(), vec3(pedestalX + 5, pedestalY, pedestalZ+0.5 + btnAnim));

			glUniform1ui(objtextureID, 1);
			pillaBtn.drawObject(objShader2, model.top(), projection, view);
		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 12.1f;
			model.top() = translate(model.top(), vec3(TreeX + 10, TreeY, TreeZ));

			glUniform1ui(objtextureID, 2);
			TREE1.drawObject(objShader3, model.top(), projection, view);
			glUniform1ui(objtextureID, 3);
			TREETop.drawObject(objShader4, model.top(), projection, view);
		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 12.1f;
			model.top() = translate(model.top(), vec3(TreeX + 15, TreeY, TreeZ));
			TREE1.drawObject(objShader3, model.top(), projection, view);
			TREETop.drawObject(objShader4, model.top(), projection, view);
		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 12.1f;
			model.top() = translate(model.top(), vec3(TreeX + 15, TreeY, TreeZ - 5));
			TREE1.drawObject(objShader3, model.top(), projection, view);
			TREETop.drawObject(objShader4, model.top(), projection, view);
			glUniform1ui(objtextureID, 4);
		}
		model.pop();
	
	}
	if (Scene == 2)
	{
		// This is the where we draw the box
		model.push(model.top());
		{
			//Here i use the hightfield to place the box ontop of the terrain.
			vec2 pos = heightfield->getGridPos(BoxXpos, BoxZpos);
			BoxYpos = heightfield->heightAtPosition(BoxXpos, BoxZpos) - 4.7f;
			// Box Transformations
			model.top() = translate(model.top(), vec3(BoxXpos, BoxYpos, BoxZpos));
			glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));
			normalmatrix = transpose(inverse(mat3(view * model.top())));
			glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);
			BoxBase.drawBox();//Draw the Box
		}
		model.pop();
		//Here i draw the box lid
		model.push(model.top());
		{
			//Here i use the hightfield to place the lid on top of the box on the terrain.
			vec2 pos = heightfield->getGridPos(BoxXpos, BoxZpos);
			BoxYpos = heightfield->heightAtPosition(BoxXpos, BoxZpos) - 4.7f;
			//Here i transform it so it fits on top of the box
			model.top() = translate(model.top(), vec3(BoxXpos, BoxYpos + 0.25f, BoxZpos));
			//Here i ues time and transformation to animate the box lid opening.
			if (boxOpen > 1)
			{
				model.top() = translate(model.top(), vec3(0, ly, lz));
				model.top() = rotate(model.top(), -radians(s), vec3(1, 0, 0));
				td = glfwGetTime() - currentTime;
				if (td < 0.6)
				{
					lz = td * -0.1;
					ly = td * -0.2;
					s = td * -270;
				}
				model.top() = translate(model.top(), vec3(0, 0, -0.5f));
			}
			//Same idea for the box being closed.
			if (boxClosed > 1)
			{
				//glfwSetTime(0);
				model.top() = translate(model.top(), vec3(BoxXpos + 1, -BoxYpos - 1.5, BoxZpos - 0.2));
				model.top() = scale(model.top(), vec3(1, 5, 0.2));
				model.top() = rotate(model.top(), -radians(-450.f), vec3(1, 0, 0));
				model.top() = translate(model.top(), vec3(BoxXpos - 1, BoxYpos, BoxZpos));
			}
			glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));
			normalmatrix = transpose(inverse(mat3(view * model.top())));
			glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);
			boxLid.drawBoxLid();//Draw the box lid.
		}
		model.pop();
		//Here i draw a gem that i made.
		model.push(model.top());
		{
			//Here i set up the controls and transform the gem so that its inside the box to begin with
			model.top() = translate(model.top(), vec3(GemX, GemY, GemZ));
			model.top() = scale(model.top(), vec3(0.1f, 0.1f, 0.1f));
			//Here i use a set of values that bring the gem out the box and spins it.
			if (go > 1)
			{
				td = glfwGetTime() - currentTime;
				float l = 1 + (float)(td*(100));
				if (td < 2.1)
				{
					ly2 = td * 0.8f;
					model.top() = rotate(model.top(), -radians(l), vec3(0, 1, 0));
					GemY = ((td*0.5) / 2) + startYpos;
				}
				else
				{
					model.top() = rotate(model.top(), -radians(l), vec3(0, 1, 0));
				}

			}
			//this is used so when the player picks up the gem it no longer gets drawn
			if (itemHandler == true)
			{
				glUniform1ui(objtextureID, 4);
				gem1.drawObject(objShader5, model.top(), projection, view);//Draw the gem
			}
		}
		model.pop();
	}

	if (Scene == 3)
	{
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(pedestalX, pedestalZ);
			pedestalY = heightfield->heightAtPosition(pedestalX, pedestalZ) - 5.f;
			model.top() = translate(model.top(), vec3(pedestalX + 5, pedestalY, pedestalZ + 10));

			glUniform1ui(objtextureID, 0);
			pilla.drawObject(objShader, model.top(), projection, view);
		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(pedestalX, pedestalZ);
			pedestalY = heightfield->heightAtPosition(pedestalX, pedestalZ) - 5.f;
			model.top() = translate(model.top(), vec3(pedestalX + 5, pedestalY, pedestalZ + 0.5 + btnAnim));

			glUniform1ui(objtextureID, 1);
			pillaBtn.drawObject(objShader2, model.top(), projection, view);
		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 5.1f;
			model.top() = translate(model.top(), vec3(TreeX, TreeY, TreeZ - 4));

			glUniform1ui(objtextureID, 2);
			TREE1.drawObject(objShader3, model.top(), projection, view);
			glUniform1ui(objtextureID, 3);
			TREETop.drawObject(objShader4, model.top(), projection, view);

		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 5.1f;
			model.top() = translate(model.top(), vec3(TreeX + 15, TreeY, TreeZ));
			TREE1.drawObject(objShader3, model.top(), projection, view);
			TREETop.drawObject(objShader4, model.top(), projection, view);

		}
		model.pop();
		model.push(model.top());
		{
			vec2 pos3 = heightfield->getGridPos(TreeX, TreeZ);
			TreeY = heightfield->heightAtPosition(TreeX, TreeZ) - 5.1f;
			model.top() = translate(model.top(), vec3(TreeX + 15, TreeY, TreeZ - 5));
			TREE1.drawObject(objShader3, model.top(), projection, view);
			TREETop.drawObject(objShader4, model.top(), projection, view);
			glUniform1ui(objtextureID, 4);
		}
		model.pop();
		model.push(model.top());
		{
			//Here i set up the controls and transform the gem so that its inside the box to begin with
			model.top() = translate(model.top(), vec3(GemX, GemY - 15, GemZ));
			model.top() = scale(model.top(), vec3(0.1f, 0.1f, 0.1f));
			//Here i use a set of values that bring the gem out the box and spins it.
			if (go > 1)
			{
				//glfwSetTime(0);
				td = glfwGetTime() - currentTime;
				float l = 1 + (float)(td*(100));
				if (td < 2.1)
				{
					//lightY = seconds * 0.8f;
					ly2 = td * 0.8f;
					model.top() = rotate(model.top(), -radians(l), vec3(0, 1, 0));
					GemY = ((td*0.5) / 2) + startYpos;
				}
				else
				{
					model.top() = rotate(model.top(), -radians(l), vec3(0, 1, 0));
				}

			}
			if (itemHandler == true)
			{
				glUniform1ui(objtextureID, 4);
				gem1.drawObject(objShader5, model.top(), projection, view);//Draw the gem
			}
		}
		model.pop();
	}	
	//Here i create my Skybox
	model.push(model.top());
	{
		model.top() = scale(model.top(), vec3(0.1f, 0.1f, 0.1f));
		model.top() = translate(model.top(), vec3(0, 500, 0));
		if (Scene == 0)
		{
			model.top() = translate(model.top(), vec3(0, 5000000, 0));
			skybox2.displaySB(skyboxShader, model.top(), projection, view, cubeTexture2);
		}
		if (Scene == 1 || Scene == 2)
		{
			model.top() = translate(model.top(), vec3(0, 500, 0));
			skybox2.displaySB(skyboxShader, model.top(), projection, view, cubeTexture2);
		}
		else
		{
			model.top() = translate(model.top(), vec3(0, 500, 0));
			skybox3.displaySB(skyboxShader, model.top(), projection, view, cubeTexture3);
		}
	}
	model.pop();

	//this is my particle animation thats meant to mimic dust
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(paraX, paraY, paraZ+50));

		glUniformMatrix4fv(parmodelID[paraShader], 1, GL_FALSE, &(model.top()[0][0]));

		paraAnim->drawPart(projection, view);
		paraAnim->animate();
	}
	model.pop();

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

//Here is the reshape window feature that will make sure the anttweakbar bars will fit the size of window it is in when it is reshaped
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
	TwWindowSize((float)w, (float)h);
	float size[2] = { ((w/100)*99), ((h/100)*17.5) };
	float pos[2] = { ((w/100)*1), ((h/100)*82.5) };
	

	float width = ((w / 100) * 80);

	TwSetParam(bar, NULL, "size", TW_PARAM_FLOAT, 2, size);
	TwSetParam(bar, NULL, "position", TW_PARAM_FLOAT, 2, pos);
	TwSetParam(bar, NULL, "valueswidth", TW_PARAM_FLOAT, 1, &width);

	float lSize[2] = { w, h };
	TwSetParam(loadScreen, NULL, "size", TW_PARAM_FLOAT, 2, lSize);

	float dbValue = ((w / 100) * 75);
	TwSetParam(deathScreen, NULL, "size", TW_PARAM_FLOAT, 2, lSize);
	TwSetParam(deathScreen, NULL, "valueswidth", TW_PARAM_FLOAT, 1, &dbValue);

	float timesize[2] = { (((float)w / 100) * 40), (((float)h / 100) * 10) };
	float timepos[2] = { (((float)w / 100) * 30), (((float)h / 100) * 50) };
	TwSetParam(TimeBar, NULL, "size", TW_PARAM_FLOAT, 2, timesize);
	TwSetParam(TimeBar, NULL, "position", TW_PARAM_FLOAT, 2, timepos);

	float MMsize[2] = { (((float)w / 100) * 20), (((float)h / 100) * 75) };
	float MMpos[2] = { ((w / 100) * 40), ((h / 100) * 12.5f) };
	float MMwidth = ((w/100)*50);
	TwSetParam(StartMenu, NULL, "size", TW_PARAM_FLOAT, 2, MMsize);
	TwSetParam(StartMenu, NULL, "position", TW_PARAM_FLOAT, 2, MMpos);
	TwSetParam(StartMenu, NULL, "valueswidth", TW_PARAM_FLOAT, 1, &MMwidth);
}

static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	bool recreate_terrain = false;		// Set to true if we want to recreate the terrain

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (pInput == true) //Here i check if the player can input
	{
		//This rests the game and starts a new
		if (key == 'S')
		{
			TwDefine(" StartMenu visible = False");
			TwDefine(" TweakBar visible = True");
			gameStart = true;
			restart = true;
		};
		//While the game is playing it can take in input that iteracts with the story
		if (gameStart == true) 
		{
			if (key == '1')
			{
				UiFeedback->play(); //Play a noise each click
				//Incriments and updates the Story and GUI
				if (storyCounter > -1 && storyCounter < 2)
				{
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					optionPoint1 = " ";
					optionPoint2 = "Next";
					cameraFront.x = -0.983714;
					cameraFront.y = -0.0532074;
					cameraFront.z = 0.171686;
				}
				if (storyCounter > 1 && storyCounter < 3)
				{

				}
				if (storyCounter > 3 && storyCounter < 5)
				{

				}
				//Inspects the pededstal, sets off animation to pan to the pedestal
				if (storyCounter > 5 && storyCounter < 7)
				{
					MovePlayer = 0;
					currentTime = glfwGetTime();
					panCamera = 1;
					sp = "After Inspecting the pedestal you notice there is an inscription and also a piece jutting out that looks like it can move.";
					optionPoint1 = "Inspect pedestal again?";
					optionPoint2 = "Push the piece";
					optionPoint3 = "Read Inscription";
					optionPoint4 = "";
					pInsp = pInsp + 1;
					rInsp = true;
					TwRefreshBar(bar);
				}
				//
				//
				//
				//SCENE 2
				//Update UI and player view and close load screen
				if (storyCounter > 7 && storyCounter < 9)
				{
					rInsp = false;
					GlobalXpos = 20;
					GlobalYpos = 0;
					GlobalZpos = 10;
					TwDefine(" LoadScreen visible = false");
					TwRefreshBar(loadScreen);
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
						cout << "storyCounter = " << storyCounter << endl;
					}
					optionPoint2 = "Next";
					sp = storyPoint[storyCounter];
					optionPoint4 = " ";
					cameraFront.x = -0.988022;
					cameraFront.y = -0.026177;
					cameraFront.z = -0.152074;
					TwRefreshBar(bar);
					btnpress = 0;
					Rumble->stop();
				}
				if (storyCounter > 9 && storyCounter < 11)
				{
				}
				if (storyCounter > 11 && storyCounter < 13)
				{
					/////////////////////////////////////////////
				}
				if (storyCounter > 13 && storyCounter < 15)
				{
				}
				//
				//
				//
				//SCENE 3
				//Advances story and closes load screen
				if (storyCounter > 15 && storyCounter < 17)
				{
					TwDefine(" LoadScreen visible = false");
					TwRefreshBar(loadScreen);
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];

					optionPoint1 = "";
					optionPoint2 = "Next";
					optionPoint3 = "";
					optionPoint4 = "";
					TwRefreshBar(bar);
				}
				if (storyCounter > 17 && storyCounter < 19)
				{
				}
				//The player loses when clicked so shows death menu, plays different music and changes game state so it has to be reset
				if (storyCounter > 19 && storyCounter < 21)
				{
					TwDefine(" DeathMENU visible = true");
					deathMsg = "You kicked the chest a bit too hard and fall unconsious due to the pain, Press 5 to restart.";
					BGM->stop();
					EGM->play();
					cameraFront.x = -0.991091;
					cameraFront.y = -0.070627;
					cameraFront.z = 0.112921;
					gameStart = false;
				}
				if (storyCounter > 21 && storyCounter < 23)
				{
				}
				//
				//
				//
				//SCENE 4 
				//Advances story and closes load screen, makes sure player is in right postiion and looking thr right way
				if (storyCounter > 23 && storyCounter < 25)
				{
					TwDefine(" LoadScreen visible = false");
					TwRefreshBar(loadScreen);
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					GlobalXpos = 5;
					GlobalYpos = 0;
					GlobalZpos = 2.5f;
					cameraFront.x = 0.0157054;
					cameraFront.y = 0.0174524;
					cameraFront.z = 0.999724;
					optionPoint1 = "";
					optionPoint2 = "Place the gem on the NovaBringer";
					optionPoint3 = "";
					optionPoint4 = "";
					TwRefreshBar(bar);
				}
				if (storyCounter > 25 && storyCounter < 27)
				{
				}
				if (storyCounter > 27 && storyCounter < 29)
				{
				}
				if (storyCounter > 29 && storyCounter < 31)
				{
				}
			};

			if (key == '2')
			{
			
				UiFeedback->play();
				//
				//
				//
				//SCENE 1
				if (storyCounter > -1 && storyCounter < 2)
				{}
				//Advances story and update sui
				if (storyCounter > 1 && storyCounter < 3)
				{
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					optionPoint2 = " ";
					optionPoint3 = "Move Towards pedestal";
					optionPoint4 = "Move Towards Tree's";
					TwRefreshBar(bar);
				}
				//Moves players sight back away from the tree and updates ui
				if (storyCounter > 3 && storyCounter < 5)
				{
					treelook = false;
					panCamera = 0;
					optionPoint1 = " ";
					optionPoint2 = " ";
					optionPoint3 = "Move Towards pedestal";
					optionPoint4 = "Move Towards Tree's";

					cameraFront.x = -0.983714;
					cameraFront.y = -0.0532074;
					cameraFront.z = 0.171686;
				}
				//Plays the button animation updates ui and story
				if (storyCounter > 5 && storyCounter < 7)
				{
					if (pInsp > 0)
					{
						MovePlayer = 0;
						panCamera = 0;
						currentTime = glfwGetTime();
						btnpress += 1.f;
						BtnClick->play();
						TwDefine(" timeBar visible = true");
						sp = "After pushing the button the ground around you starts rumbling";
						optionPoint1 = "";
						optionPoint2 = "";
						optionPoint3 = "";
						optionPoint4 = "Run to safety";
						TwRefreshBar(bar);
					
					}
					pInsp = 0;
				}
				//
				//
				//
				//SCENE 2
				if (storyCounter > 7 && storyCounter < 9)
				{
				}
				//Advances story and updates UI
				if (storyCounter > 9 && storyCounter < 11)
				{
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
						cout << "storyCounter = " << storyCounter << endl;
					}
					optionPoint1 = "";
					optionPoint2 = "";
					optionPoint3 = "Inspect the new hill";
					optionPoint4 = "";
					sp = storyPoint[storyCounter];
					TwRefreshBar(bar);
				}
				if (storyCounter > 11 && storyCounter < 13)
				{
				}
				if (storyCounter > 13 && storyCounter < 15)
				{
				}
				//
				//
				//
				//SCENE 3
				if (storyCounter > 15 && storyCounter < 17)
				{
				}
				//Advances story and updates UI, as well as movibg the player towards the chest
				if (storyCounter > 17 && storyCounter < 19)
				{
					panCamera = 1;
					MovePlayer = MovePlayer + 2;
					currentTime = glfwGetTime();
					startZpos = GlobalZpos;
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					optionPoint1 = "Kick it open?";
					optionPoint2 = "Push the Lid?";
					optionPoint3 = "Lift the Lid";
					TwRefreshBar(bar);
				}
				if (storyCounter > 19 && storyCounter < 21)
				{
				}
				//
				//
				//
				//SCENE 4
				if (storyCounter > 21 && storyCounter < 23)
				{
				}
				if (storyCounter > 23 && storyCounter < 25)
				{

				}
				//Places the gem on the pedestal and plays the win music and plays the gem animation and brings the light over
				if (storyCounter > 25 && storyCounter < 27)
				{
					Win->play();
					itemHandler = true;
					GemY = 12.6284;
					GemX = 5;
					GemZ = 10;
					go = go + 2;
					lightX = 5;
					lightY = -3;
					lightZ = 10;
					TwDefine(" LoadScreen visible = false");
					TwRefreshBar(loadScreen);
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					optionPoint1 = "";
					optionPoint2 = "";
					optionPoint3 = " ";
					optionPoint4 = "";
					TwRefreshBar(bar);
				}
				if (storyCounter > 27 && storyCounter < 29)
				{
				}
			};

			if (key == '3')
			{
				UiFeedback->play();
				//
				//
				//
				//SCENE 1
				if (storyCounter > -1 && storyCounter < 2)
				{
				}
				if (storyCounter > 1 && storyCounter && 3)
				{

				}
				//Advances story and updates UI
				if (storyCounter > 3 && storyCounter < 5)
				{
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					currentTime = glfwGetTime();
					startXpos = GlobalXpos;
					MovePlayer = MovePlayer + 2;
					optionPoint1 = "Inspect pedestal";
					optionPoint2 = " ";
					optionPoint3 = "";
					optionPoint4 = "";
					TwRefreshBar(bar);
				}
				//Advances story and updates UI
				if (storyCounter > 5 && storyCounter && 7)
				{
					if (rInsp == true)
					{
						sp = "The inscription reads: 'Place a special gem on top to activate this NovaBringer'";
						optionPoint1 = "Inspect pedestal again?";
						optionPoint2 = "Push the piece";
						optionPoint3 = "Read Inscription again?";
						optionPoint4 = "";
					}
				}
				//
				//
				//
				//SCENE 2
				if (storyCounter > 7 && storyCounter < 9)
				{
				}
				if (storyCounter > 9 && storyCounter < 11)
				{
				}
				//Sets of animation to look up the hill, updates Ui and story
				if (storyCounter > 11 && storyCounter < 13)
				{
					currentTime = glfwGetTime();

					panCamera = 1;
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
						cout << "storyCounter = " << storyCounter << endl;
					}
					optionPoint1 = "";
					optionPoint2 = "";
					optionPoint3 = "";
					optionPoint4 = "Go Up the hill";
					sp = storyPoint[storyCounter];
					TwRefreshBar(bar);
				}
				//updates UI
				if (storyCounter > 13 && storyCounter < 15)
				{
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					TwRefreshBar(bar);
				}
				//
				//
				//
				//SCENE 3
				if (storyCounter > 15 && storyCounter < 17)
				{
				}
				if (storyCounter > 17 && storyCounter < 19)
				{
				}
				//Sets off chest open animation and gem animation
				if (storyCounter > 19 && storyCounter < 21)
				{
					MovePlayer = 0;
					panCamera = 0;
					currentTime = glfwGetTime();
					startYpos = GemY;
					boxOpen += 2;
					go += 2;
					openDoor->play();
					cout << "BOXOPEN = " << boxOpen << endl;
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					optionPoint4 = "Take the Gem and go back to the pedestal!";
					sp = storyPoint[storyCounter];

					optionPoint1 = "";
					optionPoint2 = "";
					optionPoint3 = "";
					TwRefreshBar(bar);
				}

				if (storyCounter > 21 && storyCounter < 23)
				{
				}
				//
				//
				//
				//SCENE 4
				if (storyCounter > 23 && storyCounter < 25)
				{
				}
				if (storyCounter > 25 && storyCounter < 27)
				{
				}
				//pans the camera to see the clear sky
				if (storyCounter > 27 && storyCounter < 29)
				{
					TwDefine(" LoadScreen visible = false");
					TwRefreshBar(loadScreen);
					currentTime = glfwGetTime();
					panCamera = 1;
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					TwRefreshBar(bar);
					optionPoint4 = "Click to go back to the main menu";
				}
				if (storyCounter > 29 && storyCounter < 31)
				{
					itemHandler = true;
					optionPoint4 = "Click to go back to the main menu";
				}
			};

			if (key == '4')
			{
				UiFeedback->play();
				//
				//
				//
				//SCENE 1
				if (storyCounter > -1 && storyCounter < 2)
				{

				}
				if (storyCounter > 1 && storyCounter < 3)
				{

				}
				//sets of tree view animation and updates UI
				if (storyCounter > 3 && storyCounter < 5)
				{
					treelook = true;
					MovePlayer = 0;
					currentTime = glfwGetTime();
					panCamera = 1;
					sp = "There doesn't seem to be anything here, just tree's.";
					optionPoint1 = "Inspect the tree's again?";
					optionPoint2 = "Turn back";
					optionPoint3 = " ";
					optionPoint4 = " ";
				}
				//Stops timer and advances the story and makes the load screen appear
				if (storyCounter > 5 && storyCounter < 7)
				{
					currentTime = glfwGetTime();
					panCamera = 0;
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
						cout << "storyCounter = " << storyCounter << endl;
					}
					Scene += 1.f;
					TwDefine(" LoadScreen visible = true");
					TwRefreshBar(loadScreen);
					recreate_terrain = true;
					sp = storyPoint[storyCounter];
					TwRefreshBar(bar);
					btnpress = 0;
					MovePlayer = 0;
					TwDefine(" timeBar visible = false");
				}
				//
				//
				//
				//SCENE 2
				if (storyCounter > 7 && storyCounter < 9)
				{
				}
				if (storyCounter > 9 && storyCounter < 11)
				{
				}
				if (storyCounter > 11 && storyCounter < 13)
				{
				}
				//advances the story and changes the scene and puts load screen up
				if (storyCounter > 13 && storyCounter < 15)
				{
					changeScene = changeScene + 1;
					currentTime = glfwGetTime();
					panCamera = 0;
					recreate_terrain = true;
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
						cout << "storyCounter = " << storyCounter << endl;
					}
					Scene += 1.f;
					TwDefine(" LoadScreen visible = true");
					TwRefreshBar(loadScreen);
					recreate_terrain = true;
					sp = storyPoint[storyCounter];
					cameraFront.x = 0.038087;
					cameraFront.y = -0.125333;
					cameraFront.z = 0.991383;
					GlobalXpos = 0;
					GlobalYpos = 0;
					GlobalZpos = -20;
					TwRefreshBar(bar);
				}
				//
				//
				//
				//SCENE 3
				if (storyCounter > 15 && storyCounter < 17)
				{
				}
				if (storyCounter > 17 && storyCounter < 19)
				{
				}
				if (storyCounter > 19 && storyCounter < 21)
				{
				}
				//advances the story and changes the scene and puts load screen up
				if (storyCounter > 21 && storyCounter < 23)
				{
					itemHandler = false;
					MovePlayer = 0;
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					TwRefreshBar(bar);
					Scene += 1.f;
					TwDefine(" LoadScreen visible = true");
					TwRefreshBar(loadScreen);
					recreate_terrain = true;
				}
				//
				//
				//
				//SCENE 4
				if (storyCounter > 23 && storyCounter < 25)
				{
				}
				if (storyCounter > 25 && storyCounter < 27)
				{
				}
				if (storyCounter > 27 && storyCounter < 29)
				{
					optionPoint4 = "Click to go back to the main menu";
				}
				//resets the game to main menu
				if (storyCounter > 29 && storyCounter < 31)
				{
					TwDefine(" StartMenu visible = True");
					TwDefine(" TweakBar visible = False");
					gameStart = false;
					TwDefine(" LoadScreen visible = false");
					TwRefreshBar(loadScreen);
					if (storyCounter < 30)
					{
						storyCounter = storyCounter + 2;
					}
					cout << "storyCounter = " << storyCounter << endl;
					sp = storyPoint[storyCounter];
					TwRefreshBar(bar);
				}
			};
		}
		// if the game is finished
		if (gameStart == false)
		{
			//restart the game 
			if (key == '5')
			{
				restart = true;
				gameStart = true;
			};
		}
	}
	//resets all variables and reloads the terrain
	if (restart == true)
	{
		GlobalXpos = 20;
		GlobalYpos = 0;
		GlobalZpos = 5;
		lz = 0;
		ly2 = 0;
		ly = 0;
		s = 0;
		seconds = glfwGetTime();
		k = 1 + (float)(seconds*(0.5f + 0.5f));//For animation
		speed = 0.05f;
		//Global coords to be able to rotate and move the whole scene
		ViewZ2 = 0;

		GlobalModelScale = 1.f;
		TreeX = TreeY = TreeZ = 0;
		pedestalX = pedestalY = pedestalZ = 0;

		//The box and Box lid coords to help manipulate it
		boxOpen = 0;
		boxClosed = 0;
		go = 0;
		//View Coords
		ViewX = 0; ViewY = 0, ViewZ = 0;

		//Light Coords
		lightX = 0; lightY = 10.4f; lightZ = 0;

		//Gem Coords
		GemX = 0;
		GemY = 0;
		GemZ = 0;


		cameraFront.x = -0.983714;
		cameraFront.y = -0.0532074;
		cameraFront.z = 0.171686;

		paraX = 0;
		paraY = 0;
		paraZ = 0;

		btnAnim = 10;
		storyCounter = 0;
		Scene = 0;
		sp = "This land is worn by the weather summoned by evil. You must find the great gem to dispel the weather.";
		optionPoint1 = "Next";
		optionPoint2 = "";
		optionPoint3 = "";
		optionPoint4 = "";
		space = "";
		deathMsg = "You Died";
		storyCounter = 0;
		MovePlayer = 0;
		itemHandler = true;
		freeCamera = true;
		panCamera = 0;
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		firstMouse = true;
		changeScene = 0;
		//cd = 1;
		camInput = false;
		pInput = true;
		pInsp = 0;
		dt = 50;

		treelook = false;
		rInsp = false;
		btnpress = 0;
		TwDefine(" DeathMENU visible = False");
		EGM->stop();
		BGM->play();
		//	BGM->setVolume(true);
		restart = false;
		recreate_terrain = true;
		cameraFront.x = -0.983714;
		cameraFront.y = -0.0532074;
		cameraFront.z = 0.171686;
	}
	//Rectreates the terrain on restart or scene chnge
		if (recreate_terrain)
		{
			delete heightfield;
			heightfield = new terrainObj(octaves, Frequency, Scale, Scene);
			heightfield->createTerrain(50, 50, lSize, lSize);
			heightfield->setColourBasedOnHeight();
			heightfield->createTerrain();
			vec2 pos = heightfield->getGridPos(GemX, GemX);
			GemY = heightfield->heightAtPosition(GemX, GemY) - 4.5;
			vec2 pos2 = heightfield->getGridPos(paraX, paraX);
			paraY = heightfield->heightAtPosition(paraX, paraY) - 4.5;
			vec2 pos3 = heightfield->getGridPos(GlobalXpos, GlobalZpos);
			GlobalYpos = heightfield->heightAtPosition(GlobalXpos, GlobalZpos) - 4;
			changeScene = 0;
		}
	}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//If the camera is to be move this will be set off, this could be used for user control but not for the purpose of the game
	if (camInput == false)
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw1 += xoffset;
		pitch1 += yoffset;
		//Stops the camera from flippingg 
		if (pitch1 > 89.0f)
			pitch1 = 89.0f;
		if (pitch1 < -89.0f)
			pitch1 = -89.0f;
		glm::vec3 front;
		front.x = cos(glm::radians(yaw1)) * cos(glm::radians(pitch1));
		front.y = sin(glm::radians(pitch1));
		front.z = sin(glm::radians(yaw1)) * cos(glm::radians(pitch1));
		cameraFront = glm::normalize(front);
		glfwSetCursorPos(window, lastX, lastY);
	}
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1280, 1024, "Clear The Sky Demo");
	TwWindowSize(1280, 1024);
	
	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}
	//define global variables to all antweakbar menus
	TwDefine(" GLOBAL fontscaling=1.2 ");
	TwDefine(" GLOBAL fontstyle=fixed ");
	TwDefine(" GLOBAL fontsize=3 ");

	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);
	//Set up start menu
	StartMenu = TwNewBar("StartMenu");
	TwDefine(" StartMenu label = 'Clear The Sky Demo'");
	TwAddVarRW(StartMenu, " ", TW_TYPE_STDSTRING, &Start, "");
	TwAddVarRW(StartMenu, "  ", TW_TYPE_STDSTRING, &smd1, "");
	TwAddVarRW(StartMenu, "   ", TW_TYPE_STDSTRING, &smd2, "");
	TwAddVarRW(StartMenu, "    ", TW_TYPE_STDSTRING, &smd3, "");
	TwAddVarRW(StartMenu, "o- ", TW_TYPE_STDSTRING, &smd4, "");
	TwAddVarRW(StartMenu, "o-", TW_TYPE_STDSTRING, &smd5, "");
	TwDefine(" StartMenu valueswidth =200");
	TwDefine(" StartMenu color='0 0 0'");
	TwDefine(" StartMenu alpha=255 ");

	// Create a main GUI
	bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.
	TwAddVarRW(bar, " ", TW_TYPE_STDSTRING, &sp, "");
	TwAddVarRW(bar, "-------", TW_TYPE_STDSTRING, &space, "");
	TwAddVarRW(bar, "1 -", TW_TYPE_STDSTRING, &optionPoint1, "");
	TwAddVarRW(bar, "2 -", TW_TYPE_STDSTRING, &optionPoint2, "");
	TwAddVarRW(bar, "3 -", TW_TYPE_STDSTRING, &optionPoint3, "");
	TwAddVarRW(bar, "4 -", TW_TYPE_STDSTRING, &optionPoint4, "");
	TwDefine(" TweakBar label = 'Clear The Sky Demo'");
	TwDefine(" TweakBar position='15 500'");
	TwDefine(" TweakBar refresh=0.01");
	TwDefine(" TweakBar valueswidth =800");
	TwDefine(" TweakBar color='0 0 0'");
	TwDefine(" TweakBar fontstyle = fixed");
	TwDefine(" TweakBar text=light");
	TwDefine(" TweakBar visible = False");

	// Create the timer 
	TimeBar = TwNewBar("timeBar");
	TwAddVarRW(TimeBar, " Timer: ", TW_TYPE_FLOAT, &dt, "");
	TwDefine(" timeBar size = '400 100'");
	TwDefine(" timeBar position='450 500'");
	TwDefine(" timeBar refresh=0.01");
	TwDefine(" timeBar valueswidth =fit");
	TwDefine(" timeBar color='0 0 0'");
	TwDefine(" timeBar fontstyle = fixed");
	TwDefine(" timeBar visible = False");
	TwDefine(" timeBar text=light");
	TwDefine(" timeBar label = '!Danger!'");
	
	//create the Load screen
	loadScreen = TwNewBar("LoadScreen");
	TwDefine(" LoadScreen label = 'Loading... Please Press 1 To Continue!'");
	TwDefine(" LoadScreen size = '1280 1024'");
	TwDefine(" LoadScreen valueswidth =500");
	TwDefine(" LoadScreen color = '0 0 0'");
	TwDefine(" LoadScreen position='0 0'");
	TwDefine(" LoadScreen refresh=0.5");
	TwDefine(" LoadScreen alpha=255 ");
	TwDefine(" LoadScreen text=light");
	TwDefine(" LoadScreen visible = False");

	//Create death screen
	deathScreen = TwNewBar("DeathMENU");
	TwAddVarRW(deathScreen, " ", TW_TYPE_STDSTRING, &deathMsg, "");
	TwDefine(" DeathMENU label = 'You were unable to complete the quest this time'");
	TwDefine(" DeathMENU size = '1280 1024'");
	TwDefine(" DeathMENU color = '0 0 0'");
	TwDefine(" DeathMENU position='0 0'");
	TwDefine(" DeathMENU alpha=255 ");
	TwDefine(" DeathMENU valueswidth =1000");
	TwDefine(" DeathMENU fontsize=3");
	TwDefine(" DeathMENU text=light");
	TwDefine(" DeathMENU visible = False");

	//Defining all sizes of the menus depending on the size of the window.

	float MMsize[2] = { ((1280.f / 100.f) * 20.f), ((1024.f / 100.f) * 75.f) };
	float MMpos[2] = { ((1280 / 100) * 44), ((1024 / 100) * 12.5f) };
	TwSetParam(StartMenu, NULL, "size", TW_PARAM_FLOAT, 2, MMsize);
	TwSetParam(StartMenu, NULL, "position", TW_PARAM_FLOAT, 2, MMpos);

	float size[2] = { ((1280 / 100) * 99), ((1024 / 100) * 17.5) };
	float pos[2] = { ((1280 / 100) * 4), ((1024 / 100) * 82.5) };
	TwSetParam(bar, NULL, "size", TW_PARAM_FLOAT, 2, size);
	TwSetParam(bar, NULL, "position", TW_PARAM_FLOAT, 2, pos);

	float lSize[2] = { 1280,1024};
	float width = ((1280 / 100) * 80);
	float dbValue = ((1280 / 100) * 80);

	TwSetParam(bar, NULL, "valueswidth", TW_PARAM_FLOAT, 1, &width);
	TwSetParam(loadScreen, NULL, "size", TW_PARAM_FLOAT, 2, lSize);
	TwSetParam(deathScreen, NULL, "valueswidth", TW_PARAM_FLOAT, 1, &dbValue);
	// Register the callback functions

	glw->setRenderer(display); //Play game loop
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);
	if (camInput == false)
	{
		glw->setMouseCallback(mouse_callback);
	}
	/* Output the OpenGL vendor and version */
	glw->DisplayVersion();
	// Draw tweak bars
	init(glw);
	// let's start the background music first
	BGM->setRepeat(true);
	//Set all the sound volumes
	openDoor->setVolume(0.1f);
	BtnClick->setVolume(0.1f); 
	UiFeedback->setVolume(0.1f); 
	Rumble->setVolume(0.1f); 
	BGM->setVolume(0.1f); 
	EGM->setVolume(0.1f); 
	Win->setVolume(0.1f); 
	BGM->play();
	glw->eventLoop(); //play event loop
	delete(glw);
	return 0;
}


