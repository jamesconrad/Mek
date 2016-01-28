// third-party libraries
#include "include\GL\glew.h"
#include "include\GL\glfw3.h"
#include "lib\glm\glm.hpp"
#include "lib\glm\gtx\vector_angle.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"
#include "lib\glm\gtx\rotate_vector.hpp"
#include "include\IL\ilut.h"

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <list>
#include <sstream>
#include <algorithm>

// classes
#include "Program.h"
#include "ComponentLight.h"
#include "Texture.h"
#include "Camera.h"
#include "ComponentInput.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Framebuffer.h"

#include "TextRendering.h"
#include "2dOverlayAnim.h"
#include "Target.h"
#include "Projectile.h"

enum game_state { GAME, MENU };

//Bad Inits need to fix at a later time
//Pls no kill future me.  I sorry
float runTime = 0;
std::vector<GameObject*> goVec;
std::vector<Target*> targets;
int targetsKilled = 0;
twodOverlay* crosshair;
twodOverlay* startscreen;
twodOverlayAnim* skull;
//todo: revert back to menu
game_state gameState = MENU;
Interpolation camInterp;
glm::vec3 fontColour = glm::vec3(117, 176, 221);
glm::vec3 spotLightColour = glm::vec3(158, 64, 60);
std::vector<unsigned int> scoreTable;
unsigned int score;
float playTime = 0;

Framebuffer* fb;

GameObject* animatedMech;
ComponentGraphics* animatedMechGC;
Terrain* ground;
Skybox* sky;
//TODO : World/Target Loading, Menu, Timer, Target Counter

void LoadShaders(char* vertFilename, char* fragFilename) 
{
	Program::getInstance().createShader("standard", GL_VERTEX_SHADER, vertFilename);
	Program::getInstance().createShader("standard", GL_FRAGMENT_SHADER, fragFilename);

	// load skinning shaders
	Program::getInstance().createShader("skinning", GL_VERTEX_SHADER, "shaders/skinning.vert");
	Program::getInstance().createShader("skinning", GL_FRAGMENT_SHADER, "shaders/skinning.frag");
	Program::getInstance().createShader("anim", GL_VERTEX_SHADER, "shaders/skinningA.vert");
	Program::getInstance().createShader("anim", GL_FRAGMENT_SHADER, "shaders/skinningA.frag");
	Program::getInstance().createShader("hud", GL_VERTEX_SHADER, "shaders/hud.vert");
	Program::getInstance().createShader("hud", GL_FRAGMENT_SHADER, "shaders/hud.frag");
}

// constants
const glm::vec2 SCREEN_SIZE(1920, 1080);

// globals
GLFWwindow* gWindow = NULL;
double gScrollY = 0.0;
GLfloat gDegreesRotated = 0.0f;

GameObject* model;
ComponentGraphics* gModel;
ComponentCollision* gCol;

float tElap = 0;

// returns a new Texture created from the given filename
static Texture* LoadTexture(char* filename) {
	Bitmap bmp;
	bmp.bitmapFromFile(filename);
    bmp.flipVertically();
    return new Texture(filename);
}


// initialises the gWoodenCrate global
static void LoadWoodenCrateAsset() {
    LoadShaders("shaders/vertex-shader.vert", "shaders/fragment-shader.frag");
}

// convenience function that returns a translation matrix
glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z) {
    return glm::translate(glm::mat4(), glm::vec3(x,y,z));
}


// convenience function that returns a scaling matrix
glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z) {
    return glm::scale(glm::mat4(), glm::vec3(x,y,z));
}


//create all the `instance` structs for the 3D scene, and add them to `gInstances`
static void CreateInstances() {

}

void wonGame()
{
	gameState = MENU;
	scoreTable.push_back(score);
	sort(scoreTable.begin(), scoreTable.end());
	std::reverse(scoreTable.begin(), scoreTable.end());
	Camera::getInstance().setPosition(glm::vec3(1100, 75, 0));
	Camera::getInstance().setNearAndFarPlanes(1.f, 500.f);
}

void startGame()
{
	gameState = GAME;
	Camera::getInstance().offsetPosition(model->pos - Camera::getInstance().position());
	Camera::getInstance().lookAt(glm::vec3(0, 1, 0));
	score = 0;
	playTime = 0;
	targetsKilled = 0;
	for (int i = 0, s = targets.size(); i < s; i++)
	{
		targets[i]->go->scale = glm::vec3(1, 1, 1);
		targets[i]->hit = false;
		targets[i]->alive = true;
	}
	Camera::getInstance().setNearAndFarPlanes(0.01f, 50.0f);
	Camera::getInstance().lookAt(glm::vec3(0, 0.75, 0));
}

