// third-party libraries
#include "include\GL\glew.h"
#include "include\GL\glfw3.h"
#include "lib\glm\glm.hpp"
#include "lib\glm\gtx\vector_angle.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"
#include "lib\glm\gtx\rotate_vector.hpp"
#include "include\IL\ilut.h"
#include "SoundManager.h"

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <list>
#include <sstream>
#include <algorithm>
#include <time.h>

// classes
#include "Program.h"
#include "ComponentLight.h"
#include "Texture.h"
#include "Camera.h"
#include "ComponentInput.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Framebuffer.h"
#include "FramebufferEffects.h"

#include "TextRendering.h"
#include "2dOverlayAnim.h"
#include "Target.h"
#include "Projectile.h"
#include "Model.h"
#include "NavMesh.h"
#include "RayVsOBB.h"

enum game_state { GAME, MENU };
float hitTimer = 0.0;
//Bad Inits need to fix at a later time
//Pls no kill future me.  I sorry
float runTime = 0;
std::vector<GameObject*> goVec;
std::vector<Target*> targets;
int currentEnemyToUpdate = 0;
int targetsKilled = 0;
twodOverlay* crosshair;
twodOverlay* startscreen;
twodOverlayAnim* skull;
twodOverlay* ShieldBack;
twodOverlay* HPback;
//todo: revert back to menu
game_state gameState = MENU;
Interpolation camInterp;
glm::vec3 fontColour = glm::vec3(117, 176, 221);
glm::vec3 spotLightColour = glm::vec3(158, 64, 60);
std::vector<unsigned int> scoreTable;
unsigned int score;
unsigned int ammo = 11;
float reloadTimer = 0.0f;
bool zoomingIn = false;
float maxFOV = 70, minFOV = 40, currentFOV, zoomingTimer = 0.0f;
#include "ShieldVariables.h"
#include "WeaponVariables.h"

std::vector<OwnerList> soundcopy;
float playTime = 0;
NavMesh testNaveMesh;

SoundManager* SManager;
FSystem* SoundSystem;
//Model* testmodel;

Framebuffer* framebuff[4];
FramebufferEffects* framebuffeffects;

bool numpadPress[9];

//GameObject* animatedMech;
//Model* animatedMechGC;
Terrain* ground;
Skybox* sky;
Skybox* skyObs;
//TODO : World/Target Loading, Menu, Timer, Target Counter

void initFSystem(){
	SoundSystem = new FSystem;
	SManager = new SoundManager(SoundSystem, std::string("../Debug/media/"), std::string("mySounds.txt"));
	//FSound* laserSound = new FSound(SoundSystem, "../Debug/media/drumloop.wav", SOUND_TYPE_3D_LOOP, ROLLOFF_LINEARSQUARE, 0.5, 20);
	//laserSound->Play();
	//laserSound->soundPos = FMOD_VECTOR{ 0, -28, 0 };
	
};

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
	Program::getInstance().createShader("pass", GL_VERTEX_SHADER, "shaders/pass.vert");
	Program::getInstance().createShader("pass", GL_FRAGMENT_SHADER, "shaders/pass.frag");
}
// constants
//const glm::vec2 SCREEN_SIZE(1920, 1080);
const glm::vec2 SCREEN_SIZE(1280, 800);

// globals
GLFWwindow* gWindow = NULL;
double gScrollY = 0.0;
GLfloat gDegreesRotated = 0.0f;

