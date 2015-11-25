// third-party libraries
#include "include\GL\glew.h"
#include "include\GL\glfw3.h"
#include "lib\glm\glm.hpp"
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

// classes
#include "Program.h"
#include "ComponentLight.h"
#include "Texture.h"
#include "Camera.h"
//#include "ComponentGraphics.h"
//#include "ComponentCollision.h"
#include "ComponentInput.h"

#include "2dOverlayAnim.h"
#include "Target.h"
#include "Projectile.h"


//Bad Inits need to fix at a later time
//Pls no kill future me.  I sorry
float runTime = 0;
std::vector<GameObject*> goVec;
std::vector<Target*> targets;
int targetsKilled = 0;
twodOverlay* crosshair;
twodOverlayAnim* skull;

//TODO : World/Target Loading, Menu, Timer, Target Counter

void LoadShaders(char* vertFilename, char* fragFilename) 
{
	Program::getInstance().createShader("standard", GL_VERTEX_SHADER, vertFilename);
	Program::getInstance().createShader("standard", GL_FRAGMENT_SHADER, fragFilename);

	// load skinning shaders
	Program::getInstance().createShader("skinning", GL_VERTEX_SHADER, "skinning.vert");
	Program::getInstance().createShader("skinning", GL_FRAGMENT_SHADER, "skinning.frag");
	Program::getInstance().createShader("hud", GL_VERTEX_SHADER, "hud.vert");
	Program::getInstance().createShader("hud", GL_FRAGMENT_SHADER, "hud.frag");
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

GameObject* tmodel;
ComponentGraphics* tModel;
ComponentCollision* tCol;
GameObject* qtmodel;
ComponentGraphics* qtModel;
ComponentCollision* qtCol;

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
    LoadShaders("vertex-shader.vert", "fragment-shader.frag");
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

void LoadWorld()
{
	//GameObject* ob;
	//ComponentGraphics* g;
	//ComponentCollision* c;
	//
	////load vars from a txt/bin file
	//
	////to add it in
	//ObjectManager::instance().addObject(ob);
}

// draws a single frame
static void Render() {
    // clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//tModel->render();
	//qtModel->render();
	//gModel->render();
	
	for (unsigned int i = 0, s = goVec.size(); i < s; i++)
	{
		ComponentGraphics* cg = static_cast<ComponentGraphics*>(goVec[i]->GetComponent(GRAPHICS));
		cg->render();
		if (goVec[i]->HasComponent(PHYSICS))
		{
			ComponentCollision* cc = static_cast<ComponentCollision*>(goVec[i]->GetComponent(PHYSICS));
			cc->renderHitbox();
		}
	}

	for (unsigned int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
	{
		ObjectManager::instance().pMap[i]->cg->render();
		ObjectManager::instance().pMap[i]->cc->renderHitbox();
	}

	for (unsigned int i = 0, s = targets.size(); i < s; i++)
	{
		if (targets[i]->alive)
			targets[i]->cg->render();
	}

	gCol->renderHitbox();
	crosshair->render();
	skull->render();
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
		const float moveSpeed = 0.2f; //units per second
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
		const float mouseSensitivity = 0.005f;
		double mouseX, mouseY;
		glfwGetCursorPos(gWindow, &mouseX, &mouseY);
		//Camera::getInstance().offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);
		glfwSetCursorPos(gWindow, 0, 0);
		rInput.x = mouseX * mouseSensitivity;
		rInput.y = mouseY * mouseSensitivity;
	}

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
		if (targets[i]->hit && targets[i]->alive)
		{
			targets[i]->alive = false;
			skull->play();
			targetsKilled++;
		}
	}

	skull->update(secondsElapsed);

	model->pos.y = 1;

	std::vector<glm::mat4> trans;
	shotcd += secondsElapsed;
	tElap += secondsElapsed;
	//gModel->BoneTransform(tElap, trans);
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
    if(!gWindow)
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

    // setup Camera::getInstance()
    Camera::getInstance().setPosition(glm::vec3(0, 0, 0));
    Camera::getInstance().setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
	Camera::getInstance().setNearAndFarPlanes(0.1f, 500.0f);
	Camera::getInstance().setFieldOfView(179);

	crosshair = new twodOverlay("../Debug/crosshair.png", 0, 0, 1);
	skull = new twodOverlayAnim("../Debug/killSkull.png", 5, 0.5);
	skull->updatePos(-0.85f, -0.75f, 4);
	skull ->cycle = true;
	//MODEL INITS

	prepProjectiles();

	model = new GameObject(0);
	model->SetName("Moving");
	gModel = new ComponentGraphics();
	gModel->setOwner(model);
	gModel->loadModel("../Debug/models/TallCube.dae");
	Component* gp = gModel;
	model->AddComponent(GRAPHICS, gp);
	gCol = new ComponentCollision();
	gCol->setCollisionMask(gModel->getScene());
	gCol->setOwner(model);
	model->pos = glm::vec3(0, 1, 25);
	model->vel = 0.1;
	model->dir = glm::vec3(1, 0, 0);
	model->scale = glm::vec3(20, 20, 20);
	gCol->type = MOVING;
	gCol->createHitboxRender();
	gp = gCol;
	model->AddComponent(PHYSICS, gp);
	ComponentInput* ci = new ComponentInput(0.05,0.05);
	gp = ci;
	model->AddComponent(CONTROLLER, gp);
	
	//PROPER INIT
	for (int i = 0; i < 10; i++)
	{
		GameObject *gObject = new GameObject(goVec.size());
		ComponentGraphics *cModel = new ComponentGraphics();
		ComponentCollision *cCollision = new ComponentCollision();
		Component *c;

		if (i == 0)
		{
			gObject->SetName("Mek");
			cModel->loadModel("../Debug/models/Dumpster.dae");
			gObject->pos = glm::vec3(5, 0, 5);
		}
		else if (i == 1)
		{
			gObject->SetName("Water Tower");
			cModel->loadModel("../Debug/models/Watertower.dae");
			
			gObject->scale = glm::vec3(3, 3, 3);
			gObject->pos = glm::vec3(-4, 0, 4);
		}
		else if (i == 2)
		{
			gObject->SetName("Building");
			cModel->loadModel("../Debug/models/Dumpster.dae");
			
			gObject->scale = glm::vec3(1, 1, 1);// glm::vec3(1.6, 1.6, 1.6);
			gObject->pos = glm::vec3(8, 0, 3);
		}
		else if (i == 3)
		{
			gObject->SetName("Mech Dumpster");
			cModel->loadModel("../Debug/models/Dumpster.dae");
			gObject->pos = glm::vec3(0, 0, 0);
			gObject->scale = glm::vec3(1, 1, 1);
		}
		else if (i == 4)
		{
			gObject->SetName("Dumpster");
			cModel->loadModel("../Debug/models/Dumpster.dae");
			
			gObject->scale = glm::vec3(0.50, 0.50, 0.50);
			gObject->pos = glm::vec3(3, 0, -3);
		}
		else if (i == 5)
		{
			gObject->SetName("Container");
			cModel->loadModel("../Debug/models/Container.dae");
			
			gObject->scale = glm::vec3(0.7, 0.70, 0.70);
			gObject->pos = glm::vec3(12, 0, 8);
		}
		else if (i == 6)
		{
			gObject->SetName("Target");//Crane
			cModel->loadModel("../Debug/models/Dumpster.dae");
			gObject->pos = glm::vec3(0, 0, -15);
			gObject->scale = glm::vec3(3, 3, 3);
		}
		else if (i == 7)
		{
			gObject->SetName("Shack");
			cModel->loadModel("../Debug/models/Shack.dae");
			
			gObject->scale = glm::vec3(0.50, 0.50, 0.50);
			gObject->pos = glm::vec3(-6, 0, -6);
		}
		else if (i == 8)
		{
			gObject->SetName("2 boxes");
			cModel->loadModel("../Debug/models/Dumpster.dae");
			gObject->pos = glm::vec3(0, 0, 1);
		//	gObject->scale = glm::vec3(10, 10, 10);
		}
		else if (i == 9)
		{
			gObject->SetName("Container 2");
			cModel->loadModel("../Debug/models/Container.dae");
			
			gObject->scale = glm::vec3(0.70, 0.70, 0.70);
			gObject->pos = glm::vec3(8, 0, 10);
		}

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

	LightComponent* light = new LightComponent(lPOINT);
	PointLight* lc = new PointLight;
	lc->Atten.Constant = 1;
	lc->Atten.Exp = 1;
	lc->Atten.Linear = 1;
	lc->Base.AmbientIntensity = 1;
	lc->Base.Color = glm::vec3(1, 1, 1);
	lc->Base.DiffuseIntensity = 1;
	lc->Position = glm::vec3(0, 0, 100);

	light->SetVars(lPOINT, lc);
	//END MODEL INITS

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