void LoadTargets()
{
	//load in targets
	for (int i = 0; i < 6; i++)
	{
		Target* tar = new Target("models/Dummy.dae", 0.5);

		//last point needs to == first point

		if (i == 0)
		{
			tar->interp.points.push_back(glm::vec3(5, 0.4, 0));
			tar->interp.points.push_back(glm::vec3(6, 0.4, 1));
			tar->interp.points.push_back(glm::vec3(7, 0.4, 0));
			tar->interp.points.push_back(glm::vec3(6, 0.4, -1));
			tar->interp.points.push_back(glm::vec3(5, 0.4, 0));
			tar->interp.state = LINEAR;
		}
		if (i == 1)
		{
			tar->interp.points.push_back(glm::vec3(0, 0.4, 12));
			tar->interp.points.push_back(glm::vec3(0, 0.4, 9));
			tar->interp.points.push_back(glm::vec3(-1, 0.4, 9));
			tar->interp.points.push_back(glm::vec3(1, 0.4, 9));
			tar->interp.points.push_back(glm::vec3(0, 0.4, 9));
			tar->interp.points.push_back(glm::vec3(0, 0.4, 12));
			tar->interp.state = CATMULLROM;
		}
		if (i == 2)
		{
			tar->interp.points.push_back(glm::vec3(-3, 0.4, 12));
			tar->interp.points.push_back(glm::vec3(-3, 0.4, 8));
			tar->interp.points.push_back(glm::vec3(-2, 0.4, 8));
			tar->interp.points.push_back(glm::vec3(-4, 0.4, 8));
			tar->interp.points.push_back(glm::vec3(-3, 0.4, 8));
			tar->interp.points.push_back(glm::vec3(-3, 0.4, 12));
			tar->interp.state = CATMULLROM;
		}
		if (i == 3)
		{
			tar->interp.points.push_back(glm::vec3(-6, 0.4, 12));
			tar->interp.points.push_back(glm::vec3(-6, 0.4, 8));
			tar->interp.points.push_back(glm::vec3(-6, 0.4, 12));
			tar->interp.state = LINEAR;
		}
		if (i == 4)
		{
			tar->interp.points.push_back(glm::vec3(3, 0.4, 12));
			tar->interp.points.push_back(glm::vec3(3, 0.4, 8));
			tar->interp.points.push_back(glm::vec3(3, 0.4, 12));
			tar->interp.state = LINEAR;
		}
		if (i == 5)
		{
			tar->interp.points.push_back(glm::vec3(0, 0.4, 0));
			tar->interp.points.push_back(glm::vec3(-4, 0.4, 1));
			tar->interp.points.push_back(glm::vec3(-4, 0.4, -1));
			tar->interp.points.push_back(glm::vec3(0, 0.4, 0));
			tar->interp.state = LINEAR;
		}


		tar->interp.buildCurve();
		targets.push_back(tar);
	}
}