GameObject* model;
Model* gModel;
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
	Camera::getInstance().setPosition(glm::vec3(1050, 50, 0));
	Camera::getInstance().setNearAndFarPlanes(0.1f, 1024.f);

	shieldHealth = 100.f; //Just work with me.
}
void startGame()
{
	SManager->FindAndPlay("Background", "one");
	gameState = GAME;
	//Camera::getInstance().offsetPosition(model->pos - Camera::getInstance().position());
	Camera::getInstance().lookAt(glm::normalize(glm::vec3(1, 0, 1)));
	model->pos = glm::vec3(15.5, 0.5, 1);
	model->health = 100.f;
	score = 0;
	playTime = 0;
	ammo = 11;
	reloadTimer = 0.0f;
	maxShieldHealth = 100.f;
	shieldRechargeTimer = 0.f;
	shieldMaxTimer = 3.5f;
	shieldRechargeAmount = 30.f;

	playerIsHit = false;
	hitInvulnTimer = 0.0f;
	maxInvulnTime = 0.5f;
	targetsKilled = 0;
	for (int i = 0, s = targets.size(); i < s; i++)
	{
		targets[i]->go->scale = glm::vec3(1, 1, 1);
		targets[i]->hasSpottedPlayer = false;
		targets[i]->update(0.166f, testNaveMesh);
		targets[i]->hit = false;
		targets[i]->alive = true;
		targets[i]->go->health = 100.f;
	}
	Camera::getInstance().setNearAndFarPlanes(0.1f, 1024.0f);
	Camera::getInstance().lookAt(glm::vec3(0, 0.75, 0));
	//SManager->vSounds[0][5]->Play();
}
void LoadTargets()
{
	targets.reserve(100);
	float randomX, randomY;
	//load in targets
	for (int i = 0; i < 6; i++)
	{	
		OwnerList temp = *SManager->GetOwnerList("Target");
		soundcopy.push_back(temp);
		Target* tar = new Target("models/Dummy.dae", 0.5,&soundcopy[i]);

		//last point needs to == first point

		if (i == 0)
		{
			//tar->interp.points.push_back(glm::vec3(5, 0.4, 0));
			//tar->interp.points.push_back(glm::vec3(6, 0.4, 1));
			//tar->interp.points.push_back(glm::vec3(7, 0.4, 0));
			//tar->interp.points.push_back(glm::vec3(6, 0.4, -1));
			//tar->interp.points.push_back(glm::vec3(5, 0.4, 0));
			tar->interp.state = LINEAR;
			randomX = randomClampedInt(0, testNaveMesh.TriangleSet.size() - 1);
			randomY = randomClampedInt(0, testNaveMesh.TriangleSet[randomX].size() - 1);
			tar->tempPosition = testNaveMesh.TriangleSet[randomX][randomY];
			tar->generatePath(testNaveMesh);
			//tar->laserSound = laserSound; //Seriously need get sounds to the other classes in a better way than this.
			tar->fireTimeTolerance = randomClampedFloat(1.f, 3.f);
		}
		if (i == 1)
		{
			//tar->interp.points.push_back(glm::vec3(0, 0.4, 12));
			//tar->interp.points.push_back(glm::vec3(0, 0.4, 9));
			//tar->interp.points.push_back(glm::vec3(-1, 0.4, 9));
			//tar->interp.points.push_back(glm::vec3(1, 0.4, 9));
			//tar->interp.points.push_back(glm::vec3(0, 0.4, 9));
			//tar->interp.points.push_back(glm::vec3(0, 0.4, 12));
			tar->interp.state = LINEAR;
			randomX = randomClampedInt(0, testNaveMesh.TriangleSet.size() - 1);
			randomY = randomClampedInt(0, testNaveMesh.TriangleSet[randomX].size() - 1);
			tar->tempPosition = testNaveMesh.TriangleSet[randomX][randomY];
			tar->generatePath(testNaveMesh);
			//tar->laserSound = laserSound;
			tar->fireTimeTolerance = randomClampedFloat(1.f, 3.f);
		}
		if (i == 2)
		{
			//tar->interp.points.push_back(glm::vec3(-3, 0.4, 12));
			//tar->interp.points.push_back(glm::vec3(-3, 0.4, 8));
			//tar->interp.points.push_back(glm::vec3(-2, 0.4, 8));
			//tar->interp.points.push_back(glm::vec3(-4, 0.4, 8));
			//tar->interp.points.push_back(glm::vec3(-3, 0.4, 8));
			//tar->interp.points.push_back(glm::vec3(-3, 0.4, 12));
			tar->interp.state = LINEAR;
			randomX = randomClampedInt(0, testNaveMesh.TriangleSet.size() - 1);
			randomY = randomClampedInt(0, testNaveMesh.TriangleSet[randomX].size() - 1);
			tar->tempPosition = testNaveMesh.TriangleSet[randomX][randomY];
			tar->generatePath(testNaveMesh);
			//tar->laserSound = laserSound;
			tar->fireTimeTolerance = randomClampedFloat(1.f, 3.f);
		}
		if (i == 3)
		{
			//tar->interp.points.push_back(glm::vec3(-6, 0.4, 12));
			//tar->interp.points.push_back(glm::vec3(-6, 0.4, 8));
			//tar->interp.points.push_back(glm::vec3(-6, 0.4, 12));
			tar->interp.state = LINEAR;
			randomX = randomClampedInt(0, testNaveMesh.TriangleSet.size() - 1);
			randomY = randomClampedInt(0, testNaveMesh.TriangleSet[randomX].size() - 1);
			tar->tempPosition = testNaveMesh.TriangleSet[randomX][randomY];
			tar->generatePath(testNaveMesh);
			//tar->laserSound = laserSound;
			tar->fireTimeTolerance = randomClampedFloat(1.f, 3.f);
		}
		if (i == 4)
		{
			//tar->interp.points.push_back(glm::vec3(3, 0.4, 12));
			//tar->interp.points.push_back(glm::vec3(3, 0.4, 8));
			//tar->interp.points.push_back(glm::vec3(3, 0.4, 12));
			tar->interp.state = LINEAR;
			randomX = randomClampedInt(0, testNaveMesh.TriangleSet.size() - 1);
			randomY = randomClampedInt(0, testNaveMesh.TriangleSet[randomX].size() - 1);
			tar->tempPosition = testNaveMesh.TriangleSet[randomX][randomY];
			tar->generatePath(testNaveMesh);
			//tar->laserSound = laserSound;
			tar->fireTimeTolerance = randomClampedFloat(1.f, 3.f);
		}
		if (i == 5)
		{
			//tar->interp.points.push_back(glm::vec3(0, 0.4, 0));
			//tar->interp.points.push_back(glm::vec3(-4, 0.4, 1));
			//tar->interp.points.push_back(glm::vec3(-4, 0.4, -1));
			//tar->interp.points.push_back(glm::vec3(0, 0.4, 0));
			tar->interp.state = LINEAR;
			randomX = randomClampedInt(0, testNaveMesh.TriangleSet.size() - 1);
			randomY = randomClampedInt(0, testNaveMesh.TriangleSet[randomX].size() - 1);
			tar->tempPosition = testNaveMesh.TriangleSet[randomX][randomY];
			tar->generatePath(testNaveMesh);
			//tar->laserSound = laserSound;
			tar->fireTimeTolerance = randomClampedFloat(1.f, 3.f);
		}


		tar->interp.buildCurve();
		targets.push_back(tar);
	}
}

