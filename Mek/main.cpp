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


#include "Projectile.h"


//BEGIN THE SHITTIEST CODE I HAVE EVER DONE
//Pls no kill future me.  I sorry
std::vector<GameObject*> goVec;
ComponentInput* cInput;


//TODO : SLERP (Targets?), Sprite animation, Paths and Curves(Targets?) 

void LoadShaders(char* vertFilename, char* fragFilename) 
{
	Program::getInstance().createShader("standard", GL_VERTEX_SHADER, vertFilename);
	Program::getInstance().createShader("standard", GL_FRAGMENT_SHADER, fragFilename);

	// load skinning shaders
	Program::getInstance().createShader("skinning", GL_VERTEX_SHADER, "skinning.vert");
	Program::getInstance().createShader("skinning", GL_FRAGMENT_SHADER, "skinning.frag");
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
	
	tModel->render();
	//qtModel->render();
	//gModel->render();
	
	for (unsigned int i = 0, s = goVec.size(); i < s; i++)
	{
		ComponentGraphics* cg = static_cast<ComponentGraphics*>(goVec[i]->GetComponent(GRAPHICS));
		cg->render();
	}

    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(gWindow);
}


// update the scene based on the time elapsed since last update
static void Update(float secondsElapsed) {

    //move position of camera based on WASD keys, and XZ keys for up and down
    const float moveSpeed = 0.2f; //units per second
    if(glfwGetKey(gWindow, 'S')){
        Camera::getInstance().offsetPosition(secondsElapsed * moveSpeed * 500.f * -Camera::getInstance().forward());
    } else if(glfwGetKey(gWindow, 'W')){
        Camera::getInstance().offsetPosition(secondsElapsed * moveSpeed * 500.f * Camera::getInstance().forward());
    }
    if(glfwGetKey(gWindow, 'A')){
        Camera::getInstance().offsetPosition(secondsElapsed * moveSpeed * 500.f * -Camera::getInstance().right());
    } else if(glfwGetKey(gWindow, 'D')){
		Camera::getInstance().offsetPosition(secondsElapsed * moveSpeed * 500.f * Camera::getInstance().right());
    }
    if(glfwGetKey(gWindow, 'Z')){
		Camera::getInstance().offsetPosition(secondsElapsed * moveSpeed * 500.f * -glm::vec3(0, 1, 0));
    } else if(glfwGetKey(gWindow, 'X')){
		Camera::getInstance().offsetPosition(secondsElapsed * moveSpeed * 500.f * glm::vec3(0, 1, 0));
    }
	
	if (cInput->Refresh())
	{
		//Camera::getInstance().offsetPosition(cInput->leftStickY * moveSpeed * 10.0f * Camera::getInstance().forward());
		//Camera::getInstance().offsetPosition(cInput->leftStickX * moveSpeed * Camera::getInstance().right());

		//Camera::getInstance().offsetOrientation(-cInput->rightStickY * 0.5f, cInput->rightStickX * 0.5f);
	}


    //rotate camera based on mouse movement
    const float mouseSensitivity = 0.005f;
    double mouseX, mouseY;
    glfwGetCursorPos(gWindow, &mouseX, &mouseY);
    Camera::getInstance().offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);
    glfwSetCursorPos(gWindow, 0, 0); //reset the mouse, so it doesn't go out of the window

    //increase or decrease field of view based on mouse wheel
    const float zoomSensitivity = -20.2f;
    //float fieldOfView = Camera::getInstance().fieldOfView() + zoomSensitivity * (float)gScrollY;
    //if(fieldOfView < 5.0f) fieldOfView = 5.0f;
    //if(fieldOfView > 130.0f) fieldOfView = 130.0f;
	//
	//printf("FOV : %f \n", fieldOfView);

    Camera::getInstance().setFieldOfView(179);
    gScrollY = 0;


	//Theory:
	//Camera pos always = object pos
	//Camera dir always = object dir

	Camera* cam = &Camera::getInstance();

	ComponentInput* c = static_cast<ComponentInput*>(model->GetComponent(CONTROLLER));
	c->Refresh();
	glm::vec3 lInput = glm::vec3(c->leftStickX, 0, c->leftStickY);
	glm::vec2 rInput = glm::vec2(c->rightStickX / 5, c->rightStickY / 5);
	
	//c->getOwner()->pos += cam->forward() * (c->getOwner()->vel * -lInput.z);
	//c->getOwner()->pos += cam->right() * (c->getOwner()->vel * -lInput.x);
	glm::vec3 fmy = cam->forward();
	fmy.y = 0;

	model->pos += fmy * (c->getOwner()->vel * lInput.z);
	model->pos += cam->right() * (c->getOwner()->vel * lInput.x);

	cam->offsetPosition(model->pos - cam->position());

	c->getOwner()->dir = glm::rotateX(c->getOwner()->dir, -rInput.y);
	c->getOwner()->dir = glm::rotateY(c->getOwner()->dir, rInput.x);
	cam->offsetOrientation(-rInput.y, rInput.x);

	glm::vec3 f = cam->forward();
	glm::vec3 r = cam->right();
	
	for (int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
		ObjectManager::instance().pMap[i]->update(secondsElapsed);

	glm::vec3 p = Camera::getInstance().position();
	if (c->rightTrigger > 0.5)
	{
		Projectile* pr = new Projectile(p, f, 0.5, 100, 100);
		ObjectManager::instance().pMap.push_back(pr);
	}

	CollisionManager::instance().checkAll();

	//if (model->pos != p)
	//	Camera::getInstance().getInstance().offsetPosition(p - model->pos);

	std::vector<glm::mat4> trans;
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "Mek", NULL /*glfwGetPrimaryMonitor()*/, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 4.5?");

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

    // GLEW throws some errors, so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {}

	// Init DevIL
	ilInit();
	//iluInit();
	//ilutRenderer(ILUT_OPENGL);

	// enable vsync using windows only code
#ifdef _WIN32
	// Turn on vertical screen sync under Windows.
	// (I.e. it uses the WGL_EXT_swap_control extension)
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
    if(!GLEW_VERSION_4_5)
        throw std::runtime_error("OpenGL 4.5 API is not available.");

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



	//MODEL INITS

	prepProjectiles();

	model = new GameObject(0);
	model->SetName("Moving");
	gModel = new ComponentGraphics();
	gModel->setOwner(model);
	gModel->loadModel("../Debug/models/2boxes.dae");
	Component* gp = gModel;
	model->AddComponent(GRAPHICS, gp);
	gCol = new ComponentCollision();
	gCol->setCollisionMask(gModel->getScene());
	gCol->setOwner(model);
	model->pos = glm::vec3(0, 1, 0);
	model->vel = 0.1;
	model->dir = glm::vec3(1, 0, 0);
	gCol->type = MOVING;
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
			cModel->loadModel("../Debug/models/2boxes.dae");
			gObject->pos = glm::vec3(0, 0, -5);
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
			cModel->loadModel("../Debug/models/Building.dae");
			
			gObject->scale = glm::vec3(1.6, 1.6, 1.6);
			gObject->pos = glm::vec3(8, 0, 3);
		}
		else if (i == 3)
		{
		//	gObject->SetName("Mech");
		//	cModel->loadModel("../Debug/models/Mech.dae");
		//	gObject->pos = glm::vec3(0, 0, 0);
		//	gObject->scale = glm::vec3(10, 10, 10);
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
		//else if (i == 6)
		//{
		//	gObject->SetName("Crane");
		//	cModel->loadModel("../Debug/models/Crane.dae");
		//	gObject->pos = glm::vec3(0, 0, -15);
		//	gObject->scale = glm::vec3(3, 3, 3);
		//}
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
			cModel->loadModel("../Debug/models/2boxes.dae");
			gObject->pos = glm::vec3(0, 0, 0);
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
		//cCollision->setOwner(gObject);
		//cCollision->setCollisionMask(cModel->getScene());
		//cCollision->type = STATIC;
		//cCollision->setCollisionElip(glm::vec3(1, 1, 1));
		//gObject->AddComponent(PHYSICS, cCollision);
		goVec.push_back(gObject);
	}

	tmodel = new GameObject(1);
	tmodel->SetName("Base");
	tModel = new ComponentGraphics();
	tModel->setOwner(tmodel);
	tModel->loadModel("../Debug/models/base.dae");
	gp = tModel;
	tmodel->AddComponent(GRAPHICS, gp);
	tCol = new ComponentCollision();
	tCol->setCollisionMask(tModel->getScene());
	tCol->setOwner(model);
	tCol->setCollisionElip(glm::vec3(1, 6, 2));
	tmodel->pos = glm::vec3(0, -0.5, 0);
	gp = tCol;
	tmodel->AddComponent(ComponentId::PHYSICS, gp);
	model->scale = glm::vec3(10, 10, 10);
	
	//qtmodel = new GameObject(2);
	//qtmodel->SetName("StaticObject");
	//qtModel = new ComponentGraphics();
	//qtModel->setOwner(qtmodel);
	//qtModel->loadModel("../Debug/models/2boxesv1.dae");
	//gp = qtModel;
	//qtmodel->AddComponent(GRAPHICS, gp);
	//qtCol = new ComponentCollision();
	//qtCol->setCollisionMask(qtModel->getScene());
	//qtCol->setOwner(model);
	//qtCol->setCollisionElip(glm::vec3(1, 3, 2));
	//gp = qtCol;
	//qtmodel->AddComponent(ComponentId::PHYSICS, gp);
	//qtmodel->pos = glm::vec3(5, 0, 0);

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

	//tModel->render();
	//gModel->render();
	//qtModel->render();
	//END MODEL INITS
	cInput = new ComponentInput(0.25, 0.25);

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