// draws a single frame
static void Render() {
	//fb->Bind();

    // clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	sky->render();
	ground->Render();
	animatedMechGC->render();
	for (unsigned int i = 0, s = goVec.size(); i < s; i++)
	{
		ComponentGraphics* cg = static_cast<ComponentGraphics*>(goVec[i]->GetComponent(GRAPHICS));
		cg->render();
		if (goVec[i]->HasComponent(PHYSICS))
		{
			ComponentCollision* cc = static_cast<ComponentCollision*>(goVec[i]->GetComponent(PHYSICS));
			//cc->renderHitbox();
		}
	}

	for (unsigned int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
	{
		ObjectManager::instance().pMap[i]->cg->render();
		//ObjectManager::instance().pMap[i]->cc->renderHitbox();
	}

	for (unsigned int i = 0, s = targets.size(); i < s; i++)
	{
		if (targets[i]->alive)
		{
			targets[i]->cg->render();
			//targets[i]->cc->renderHitbox();
		}
	}

	//gCol->renderHitbox();
	if (gameState == MENU)
	{
		startscreen->render();

		TextRendering::getInstance().printText2D("HIGHSCORES", -0.6f, -0.675f, 0.125f, fontColour);
		for (int i = 0, s = scoreTable.size(); i < s && i < 5; i++)
		{
			char buffer[64];
			_snprintf_s(buffer, 64, "SCORE:%i", scoreTable[i]);
			TextRendering::getInstance().printText2D(buffer, -0.38f, -0.75f - i / 16.f, 0.075f, fontColour);
		}
		if (score != 0)
		{
			char buffer[64];
			_snprintf_s(buffer, 64, "SCORE:%i", score);
			TextRendering::getInstance().printText2D(buffer, -0.38f, 0.85f, 0.075f, fontColour);
		}


	}
	else if (gameState == GAME)
	{
		crosshair->render();
		skull->render();

		char buffer[5];
		_snprintf_s(buffer, 5, "%i/%i", targetsKilled, targets.size());
		TextRendering::getInstance().printText2D(buffer, -0.70f, -0.8f, 0.125f, fontColour);
		char scbuff[64];
		_snprintf_s(scbuff, 64, "SCORE:%i", score);
		TextRendering::getInstance().printText2D(scbuff, -0.38f, 0.85f, 0.075f, fontColour);
	}

	//_snprintf_s(buffer, 5, "%i", score);
    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(gWindow);
}

#define SHOT_CD 0.1
float shotcd = 0;
// update the scene based on the time elapsed since last update
static void Update(float secondsElapsed) {
	runTime += secondsElapsed;

	glm::vec3 lInput;
	glm::vec2 rInput;
	Camera* cam = &Camera::getInstance();
	glm::vec3 f = cam->forward();
	glm::vec3 r = cam->right();
	glm::vec3 fmy = cam->forward();
	fmy.y = 0;
	bool shoot = false;

	ComponentInput* c = static_cast<ComponentInput*>(model->GetComponent(CONTROLLER));
	if (c->Refresh())
	{
		lInput = glm::vec3(c->leftStickX, 0, c->leftStickY);
		rInput = glm::vec2(c->rightStickX / 5, c->rightStickY / 5);
		if (c->rightTrigger > 0.5 && shotcd > SHOT_CD)
			shoot = true;
	}
	else
	{
		const float moveSpeed = 1.f; //units per second
		if (glfwGetKey(gWindow, 'S'))
			lInput.z = -1;
		else if (glfwGetKey(gWindow, 'W'))
			lInput.z = 1;
		if (glfwGetKey(gWindow, 'A'))
			lInput.x = -1;
		else if (glfwGetKey(gWindow, 'D'))
			lInput.x = 1;

		if (glfwGetKey(gWindow, ' '))
		{
			shoot = true;
		}

		//rotate camera based on mouse movement
		const float mouseSensitivity = 0.05f;
		double mouseX, mouseY;
		glfwGetCursorPos(gWindow, &mouseX, &mouseY);
		//Camera::getInstance().offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);
		glfwSetCursorPos(gWindow, 0, 0);
		rInput.x = mouseX * mouseSensitivity;
		rInput.y = -mouseY * mouseSensitivity;
	}


	if (gameState == GAME)
	{
		//Begin Camera code
		model->pos += fmy * (c->getOwner()->vel * lInput.z);
		model->pos += cam->right() * (c->getOwner()->vel * lInput.x);

		cam->offsetPosition(model->pos - cam->position());

		c->getOwner()->dir = glm::rotateX(c->getOwner()->dir, -rInput.y);
		c->getOwner()->dir = glm::rotateY(c->getOwner()->dir, rInput.x);
		cam->offsetOrientation(-rInput.y, rInput.x);
		//End Camera code

		for (int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
			ObjectManager::instance().pMap[i]->update(secondsElapsed);

		glm::vec3 p = Camera::getInstance().position();
		if (shoot && shotcd > SHOT_CD)
		{
			Projectile* pr = new Projectile(p, f, 0.5, 100, 10);
			ObjectManager::instance().pMap.push_back(pr);
			shotcd = 0;
		}


		CollisionManager::instance().checkAll();

		ObjectManager::instance().updateProjectile(secondsElapsed);

		for (int i = 0, s = targets.size(); i < s; i++)
		{
			targets[i]->update(secondsElapsed/10);
			if (targets[i]->hit && targets[i]->alive)
			{
				targets[i]->alive = false;
				skull->play();
				targetsKilled++;
			}
		}

		skull->update(secondsElapsed);

		model->pos.y = 0.5;
		//model->pos.y = ground->HeightAtLocation(model->pos);

		playTime += secondsElapsed;
		if (score >= 0)
		{
			float gameTime = playTime / 1000;
			score = ((gameTime * 100) / pow(gameTime, 2)) * 100 - 15;
		}
		else
			score == 0;

		if (targetsKilled == targets.size() || c->IsPressed(XINPUT_GAMEPAD_BACK))
			wonGame();


		//cam->lookAt(glm::vec3(7.5, 0, -11));
	}
	else if (gameState == MENU)
	{
		//camInterp.speedControlInterp(secondsElapsed/40);
		//cam->setPosition(camInterp.pos);
		cam->lookAt(glm::vec3(1000, 0, 0));
		//camInterp.speedControlInterp(secondsElapsed/40);
		//cam->setPosition(camInterp.pos);
		cam->offsetPosition(cam->right() * 0.1f);

		if (c->Refresh())
		{
			if (c->IsPressed(XINPUT_GAMEPAD_START))
			{
				startGame();
			}
		}
		else
		{
			if (glfwGetKey(gWindow, ' '))
			{
				startGame();
			}
		}
	}
	std::vector<glm::mat4> trans;
	shotcd += secondsElapsed;
	tElap += secondsElapsed;
	//Will need to uncomment the following and have gModel relate to the mech's graphics
	animatedMechGC->BoneTransform(tElap, trans);
}

// records how far the y axis has been scrolled
void OnScroll(GLFWwindow* window, double deltaX, double deltaY) {
    gScrollY += deltaY;
}

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

// the program starts here
void AppMain() {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "Mek", NULL /*glfwGetPrimaryMonitor()*/, NULL);
	if (!gWindow)
		throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");

    // GLFW settings
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, 0, 0);
    glfwSetScrollCallback(gWindow, OnScroll);
    glfwMakeContextCurrent(gWindow);

	// required or we crash on VAO creation
	glewExperimental = GL_TRUE;
    // initialise GLEW
	if (glewInit() != GLEW_OK)
	{
        throw std::runtime_error("glewInit failed");
	}

    // GLEW throws some errors so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {}

	// Init DevIL
	ilInit();

	// enable vsync using windows only code
#ifdef _WIN32
	// Turn on vertical screen sync under Windows.
	typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(1);
#endif

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_3)
        throw std::runtime_error("OpenGL 3.3 API is not available.");

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialise the gWoodenCrate asset
    LoadWoodenCrateAsset();

    // create all the instances in the 3D scene based on the gWoodenCrate asset
    CreateInstances();

	//fb = new Framebuffer();
	//fb->CreateDepthTexture(1920, 1080);
	//fb->CreateColorTexture(1920, 1080);

    // setup Camera::getInstance()
    Camera::getInstance().setPosition(glm::vec3(1100, 75, 0));
    Camera::getInstance().setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
	Camera::getInstance().setNearAndFarPlanes(1.f, 1024.0f);
	Camera::getInstance().setFieldOfView(50);

	crosshair = new twodOverlay("crosshair.png", 0, 0, 1);
	skull = new twodOverlayAnim("killSkull.png", 5, 0.5);
	startscreen = new twodOverlay("pressStart.png", 0, 0, 10);
	skull->updatePos(-0.85f, -0.75f, 4);
	skull->cycle = true;

	ground = new Terrain();
	ground->LoadHeightMap("heightmap2.png",1);
	ground->InitRender();
	char* sb[6] = { "ri.png", "le.png", "to.png", "bo.png", "ba.png", "fr.png" };
	sky = new Skybox(sb);
	//MODEL INITS

	prepProjectiles();

	model = new GameObject(0);
	model->SetName("Moving");
	gModel = new ComponentGraphics();
	gModel->setOwner(model);
	gModel->loadModel("models/TallCube.dae");
	Component* gp = gModel;
	model->AddComponent(GRAPHICS, gp);
	gCol = new ComponentCollision();
	gCol->setCollisionMask(gModel->getScene());
	gCol->setOwner(model);
	model->pos = glm::vec3(7.5, 0.5, -11);
	model->vel = 0.1;
	model->dir = glm::vec3(1, 0, 0);
	model->scale = glm::vec3(5, 5, 5);
	gCol->type = MOVING;
	gCol->createHitboxRender();
	gp = gCol;
	model->AddComponent(PHYSICS, gp);
	ComponentInput* ci = new ComponentInput(0.05,0.05);
	gp = ci;
	model->AddComponent(CONTROLLER, gp);
	
	//PROPER INIT
	for (int i = 0; i < 22; i++)
	{
		if (i != 3 && i != 0 && i != 4 && i != 8 && i != 18 && i != 19 && i != 20 && i !=21)
		{
			GameObject *gObject = new GameObject(goVec.size());
			ComponentGraphics *cModel = new ComponentGraphics();
			ComponentCollision *cCollision = new ComponentCollision();
			Component *c;

			if (i == 0)
			{
				gObject->SetName("Spawn Container 1");
				cModel->loadModel("models/Container.dae");

				gObject->scale = glm::vec3(0.7, 0.7, 0.7);
				gObject->pos = glm::vec3(60, 0, -110);
			}
			else if (i == 1)
			{
				gObject->SetName("Water Tower");
				cModel->loadModel("models/Watertower.dae");

				gObject->scale = glm::vec3(3, 3, 3);
				gObject->pos = glm::vec3(-65, 0, -90);
			}
			else if (i == 2)
			{
				gObject->SetName("MenuScene");
				cModel->loadModel("models/Warehouse_One_mesh_No_roof.dae");

				gObject->scale = glm::vec3(1, 1, 1);// glm::vec3(1.6, 1.6, 1.6);
				gObject->pos = glm::vec3(10000, 0, 0);
			}
			else if (i == 3)
			{
				gObject->SetName("Spawn Container 2");
				cModel->loadModel("models/Container90.dae");

				gObject->scale = glm::vec3(0.7, 0.7, 0.7);
				gObject->pos = glm::vec3(85, 0, -75);
			}
			else if (i == 4)
			{
				gObject->SetName("Middle Plus");
				cModel->loadModel("models/Container.dae");

				gObject->scale = glm::vec3(0.7, 0.7, 0.7);
				gObject->pos = glm::vec3(15, 0, -20);
			}
			else if (i == 5)
			{
				gObject->SetName("North Wall");
				cModel->loadModel("models/Container_Wal_LPl.dae");

				gObject->scale = glm::vec3(0.7, 0.70, 0.70);
				gObject->pos = glm::vec3(100, 0, 165);
			}
			else if (i == 6)
			{
				gObject->SetName("Dumbster");//Crane
				cModel->loadModel("models/Dumspter2.dae");
				gObject->pos = glm::vec3(0, 0, -140);
				gObject->scale = glm::vec3(0.4, 0.4, 0.4);
			}
			else if (i == 7)
			{
				gObject->SetName("Shack");
				cModel->loadModel("models/Shack.dae");

				gObject->scale = glm::vec3(0.75, 0.75, 0.75);
				gObject->pos = glm::vec3(0, 0, 120);
			}
			else if (i == 8)
			{
				gObject->SetName("Middle Plus");
				cModel->loadModel("models/Container.dae");

				gObject->scale = glm::vec3(0.7, 0.7, 0.7);
				gObject->pos = glm::vec3(-5, 0, -20);
			}
			else if (i == 9)
			{
				gObject->SetName("Container 2");
				cModel->loadModel("models/Container.dae");

				gObject->scale = glm::vec3(0.70, 0.70, 0.70);
				gObject->pos = glm::vec3(80, 0, 100);
			}
			else if (i == 10)
			{
				gObject->SetName("South Wall");
				cModel->loadModel("models/Container_Wal_LPl.dae");

				gObject->scale = glm::vec3(0.7, 0.70, 0.70);
				gObject->pos = glm::vec3(-100, 0, 165);
			}
			else if (i == 11)
			{
				gObject->SetName("East Wall");
				cModel->loadModel("models/Container_Wal_LP90.dae");

				gObject->scale = glm::vec3(0.7, 0.70, 0.70);
				gObject->pos = glm::vec3(50, 0, 145);
			}
			else if (i == 12)
			{
				gObject->SetName("West Wall");
				cModel->loadModel("models/Container_Wal_LP90.dae");

				gObject->scale = glm::vec3(0.7, 0.70, 0.70);
				gObject->pos = glm::vec3(50, 0, -125);
			}
			else if (i == 13)
			{
				gObject->SetName("Container 2");
				cModel->loadModel("models/Container.dae");

				gObject->scale = glm::vec3(0.70, 0.70, 0.70);
				gObject->pos = glm::vec3(60, 0, 100);
			}
			else if (i == 14)
			{
				gObject->SetName("Container 90");
				cModel->loadModel("models/Container90.dae");

				gObject->scale = glm::vec3(0.70, 0.70, 0.70);
				gObject->pos = glm::vec3(70, 0, 70);
			}
			else if (i == 15)
			{
				gObject->SetName("Shack");
				cModel->loadModel("models/Shack.dae");

				gObject->scale = glm::vec3(0.75, 0.75, 0.75);
				gObject->pos = glm::vec3(-30, 0, 120);
			}
			else if (i == 16)
			{
				gObject->SetName("Shack");
				cModel->loadModel("models/Shack.dae");

				gObject->scale = glm::vec3(0.75, 0.75, 0.75);
				gObject->pos = glm::vec3(30, 0, 120);
			}
			else if (i == 17)
			{
				gObject->SetName("Shack");
				cModel->loadModel("models/Shack.dae");

				gObject->scale = glm::vec3(0.75, 0.75, 0.75);
				gObject->pos = glm::vec3(-60, 0, 120);
			}
			else if (i == 18)
			{
				gObject->SetName("Middle Plus North");
				cModel->loadModel("models/Container90.dae");

				gObject->scale = glm::vec3(0.7, 0.7, 0.7);
				gObject->pos = glm::vec3(27, 0, -5);
			}
			else if (i == 19)
			{
				gObject->SetName("Middle Plus North");
				cModel->loadModel("models/Container90.dae");

				gObject->scale = glm::vec3(0.7, 0.7, 0.7);
				gObject->pos = glm::vec3(27, 0, 15);
			}
			else if (i == 20)
			{
				gObject->SetName("Middle Plus North");
				cModel->loadModel("models/Container90.dae");

				gObject->scale = glm::vec3(0.7, 0.7, 0.7);
				gObject->pos = glm::vec3(-20, 0, 15);
			}
			else if (i == 21)
			{
				gObject->SetName("Middle Plus North");
				cModel->loadModel("models/Container90.dae");

				gObject->scale = glm::vec3(0.7, 0.7, 0.7);
				gObject->pos = glm::vec3(-20, 0, -5);
			}

			gObject->pos /= 10.f;

			cModel->setOwner(gObject);
			c = cModel;
			gObject->AddComponent(GRAPHICS, c);
			cCollision->setOwner(gObject);
			cCollision->setCollisionMask(cModel->getScene());
			cCollision->type = STATIC;
			cCollision->setCollisionElip(glm::vec3(1, 1, 1));
			cCollision->createHitboxRender();
			gObject->AddComponent(PHYSICS, cCollision);
			goVec.push_back(gObject);
		}
	}

	LoadTargets();
	

	spotLightColour = glm::normalize(spotLightColour);
	for (int i = 0; i < 6; i++)
	{
		LightComponent* light;
		if (i == 0)
		{
			light = new LightComponent(lSPOT);
			SpotLight* lc = new SpotLight;
			lc->Base.Base.Color = spotLightColour;
			lc->Base.Base.AmbientIntensity = 0.1f;
			lc->Base.Base.DiffuseIntensity = 0.1f;
			
			lc->Base.Atten.Constant = 0.1f;
			lc->Base.Atten.Exp = 0.1f;
			lc->Base.Atten.Linear = 0.1f;
			
			lc->Cutoff = 0.75f;
			lc->Base.Position = glm::vec3(-6, 1, 11);
			lc->Direction = glm::vec3(0, 0, -1);
			
			light->SetVars(lSPOT, lc);
		}
		if (i == 1)
		{
			light = new LightComponent(lSPOT);
			SpotLight* lc = new SpotLight;
			lc->Base.Base.Color = spotLightColour;
			lc->Base.Base.AmbientIntensity = 0.5f;
			lc->Base.Base.DiffuseIntensity = 0.5f;
			
			lc->Base.Atten.Constant = 0.5f;
			lc->Base.Atten.Exp = 0.5f;
			lc->Base.Atten.Linear = 0.5f;
			
			lc->Cutoff = 0.75f;
			lc->Base.Position = glm::vec3(3, 1, 11);
			lc->Direction = (glm::vec3(0, 0, 10));
			
			light->SetVars(lSPOT, lc);
		}
		if (i == 2)
		{
			//light = new LightComponent(lSPOT);
			//SpotLight* lc = new SpotLight;
			//lc->Base.Base.Color = glm::vec3(0,0.1,0);
			//lc->Base.Base.AmbientIntensity = 0.5f;
			//lc->Base.Base.DiffuseIntensity = 0.5f;
			//
			//lc->Base.Atten.Constant = 0.5f;
			//lc->Base.Atten.Exp = 0.5f;
			//lc->Base.Atten.Linear = 0.5f;
			//
			//lc->Cutoff = 0.75f;
			//lc->Base.Position = glm::vec3(-3, 1, 11);
			//lc->Direction = (glm::vec3(-3, 0, 12));
			//
			//light->SetVars(lSPOT, lc);
		}
		if (i == 3)
		{
			//light = new LightComponent(lSPOT);
			//SpotLight* lc = new SpotLight;
			//lc->Base.Base.Color = spotLightColour;
			//lc->Base.Base.AmbientIntensity = 0.5f;
			//lc->Base.Base.DiffuseIntensity = 0.5f;
			//
			//lc->Base.Atten.Constant = 0.5f;
			//lc->Base.Atten.Exp = 0.5f;
			//lc->Base.Atten.Linear = 0.5f;
			//
			//lc->Cutoff = 0.75f;
			//lc->Base.Position = glm::vec3(-6, 1, 11);
			//lc->Direction = (glm::vec3(-6, 1, 12));
			//
			//light->SetVars(lSPOT, lc);
		}
		if (i == 4)
		{
			//light = new LightComponent(lSPOT);
			//SpotLight* lc = new SpotLight;
			//lc->Base.Base.Color = spotLightColour;
			//lc->Base.Base.AmbientIntensity = 0.1f;
			//lc->Base.Base.DiffuseIntensity = 0.1f;
			//
			//lc->Base.Atten.Constant = 0.1f;
			//lc->Base.Atten.Exp = 0.1f;
			//lc->Base.Atten.Linear = 0.1f;
			//
			//lc->Cutoff = 0.75f;
			//lc->Base.Position = glm::vec3(0, 1, 0);
			//lc->Direction = glm::vec3(0, -1, 0);
			//
			//light->SetVars(lSPOT, lc);
		}
		if (i == 5)
		{
			light = new LightComponent(lSPOT);
			SpotLight* lc = new SpotLight;
			lc->Base.Base.Color = spotLightColour;
			lc->Base.Base.AmbientIntensity = 0.5f;
			lc->Base.Base.DiffuseIntensity = 0.5f;

			lc->Base.Atten.Constant = 0.5f;
			lc->Base.Atten.Exp = 0.5f;
			lc->Base.Atten.Linear = 0.5f;

			lc->Cutoff = 0.5f;
			lc->Base.Position = glm::vec3(1000, 1, 0);//4 1 0
			lc->Direction = glm::vec3(0, -1, 0);// 5 0 0

			light->SetVars(lSPOT, lc);
		}
	}

	animatedMech = new GameObject(100);
	animatedMechGC = new ComponentGraphics();
	animatedMechGC->loadModel("models/Test_Animation_DAE.dae");
	Component* c = animatedMechGC;
	animatedMech->AddComponent(GRAPHICS, c);
	animatedMech->pos = glm::vec3(0, 0, 0);
	animatedMech->scale = glm::vec3(1, 1, 1);

	//END MODEL INITS
	camInterp.points.push_back(glm::vec3(1025, 1, 0));
	camInterp.points.push_back(glm::vec3(1000, 1, 25));
	camInterp.points.push_back(glm::vec3(975, 1, 0));
	camInterp.points.push_back(glm::vec3(1000, 1, -25));
	camInterp.points.push_back(glm::vec3(1025, 1, 0));
	camInterp.state = SLERP;
	camInterp.buildCurve();

	TextRendering::getInstance().initText2D("MekFont.bmp");
	fontColour = glm::normalize(fontColour);

	wglSwapIntervalEXT(1);


    // run while the window is open
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();

        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        Update((float)(thisTime - lastTime));
        lastTime = thisTime;

        // draw one frame
        Render();

        // check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL Error " << error << " - " << glewGetErrorString(error) << std::endl;
		}

        //exit program if escape key is pressed
        if(glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }

    // clean up and exit
    glfwTerminate();
}


int main(int argc, char *argv[]) {
    try {
        AppMain();
    } catch (const std::exception& e){
		std::cerr << "ERROR: " << e.what() << std::endl;
	    return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