static void DrawSceneShadowPass()
{
	//ground->RenderShadowPass();
	//animatedMechGC->render(); //Source of the glError 1282
	for (unsigned int i = 0, s = goVec.size(); i < s; i++)
	{
		Model* cg = static_cast<Model*>(goVec[i]->GetComponent(GRAPHICS));
		cg->renderShadowPass();
	}

	for (unsigned int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
	{
		ObjectManager::instance().pMap[i]->cg->renderShadowPass();
	}

	for (unsigned int i = 0, s = targets.size(); i < s; i++)
	{
		if (targets[i]->alive)
		{
			targets[i]->cg->renderShadowPass();
			//targets[i]->cc->renderHitbox();
		}
	}
}

static void DrawScene(int shadowMapTexID)
{
	skyObs->render(true);
	sky->render(false);

	ground->Render(shadowMapTexID);
	//animatedMechGC->render(); //Source of the glError 1282
	for (unsigned int i = 0, s = goVec.size(); i < s; i++)
	{
		Model* cg = static_cast<Model*>(goVec[i]->GetComponent(GRAPHICS));
		cg->setShadowMapID(shadowMapTexID);
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

	for (unsigned int i = 0, s = ObjectManager::instance().enemyPMap.size(); i < s; i++)
	{
		ObjectManager::instance().enemyPMap[i]->cg->render();
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

	//testmodel->render();
}
// draws a single frame
static void Render() {
	framebuff[0]->Bind();



    // clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	framebuffeffects->PrepShadowMap();
	DrawSceneShadowPass();
	DrawScene(framebuffeffects->FinShadowMap());

	//_snprintf_s(buffer, 5, "%i", score);
    // swap the display buffers (displays what was just drawn)

	//if (numpadPress[3])
	if (gameState == GAME)
	{
		framebuffeffects->Toon(false);
		framebuffeffects->GodRays(glm::vec3(-8.f, 9.f, 10.f));
	}
	else if (gameState == MENU)
	{
		framebuffeffects->Toon(true);
	}
	//if (numpadPress[1])
	framebuffeffects->Bloom(4);
	//if (numpadPress[2])
	framebuffeffects->FXAA();		

	framebuff[0]->Unbind();
	Program::getInstance().bind("pass");
	framebuff[0]->PassTextureToPreBoundShader("tex0", 0);
	framebuff[0]->RenderQuad();

	//Render HUD
	glDisable(GL_DEPTH_TEST);
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
		char buffer[8];
		_snprintf_s(buffer, 8, "%i/%i", targetsKilled, targets.size());
		TextRendering::getInstance().printText2D(buffer, -0.70f, -0.8f, 0.125f, fontColour);
		char scbuff[64];
		_snprintf_s(scbuff, 64, "SCORE:%i", score);
		TextRendering::getInstance().printText2D(scbuff, -0.49f, 0.91f, 0.075f, fontColour);
		char amBuff[8];
		_snprintf_s(amBuff, 8, "AMMO:%i", ammo);
		TextRendering::getInstance().printText2D(amBuff, 0.3f, -0.8f, 0.1f, fontColour);
		ShieldBack->cutoffPercent(shieldHealth / maxShieldHealth);
	    ShieldBack->render();
		HPback->cutoffPercent(model->health / 100.f);
	    HPback->render();
	}
	glEnable(GL_DEPTH_TEST);

    glfwSwapBuffers(gWindow);
}
#define SHOT_CD 0.1
float shotcd = 0;
// update the scene based on the time elapsed since last update
static void Update(float secondsElapsed) {
	SManager->Update();
	SoundSystem->Update();
	runTime += secondsElapsed;

	glm::vec3 lInput;
	glm::vec2 rInput;
	Camera* cam = &Camera::getInstance();
	glm::vec3 f = cam->forward();
	glm::vec3 r = cam->right();
	glm::vec3 fmy = cam->forward();
	fmy.y = 0;
	bool shoot = false;

	FMOD_VECTOR _pos, _for, _up;
	_pos = { cam->position().x, cam->position().y, cam->position().z };
	_for = { -cam->forward().x, cam->forward().y, -cam->forward().z };
	_up = { cam->up().x, cam->up().y, cam->up().z };

	SManager->UpdateSysO(cam->position(), -cam->forward(), cam->up(), glm::vec3(0, 0, 0));

	for (int i = 0; i < 9; i++)
	{
		if (glfwGetKey(gWindow, GLFW_KEY_KP_0 + i))
			numpadPress[i] = true;
		else
			numpadPress[i] = false;
	}

	
	//system("CLS");
	//cout << "CamPos: " << fsystem->listenerpos.x << " " << fsystem->listenerpos.y << " " << fsystem->listenerpos.z << flush;
	//cout << "\nSoundPostion:" << test->name << " :" << test->soundPos.x << " " << test->soundPos.y << " " << test->soundPos.z << flush;

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
		if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT))
		{
			shoot = true;
		}
		if (glfwGetKey(gWindow, 'R') && ammo < 10 && reloadTimer == 0.0f)
			reloadTimer = 0.0001f;

		if (glfwGetKey(gWindow, '1'))
		{
			currentWeapon = machineGun;
		}
		if (glfwGetKey(gWindow, '2'))
		{
			currentWeapon = shotgun;
		}
		if (glfwGetKey(gWindow, '3'))
		{
			currentWeapon = bfg;
		}



		//rotate camera based on mouse movement
		float mouseSensitivity = 0.05f;
		zoomingIn = false;
		if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT))
		{
			zoomingIn = true;
		}
		if (zoomingIn)
		{
			zoomingTimer += secondsElapsed * 7;
			if (zoomingTimer > 1.0f)
				zoomingTimer = 1.0f;
			currentFOV = lerp(maxFOV, minFOV, zoomingTimer);
			Camera::getInstance().setFieldOfView(currentFOV);
			mouseSensitivity = 0.02f;
		}
		else
		{
			zoomingTimer -= secondsElapsed * 7;
			if (zoomingTimer < 0.0f)
				zoomingTimer = 0.0f;
			currentFOV = lerp(maxFOV, minFOV, zoomingTimer);
			Camera::getInstance().setFieldOfView(currentFOV);
			mouseSensitivity = 0.05f;
		}
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
		model->pos += fmy * (c->getOwner()->vel * lInput.z) + model->force;
		model->force = model->force / 1.2f;
		if (glm::length(model->force) < 0.1f)
			model->force = glm::vec3(0);
		model->pos += cam->right() * (c->getOwner()->vel * lInput.x);

		cam->offsetPosition(model->pos - cam->position());

		c->getOwner()->dir = glm::rotateX(c->getOwner()->dir, -rInput.y);
		c->getOwner()->dir = glm::rotateY(c->getOwner()->dir, rInput.x);
		cam->offsetOrientation(-rInput.y, rInput.x);
		//End Camera code

		for (int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
			ObjectManager::instance().pMap[i]->update(secondsElapsed);
		for (unsigned int i = 0, s = ObjectManager::instance().enemyPMap.size(); i < s; i++)
			ObjectManager::instance().enemyPMap[i]->update(secondsElapsed);

		glm::vec3 p = Camera::getInstance().position();
		if (shoot && shotcd > SHOT_CD && ammo > 0 && reloadTimer == 0.0f)
		{
			FSound* sounds = SManager->FindSound("Player","Projectile");
			//sounds->Play();
			Projectile* pr;

			if (currentWeapon == machineGun)
			{
				pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.015f, 0.015f))), 0.5, 25, 10, SManager->FindSound("Player", "Projectile"));
				ObjectManager::instance().pMap.push_back(pr);
				ammo--;
			}
			else if (currentWeapon == shotgun)
			{
				if (ammo >= shotgun)
				{
					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 0.5, 15, 10, SManager->FindSound("Player", "Projectile"));
					ObjectManager::instance().pMap.push_back(pr);
					ammo--;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 0.5, 15, 10, SManager->FindSound("Player", "Projectile"));
					ObjectManager::instance().pMap.push_back(pr);
					ammo--;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 0.5, 15, 10, SManager->FindSound("Player", "Projectile"));
					ObjectManager::instance().pMap.push_back(pr);
					ammo--;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 0.5, 15, 10, SManager->FindSound("Player", "Projectile"));
					ObjectManager::instance().pMap.push_back(pr);
					ammo--;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 0.5, 15, 10, SManager->FindSound("Player", "Projectile"));
					ObjectManager::instance().pMap.push_back(pr);
					ammo--;
				}
			}
			else if (currentWeapon == bfg)
			{
				if (ammo >= bfg)
				{
					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.015f, 0.015f))), 0.3, 100, 10, SManager->FindSound("Player", "Projectile"));
					pr->go->scale = glm::vec3(1.5f);
					ObjectManager::instance().pMap.push_back(pr);
					ammo -= bfg;
				}
			}



				shotcd = 0;
			
		}

		if (reloadTimer > 0.0f)
		{
			reloadTimer += secondsElapsed;
			if (reloadTimer > 2.f)
			{
				reloadTimer = 0.0f;
				ammo = 10;
			}
		}

		
		hitTimer += secondsElapsed;
		if (playerIsHit){
			if (hitTimer > 0.25){
				hitTimer = 0;
				SManager->FindAndPlay("Player", "NoShield");
			}
		}
		if (hitInvulnTimer >= 0.0f)
		{
			hitInvulnTimer -= secondsElapsed;
			if (hitInvulnTimer < 0.0f)
			{
				playerIsHit = false;
			}
		}

		if (shieldHealth < maxShieldHealth)
		{
			shieldRechargeTimer -= secondsElapsed;
			if (shieldRechargeTimer <= 0.0f)
			{
				shieldHealth += shieldRechargeAmount * secondsElapsed;

				if (shieldHealth > maxShieldHealth)
					shieldHealth = maxShieldHealth;
			}
		}
		

		CollisionManager::instance().checkAll();
		
		ObjectManager::instance().updateProjectile(secondsElapsed);
		ObjectManager::instance().updateEnemyProjectile(secondsElapsed);
		
		if (targets[currentEnemyToUpdate]->hasSpottedPlayer == true && targets[currentEnemyToUpdate]->alive)
		{
			targets[currentEnemyToUpdate]->selectedToDoCombatUpdate = true;
		}
		currentEnemyToUpdate++;
		if (currentEnemyToUpdate >= targets.size())
		{
			currentEnemyToUpdate = 0;
		}

		for (int i = 0, s = targets.size(); i < s; i++)
		{
			
			targets[i]->update(secondsElapsed / 5, testNaveMesh);
			targets[i]->go->pos.y = ground->HeightAtLocation(targets[i]->go->pos) + 0.4; //this moves the targets to the correct position above the ground.

			if (targets[i]->hasSpottedPlayer == false && targets[i]->alive)
			{
				targets[i]->canSeePlayer(model->pos);
			}

			//if (shoot)
			//{
			//	if (RayVsOBB((model->pos), cam->forward(), targets[i]->cc->_cMesh[0]->fmin, targets[i]->cc->_cMesh[0]->fmax))
			//	{
			//		targets[i]->alive = false;
			//		skull->play();
			//		targetsKilled++;
			//	}
			//}

			if (targets[i]->hit && targets[i]->alive)
			{
				targets[i]->alive = false;
				skull->play();
				targetsKilled++;
			}
			
			targets[i]->fireTimer += secondsElapsed;
			if (targets[i]->fireTimer >= targets[i]->fireTimeTolerance && targets[i]->alive && targets[i]->hasSpottedPlayer)
			{
				targets[i]->fireTimer = 0.f;
				targets[i]->weaponProjectile = new Projectile(targets[i]->go->pos, glm::normalize((model->pos - targets[i]->go->pos) + glm::vec3(randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f))) /* targets[i]->vecToPlayer*/, 0.1, 10, 7, SManager->FindSound("Player", "Projectile"));
				targets[i]->weaponProjectile->go->scale = glm::vec3(1.1f);
				targets[i]->weaponProjectile->go->SetName("EnemyProjectile");
				targets[i]->weaponProjectile->handle = ObjectManager::instance().enemyPMap.size();
				ObjectManager::instance().enemyPMap.push_back(targets[i]->weaponProjectile);
				targets[i]->fireTimeTolerance = randomClampedFloat(0.5f, 2.f);
			}
		}

		skull->update(secondsElapsed);

		//model->pos.y = 0.5;
		float h = ground->HeightAtLocation(model->pos);
		//printf("%f\n", h);
		model->pos.y = h + 0.5;

		cam->offsetPosition(model->pos - cam->position());

		playTime += secondsElapsed;
		if (score >= 0)
		{
			float gameTime = playTime / 1000;
			score = ((gameTime * 100) / pow(gameTime, 2)) * 100 - 15;
		}
		else
			score = 0;

		if (targetsKilled == targets.size() || c->IsPressed(XINPUT_GAMEPAD_BACK))
			wonGame();
		
		//Only a different if statement because we need a loseGame function to replace the wonGame function.
		if (model->health <= 0)
		{
			wonGame();
		}

		
		//printf("%f\n", ground->HeightAtLocation(model->pos));
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
	//animatedMechGC->BoneTransform(tElap, trans);
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
	initFSystem();
	testNaveMesh.loadNavMesh("../Debug/models/NavMeshes/TestLevelNavMesh-scaled.obj");
	srand(time(NULL));
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "Mek", NULL /*glfwGetPrimaryMonitor()*/, NULL);
	if (!gWindow)
		throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 4.3?");

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
    if(!GLEW_VERSION_4_3)
        throw std::runtime_error("OpenGL 4.3 API is not available.");

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialise the gWoodenCrate asset
    LoadWoodenCrateAsset();

    // create all the instances in the 3D scene based on the gWoodenCrate asset
    CreateInstances();

	framebuff[0] = new Framebuffer();
	framebuff[0]->CreateDepthTexture(SCREEN_SIZE.x, SCREEN_SIZE.y);
	framebuff[0]->CreateColorTexture(4, SCREEN_SIZE.x, SCREEN_SIZE.y);
	framebuff[1] = new Framebuffer();
	framebuff[1]->CreateDepthTexture(SCREEN_SIZE.x/2, SCREEN_SIZE.y/2);
	framebuff[1]->CreateColorTexture(1, SCREEN_SIZE.x/2, SCREEN_SIZE.y/2);
	framebuff[2] = new Framebuffer();
	framebuff[2]->CreateDepthTexture(SCREEN_SIZE.x/2, SCREEN_SIZE.y/2);
	framebuff[2]->CreateColorTexture(1, SCREEN_SIZE.x/2, SCREEN_SIZE.y/2);
	framebuff[3] = new Framebuffer();
	framebuff[3]->CreateDepthTexture(SCREEN_SIZE.x, SCREEN_SIZE.y);
	framebuff[3]->CreateColorTexture(4, SCREEN_SIZE.x, SCREEN_SIZE.y);
	framebuffeffects = new FramebufferEffects(framebuff);
	framebuffeffects->LoadBloomShaders();
	framebuffeffects->LoadFXAAShaders();
	framebuffeffects->loadToonShaders();
	framebuffeffects->LoadShadowMapShaders();
	framebuffeffects->LoadGodRayShaders();

    // setup Camera::getInstance()
    Camera::getInstance().setPosition(glm::vec3(1050, 50, 0));
    Camera::getInstance().setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
	Camera::getInstance().setNearAndFarPlanes(0.1f, 1024.0f);
	Camera::getInstance().setFieldOfView(maxFOV);

	crosshair = new twodOverlay("crosshair.png", 0, 0, 1);
	skull = new twodOverlayAnim("killSkull.png", 5, 0.5);
	ShieldBack = new twodOverlay("ShieldBarBack.png", 0, 0.85, 35);
	HPback = new twodOverlay("HPBarBack.png", 0, 0.84, 35);
	startscreen = new twodOverlay("pressStart.png", 0, 0, 10);
	skull->updatePos(-0.85f, -0.75f, 4);
	skull->cycle = true;

	ground = new Terrain();
	ground->LoadHeightMap("testhm.png", 1, 5, 0.8);
	ground->InitRender();
	char* sb[6] = { "ri.png", "le.png", "to.png", "bo.png", "ba.png", "fr.png" };
	sky = new Skybox(sb);
	char* Osb[6] = { "ri-O.png", "le-O.png", "to-O.png", "bo-O.png", "ba-O2.png", "fr-O.png" };
	skyObs = new Skybox(Osb);
	//MODEL INITS

	prepProjectiles();

	model = new GameObject(0);
	model->SetName("Player");
	gModel = new Model();
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
		if (i != 5 && i != 8 && i != 10 && i != 11 && i != 12)
		{
			GameObject *gObject = new GameObject(goVec.size());
			Model *cModel = new Model();
			ComponentCollision *cCollision = new ComponentCollision();
			Component *c;

			if (i == 0)
			{
				gObject->SetName("Spawn Container 1");
				cModel->loadModel("models/Tree 1.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-10.526, 0, -176.58);
				gObject->rot = glm::vec3(0, -34.f, 0);
			}
			else if (i == 1)
			{
				gObject->SetName("Water Tower");
				cModel->loadModel("models/Watertower-LP.dae");

				gObject->scale = glm::vec3(3, 3, 3);
				gObject->pos = glm::vec3(-112.75, 0, 105);
			}
			else if (i == 2)
			{
				gObject->SetName("MenuScene");
				cModel->loadModel("models/Warehouse.dae");

				gObject->scale = glm::vec3(1, 1, 1);// glm::vec3(1.6, 1.6, 1.6);
				gObject->pos = glm::vec3(50, 0, 145);
			}
			else if (i == 3)
			{
				gObject->SetName("Spawn Container 2");
				cModel->loadModel("models/Tree 2.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-69.65f, 0, 108.334);
			}
			else if (i == 4)
			{
				gObject->SetName("Middle Plus");
				cModel->loadModel("models/Shipping Container.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-14, 0, -73);
			}
			else if (i == 5)
			{
				gObject->SetName("North Wall");
				cModel->loadModel("models/Container_Wal_LPl.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(100, 0, 165);
			}
			else if (i == 6)
			{
				gObject->SetName("Dumbster");//Crane
				cModel->loadModel("models/Garbage Bin.dae");
				gObject->pos = glm::vec3(59.751, 0, -76.667);
				gObject->scale = glm::vec3(1);
				gObject->rot = glm::vec3(0, 130.6, 0);
			}
			else if (i == 7)
			{
				gObject->SetName("Tall Rock");
				cModel->loadModel("models/Tall Rock.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-92.424f, 0, -106.131);
			}
			else if (i == 8)
			{
				gObject->SetName("Middle Plus");
				cModel->loadModel("models/Container.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-5, 0, -20);
			}
			else if (i == 9)
			{
				gObject->SetName("Container 2");
				cModel->loadModel("models/Shipping Container.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-14, 0, 73);
				gObject->rot = glm::vec3(0, 65.44f , 0);
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
				cModel->loadModel("models/Shipping Container.dae");

				gObject->scale = glm::vec3(0.70, 0.70, 0.70);
				gObject->pos = glm::vec3(-55, 0, -47);
			}
			else if (i == 14)
			{
				gObject->SetName("Container 90");
				cModel->loadModel("models/Shipping Container.dae");

				gObject->scale = glm::vec3(0.70, 0.70, 0.70);
				gObject->pos = glm::vec3(-94.87, 0, -125.5);
				gObject->rot = glm::vec3(0, -48.489f, 0);
			}
			else if (i == 15)
			{
				gObject->SetName("Small Rock 1");
				cModel->loadModel("models/Rock 1 Round.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-94.393, 0, -103.22);
			}
			else if (i == 16)
			{
				gObject->SetName("Small Rock 1");
				cModel->loadModel("models/Rock 1 Round.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-3.347f, 0, -180.318f);
			}
			else if (i == 17)
			{
				gObject->SetName("Small Rock 2");
				cModel->loadModel("models/Rock 2 Round.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-5.196f, 0, -172.384f);
			}
			else if (i == 18)
			{
				gObject->SetName("Middle Plus North");
				cModel->loadModel("models/Shipping Container.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-110, 0, -149);
				gObject->rot = glm::vec3(0, -48.5f, 0);
			}
			else if (i == 19)
			{
				gObject->SetName("Fountain");
				cModel->loadModel("models/Fountain.dae");

				gObject->scale = glm::vec3(1.5);
				gObject->pos = glm::vec3(0, 0, 0);
			}
			else if (i == 20)
			{
				gObject->SetName("Crane");
				cModel->loadModel("models/Crane.dae");

				gObject->scale = glm::vec3(1.5);
				gObject->pos = glm::vec3(84.727f, 0, -154.085f);
			}
			else if (i == 21)
			{
				gObject->SetName("Destroyed House");
				cModel->loadModel("models/Destroyed Building.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-35, 0, -167.582f);
				gObject->rot = glm::vec3(0, -90.f, 0);
			}

			gObject->pos /= 10.f;
			gObject->pos.y = ground->HeightAtLocation(gObject->pos);

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
	
	//testmodel = new Model();
	//testmodel->loadModel("models/Watertower.dae");

	spotLightColour = glm::normalize(spotLightColour);
	for (int i = 0; i < 6; i++)
	{
		LightComponent* light;
		if (i == 0)
		{
			light = new LightComponent(lSPOT);
			SpotLight* lc = new SpotLight;
			lc->Base.Base.Color = spotLightColour;
			lc->Base.Base.AmbientIntensity = 0.04f;
			lc->Base.Base.DiffuseIntensity = 0.04f;
			
			lc->Base.Atten.Constant = 1;
			lc->Base.Atten.Exp = 0;
			lc->Base.Atten.Linear = 0;
			
			lc->Cutoff = 0.9f;
			lc->Base.Position = glm::vec3(-6, 10, 21);
			lc->Direction = glm::vec3(0, 0, 0);
			
			light->SetVars(lSPOT, lc);
		}
		if (i == 1)
		{
			light = new LightComponent(lSPOT);
			SpotLight* lc = new SpotLight;
			lc->Base.Base.Color = spotLightColour;
			lc->Base.Base.AmbientIntensity = 0.025f;
			lc->Base.Base.DiffuseIntensity = 0.025f;
			
			lc->Base.Atten.Constant = 1;
			lc->Base.Atten.Exp = 0;
			lc->Base.Atten.Linear = 0;
			
			lc->Cutoff = 0.8;
			lc->Base.Position = glm::vec3(-8, 9, 10);
			lc->Direction = (glm::vec3(-10, 0, 20));
			
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
			lc->Base.Base.AmbientIntensity = 0.04f;
			lc->Base.Base.DiffuseIntensity = 0.04f;
			
			lc->Base.Atten.Constant = 1.0f;
			lc->Base.Atten.Exp = 0;
			lc->Base.Atten.Linear = 0;
			
			lc->Cutoff = 0.9f;
			lc->Base.Position = glm::vec3(-10, 10, -10);//4 1 0
			lc->Direction = glm::vec3(-1, 0, -1);// 5 0 0
			
			light->SetVars(lSPOT, lc);
		}
	}

	//animatedMech = new GameObject(100);
	//animatedMechGC = new Model();
	//animatedMechGC->loadModel("models/Test_Animation_DAE.dae");
	//Component* c = animatedMechGC;
	//animatedMech->AddComponent(GRAPHICS, c);
	//animatedMech->pos = glm::vec3(0, 0, 0);
	//animatedMech->scale = glm::vec3(1, 1, 1);

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
