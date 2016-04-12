// third-party libraries
#include "include\GL\glew.h"
#include "include\GL\glfw3.h"
#include "lib\glm\glm.hpp"
#include "lib\glm\gtx\vector_angle.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"
#include "lib\glm\gtx\rotate_vector.hpp"
#include "include\IL\ilut.h"
#include <SlimManager\Manager.h>

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
#include "AISpawner.h"
#include "SaveFunctions.h"

//enable or disable the code slim wrote that steals console output
#define HIJACKCONSOLE true

enum game_state { GAME, MENU, VICTORYSCREEN };
float hitTimer = 0.0;
float dshield = 0.0;
//Bad Inits need to fix at a later time
//Pls no kill future me.  I sorry
float runTime = 0;
std::vector<GameObject*> goVec;
std::vector<Target*> targets;
int maxNumOfTargets;
TargetSpawner targetSpawner1;
TargetSpawner targetSpawner2;
int currentEnemyToUpdate = 0;
int targetsKilled = 0;
twodOverlay* crosshair;
twodOverlay* startscreen;
twodOverlay* gameLogo;
twodOverlay* studioLogo;
twodOverlayAnim* skull;
twodOverlay* ShieldBack;
twodOverlay* ShieldFront;
twodOverlay* HPback;
twodOverlay* HPFront;
twodOverlay* machineIcon;
twodOverlay* shotgunIcon;
twodOverlay* bfgIcon;
twodOverlay* iconGlow;
twodOverlay* bulletTimeIconFront;
twodOverlay* bulletTimeIconBack;
twodOverlay* dashIconFront;
twodOverlay* dashIconBack;
//todo: revert back to menu
game_state gameState = MENU;
int nameSelect = 0;
int nameOffset[3] = { 0, 0, 0 };
bool isPlayingSearchAndDestroy = true;
Interpolation camInterp;
glm::vec3 fontColour = glm::vec3(117, 176, 221);
glm::vec3 ammoColour = glm::vec3(0.f, 0.f, 1.f);
glm::vec3 white = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 red = glm::vec3(1.0f, 0, 0);
glm::vec3 menuItem2Colour = glm::vec3(0.68, 0.91, 1.0);
glm::vec3 menuItem1Colour = glm::vec3(0.0, 0.4, 1.0);
glm::vec3 spotLightColour = glm::vec3(158, 64, 60);
unsigned int scoreInsertionIndex = 0;
std::vector<unsigned int> scoreTable;
std::vector<string> sndNames;
std::vector<unsigned int> survivalScoreTable;
std::vector<string> survivalNames;
float characterSpaceInterp = 0.0f;
float characterSpaceInterpIsIncreasing = true;
unsigned int score;
int enemiesAlive = 0;

float ammoInterp = 0.0f;
float noammoInterp = 0.0f;
bool noammoInterpIsIncreasing = false;
float machineGunReloadTimer = 0.0f;
float shotgunReloadTimer = 0.0f;
float bfgReloadTimer = 0.0f;
float *currentReloadTimer = &machineGunReloadTimer;
float timeFactor = 0.05f;
float maxBulletTimeCooldown = 2.0f;
float bulletTimeCooldown = maxBulletTimeCooldown;
bool bulletTimeHitZero = false;
bool isUsingBulletTime = false;
float maxDashingCooldown = 0.5f;
float dashingCooldown = maxDashingCooldown;
bool dashingHitZero = false;
bool IsDashing = false;
bool zoomingIn = false;
float maxFOV = 70, minFOV = 40, currentFOV, zoomingTimer = 0.0f;
#include "ShieldVariables.h"
#include "WeaponVariables.h"
#include "MenuSelectionVariables.h"
int* currentAmmo = &machineGunAmmo;

float playTime = 0;
float openingMessageTimer = 3.5f;
float openningMessageInterp = 0.0f;
bool openningMessageInterpIsIncreasing = false;
NavMesh testNaveMesh;
bool dShield = false;
bool BackPaused = false;

//Model* testmodel;
bool isTutorial = false;
Framebuffer* framebuff[5];
FramebufferEffects* framebuffeffects;

bool numpadPress[9];

//GameObject* animatedMech;
//Model* animatedMechGC;
Terrain* ground;
Skybox* sky;
Skybox* skyObs;
//TODO : World/Target Loading, Menu, Timer, Target Counter
GameObject* arms;
bool PTUT = true;
bool RunWasd = true;
bool RunShield = true;
bool RunEnemy = true;
bool RunDash = true;
bool RunSlowTime = true;
Manager* manager = new Manager;
Sound* s_pWalk = new Sound;
Sound* s_sReload = new Sound;
bool isReverb = true;
void Tutorial(){
	//system("CLS");
	//std::cout << "Play Tutorial : " << PTUT << std::endl << std::flush;
	//std::cout << "WASD Tutorial: " << RunWasd << " p: " << SManager->FindSound("Tutorial", "wasd")->isPlaying<<std::endl << std::flush;
	//std::cout << "Enemy Tutorial: " << RunEnemy << " p: " << SManager->FindSound("Tutorial", "enemy")->isPlaying << std::endl << std::flush;
	//std::cout << "Shield Tutorial: " << RunShield << " p: " << SManager->FindSound("Tutorial", "shield")->isPlaying << std::endl << std::flush;
	//std::cout << "Dash Tutorial: " << RunDash << " p: " << SManager->FindSound("Tutorial", "dash")->isPlaying << std::endl << std::flush;
	//std::cout << "Slow time Tutorial: " << RunSlowTime << " p: " << SManager->FindSound("Tutorial", "slowtime")->isPlaying << std::endl << std::flush;

	//if (manager->GetSoundManager()->FindSound("Tutorial", "wasd")->GetIsPlaying()){
	//	PTUT = true;
	//}
	//else if (manager->GetSoundManager()->FindSound("Tutorial", "enemy")->GetIsPlaying()){
	//	PTUT = true;
	//}
	//else if (manager->GetSoundManager()->FindSound("Tutorial", "shield")->GetIsPlaying()){
	//	PTUT = true;
	//}
	//else if (manager->GetSoundManager()->FindSound("Tutorial", "slowtime")->GetIsPlaying()){
	//	PTUT = true;
	//}
	//else if (manager->GetSoundManager()->FindSound("Tutorial", "dash")->GetIsPlaying()){
	//	PTUT = true;
	//}
	//else{
	//	PTUT = false;
	//}

}

void FreqBand(){
	//Create an instance of the ConsoleMagic class
	//cm.Init(100, 50);//Resize the console window to 100 by 50 characters
	//cm.SetTitle("Frequency Bands");
}
void InitSounds(){
	manager->GetSoundManager()->PlayAndPause("Player", "Walk",true);

	manager->GetSoundManager()->PlayAndPause("Background", "two",true);
	glm::vec3 pos{ 3.9,12.8717,12.4732 };
	manager->GetSoundManager()->FindSound("Background", "two")->SetSoundPosition(pos);
	manager->GetSoundManager()->FindAndPlay("Load", "one");
}
void InitManager(){
	manager->Init();

	InitSounds();
}
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
const glm::vec2 SCREEN_SIZE(1024, 768);

// globals
GLFWwindow* gWindow = NULL;
double gScrollY = 0.0;
GLfloat gDegreesRotated = 0.0f;

GameObject* model;
Model* gModel;
Model* playerArms;
glm::vec3 leftArmFiringPosition = { 12.f, -2.7f, -5.f };
glm::vec3 rightArmFiringPosition = { 12.f, -2.2f, 5.f };
ComponentCollision* gCol;

GameObject* ammoPlane;
Model* gAmmoPlane;

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
	return glm::translate(glm::mat4(), glm::vec3(x, y, z));
}
// convenience function that returns a scaling matrix
glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z) {
	return glm::scale(glm::mat4(), glm::vec3(x, y, z));
}
//create all the `instance` structs for the 3D scene, and add them to `gInstances`
static void CreateInstances() {

}
void wonGame()
{
	gameState = VICTORYSCREEN;
	if (isPlayingSearchAndDestroy)
	{
		for (scoreInsertionIndex = 0; scoreInsertionIndex < scoreTable.size(); scoreInsertionIndex++)
		{
			if (scoreTable[scoreInsertionIndex] < score)
				break;
		}
		if (scoreTable.size() == 0)
			scoreTable.push_back(score);
		else
			scoreTable.insert(scoreTable.begin() + scoreInsertionIndex, score);
		//scoreTable.push_back(score);
		//sort(scoreTable.begin(), scoreTable.end());
		//std::reverse(scoreTable.begin(), scoreTable.end());
	}
	else
	{
		for (scoreInsertionIndex = 0; scoreInsertionIndex < survivalScoreTable.size(); scoreInsertionIndex++)
		{
			if (survivalScoreTable[scoreInsertionIndex] < targetsKilled)
				break;
		}
		if (survivalScoreTable.size() == 0)
			survivalScoreTable.push_back(targetsKilled);
		else
			survivalScoreTable.insert(survivalScoreTable.begin() + scoreInsertionIndex, targetsKilled);
		//survivalScoreTable.push_back(targetsKilled);
		//sort(survivalScoreTable.begin(), survivalScoreTable.end());
		//std::reverse(survivalScoreTable.begin(), survivalScoreTable.end());
	}
	Camera::getInstance().setPosition(glm::vec3(1050, 50, 0));
	Camera::getInstance().setNearAndFarPlanes(0.1f, 1024.f);
	manager->GetSoundManager()->StopAll();

	machineGunAmmo = 10.f;
	shotgunAmmo = 10.f;
	bfgAmmo = 10.f;
	currentFOV = maxFOV;
	shieldHealth = 100.0f; //Just work with me.
}
void startGame()
{
	manager->GetSoundManager()->PlayAndPause("Background", "two", false);
	manager->GetSoundManager()->PlayAndPause("Background", "one",true);
	manager->GetSoundManager()->PauseSound("Load", "one",true);
	gameState = GAME; // GAME
	if (!PTUT){
		if (RunWasd){
			RunWasd = false;
			//SManager->SoundVolumeAll(0);
			//SManager->FindSound("Tutorial", "wasd")->SetVolume(1);
			manager->GetSoundManager()->FindAndPlay("Tutorial", "wasd");
			if (!manager->GetSoundManager()->FindSound("Tutorial", "wasd")->GetIsPlaying()){
				//SManager->SoundVolumeAll(1);
			}
			else{
				std::cout << "Still playing" << std::endl;
			}
		}
	}
	openingMessageTimer = 3.5f;
	//Camera::getInstance().offsetPosition(model->pos - Camera::getInstance().position());
	Camera::getInstance().lookAt(glm::normalize(glm::vec3(1, 0, 1)));
	model->pos = glm::vec3(15.5, 0.5, 1);
	model->health = 100.f;
	score = 0;
	playTime = 0;
	machineGunReloadTimer = 0.0f;
	shotgunReloadTimer = 0.0f;
	bfgReloadTimer = 0.0f;
	currentReloadTimer = &machineGunReloadTimer;
	maxShieldHealth = 100.f;
	shieldRechargeTimer = 0.f;
	shieldMaxTimer = 3.5f;
	shieldRechargeAmount = 30.f;
	nameOffset[0] = 0;
	nameOffset[1] = 0;
	nameOffset[2] = 0;
	playerIsHit = false;
	hitInvulnTimer = 0.0f;
	maxInvulnTime = 0.5f;
	targetsKilled = 0;
	for (int i = 0, s = maxNumOfTargets; i < s; i++)
	{
		if (randomClampedFloat(0, 2) > 1.f)
		{
			targets[i]->enemyType = STANDARD;
			targets[i]->maxVelocity = 12.f;
			targets[i]->go->health = 100.f;
		}
		else
		{
			targets[i]->enemyType = HEAVYHITTER;
			targets[i]->maxVelocity = 5.0f;
			targets[i]->go->health = 175.f;
		}
		targets[i]->go->scale = glm::vec3(1, 1, 1);
		targets[i]->hasSpottedPlayer = false;
		int randomX = randomClampedInt(0, testNaveMesh.TriangleSet.size() - 1);
		int randomY = randomClampedInt(0, testNaveMesh.TriangleSet[randomX].size() - 1);
		targets[i]->tempPosition = testNaveMesh.TriangleSet[randomX][randomY];
		targets[i]->go->pos = targets[i]->tempPosition.center;
		targets[i]->generatePath(testNaveMesh);
		targets[i]->hit = false;
		targets[i]->alive = true;
		targets[i]->go->dir = glm::vec3(1.f, 0.f, 0.f);
	}
    for (int i = maxNumOfTargets; i < targets.size(); i++)
    {
        targets[i]->go->scale = glm::vec3(0.1f);
        targets[i]->hasSpottedPlayer = false;
        targets[i]->update(0.166f, testNaveMesh);
        targets[i]->hit = false;
        targets[i]->alive = false;
        targets[i]->go->health = 0.f;
        targets[i]->go->pos = glm::vec3(5000.f);
        targets[i]->go->dir = glm::vec3(1.f, 0.f, 0.f);
    }
    if (isPlayingSearchAndDestroy)
    {
        targetSpawner1.deactivate();
		targetSpawner2.deactivate();
    }
    else
    {
        targetSpawner1.activate();
		targetSpawner2.activate();
    }
	Camera::getInstance().setNearAndFarPlanes(0.1f, 1024.0f);
	Camera::getInstance().lookAt(glm::vec3(0, 0.75, 0));
	//SManager->vSounds[0][5]->Play();
}
void LoadTargets()
{
	targets.reserve(50);
	float randomX, randomY;
	//load in targets
	for (int i = 0; i < 6; i++)
	{
		//OwnerList temp = *SManager->GetOwnerList("Target");
		//soundcopy.push_back(temp);
		Target* tar = new Target("models/Mek.fbx", "models/Mek2b.dae", 0.5, manager->GetSoundManager()->GetOwnerList("Target"));

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
			if (tar->fireTimeTolerance < 3.f)
				tar->firingfromRightBarrel = true;
			else
				tar->firingfromRightBarrel = false;
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
			if (tar->fireTimeTolerance < 3.f)
				tar->firingfromRightBarrel = true;
			else
				tar->firingfromRightBarrel = false;
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
			if (tar->fireTimeTolerance < 3.f)
				tar->firingfromRightBarrel = true;
			else
				tar->firingfromRightBarrel = false;
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
			if (tar->fireTimeTolerance < 3.f)
				tar->firingfromRightBarrel = true;
			else
				tar->firingfromRightBarrel = false;
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
			if (tar->fireTimeTolerance < 3.f)
				tar->firingfromRightBarrel = true;
			else
				tar->firingfromRightBarrel = false;
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
			if (tar->fireTimeTolerance < 3.f)
				tar->firingfromRightBarrel = true;
			else
				tar->firingfromRightBarrel = false;
		}


		tar->interp.buildCurve();
		targets.push_back(tar);
	}
    for (unsigned int i = 6; i < 50; i++)
    {
		Target* tar = new Target("models/Mek.fbx", "models/Mek2b.dae", 0.5, manager->GetSoundManager()->GetOwnerList("Target"));
        tar->interp.state = LINEAR;
        randomX = randomClampedInt(0, testNaveMesh.TriangleSet.size() - 1);
        randomY = randomClampedInt(0, testNaveMesh.TriangleSet[randomX].size() - 1);
        tar->tempPosition = testNaveMesh.TriangleSet[randomX][randomY];
        tar->generatePath(testNaveMesh);
        //tar->laserSound = laserSound;
        tar->fireTimeTolerance = randomClampedFloat(1.f, 3.f);
        if (tar->fireTimeTolerance < 3.f)
            tar->firingfromRightBarrel = true;
        else
            tar->firingfromRightBarrel = false;
        tar->interp.buildCurve();
        tar->go->scale = glm::vec3(0);
        tar->go->dir = glm::vec3(1, 0, 0);
        tar->alive = false;
        tar->go->health = 0.0f;
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
			if (targets[i]->enemyType == 0)
				targets[i]->cg->renderShadowPass();
			else if (targets[i]->enemyType == 1)
				targets[i]->hvycg->renderShadowPass();
			//targets[i]->cc->renderHitbox();
		}
	}
}

static void DrawScene(int shadowMapTexID)
{
	if (gameState == GAME)
		sky->render(true);
	else
		sky->render(false);
	//	skyObs->render(true);
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
	playerArms->render();

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
			if (targets[i]->enemyType == 0)
			{
				targets[i]->cg->render();
				targets[i]->healthBar->pos = targets[i]->standardHealthPosition + targets[i]->go->pos;
				targets[i]->healthBarCG->render();
			}
			else if (targets[i]->enemyType == 1)
			{
				targets[i]->hvycg->render();
				targets[i]->healthBar->pos = targets[i]->heavyHealthPosition + targets[i]->go->pos;
				targets[i]->healthBarCG->render();
			}
			
			//targets[i]->cc->renderHitbox();
		}
	}
	gAmmoPlane->set_RenderedTexture(framebuff[4]->GetTextureID(0));
	gAmmoPlane->render();
	//gCol->renderHitbox();

	//testmodel->render();
}
// draws a single frame
static void Render() {
	framebuff[4]->Bind(); // rendering the ammo texture
	glViewport(0, 0, 256, 256);
	glClearColor(0, 1, 0, 0.0); // green
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	char amBuff2[8];
	_snprintf_s(amBuff2, 8, "%i", *currentAmmo);
	TextRendering::getInstance().printText2D(amBuff2, *currentAmmo > 9 ? -0.9f : -0.2f, -0.8f, 1.2f, ammoColour);
	glEnable(GL_DEPTH_TEST);
	framebuff[0]->Bind();
	glViewport(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y);


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
		framebuffeffects->GodRays(glm::vec3(-8.f, 9.f, 10.f), model->pos, Camera::getInstance().forward());
		framebuffeffects->Bloom(4);
	}
	else if (gameState == MENU || VICTORYSCREEN)
	{
		framebuffeffects->Toon(true);
		framebuffeffects->Bloom(4);
	}
	//if (numpadPress[1])

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
		if (showHighScores)
		{
			TextRendering::getInstance().printText2D("S N D", -0.7f, 0.875f, 0.09f, menuItem1Colour, red);
			for (int i = 0, s = scoreTable.size(); i < s && i < 30; i++)
			{
				char buffer[64];
				_snprintf_s(buffer, 64, "%s %i", sndNames[i].c_str(), scoreTable[i]);
				TextRendering::getInstance().printText2D(buffer, -0.85f, 0.75f - i / 16.f, 0.075f, menuItem1Colour, red);
			}

			TextRendering::getInstance().printText2D("SURVIVAL", 0.15f, 0.875f, 0.09f, menuItem1Colour, red);
			for (int i = 0, s = survivalScoreTable.size(); i < s && i < 30; i++)
			{
				char buffer[64];
				_snprintf_s(buffer, 64, "%s %i", survivalNames[i].c_str(), survivalScoreTable[i]);
				TextRendering::getInstance().printText2D(buffer, 0.27f, 0.75f - i / 16.f, 0.075f, menuItem1Colour, red);
			}
		}
		else
		{
			//startscreen->render();
			gameLogo->render();
			studioLogo->render();
			if (currentlySelectedMenuItem == SEARCHANDDESTROY)
			{
				TextRendering::getInstance().printText2D("SEARCH AND DESTROY", -0.95f, 0.3f, 0.09f, menuItem2Colour, menuItem1Colour);
				TextRendering::getInstance().printText2D("SURVIVAL", -0.95f, 0.2f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("HIGHSCORES", -0.95, 0.1f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("EXIT", -0.95f, 0.0f, 0.09f, glm::vec3(0));
			}
			if (currentlySelectedMenuItem == SURVIVAL)
			{
			

				TextRendering::getInstance().printText2D("SEARCH AND DESTROY", -0.95f, 0.3f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("SURVIVAL", -0.95f, 0.2f, 0.09f, menuItem2Colour, menuItem1Colour);
				TextRendering::getInstance().printText2D("HIGHSCORES", -0.95, 0.1f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("EXIT", -0.95f, 0.0f, 0.09f, glm::vec3(0));
			}
			if (currentlySelectedMenuItem == HIGHSCORES)
			{
				TextRendering::getInstance().printText2D("SEARCH AND DESTROY", -0.95f, 0.3f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("SURVIVAL", -0.95f, 0.2f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("HIGHSCORES", -0.95, 0.1f, 0.09f, menuItem2Colour, menuItem1Colour);
				TextRendering::getInstance().printText2D("EXIT", -0.95f, 0.0f, 0.09f, glm::vec3(0));
			}
			if (currentlySelectedMenuItem == EXIT)
			{
				TextRendering::getInstance().printText2D("SEARCH AND DESTROY", -0.95f, 0.3f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("SURVIVAL", -0.95f, 0.2f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("HIGHSCORES", -0.95, 0.1f, 0.09f, glm::vec3(0));
				TextRendering::getInstance().printText2D("EXIT", -0.95f, 0.0f, 0.09f, menuItem2Colour, menuItem1Colour);
			}
		
		//if (score != 0)
		//{
		//	char buffer[64];
		//	_snprintf_s(buffer, 64, "SCORE:%i", score);
		//	TextRendering::getInstance().printText2D(buffer, -0.38f, 0.85f, 0.075f, fontColour);
		//}
		}
	}
	else if (gameState == GAME)
	{
		crosshair->render();
		skull->render();
		if (isPlayingSearchAndDestroy)
        {
            char buffer[8];
            _snprintf_s(buffer, 8, "%i/%i", targetsKilled, maxNumOfTargets);
            TextRendering::getInstance().printText2D(buffer, -0.70f, -0.8f, 0.125f, fontColour);
        }
        else
        {
            char buffer[8];
            _snprintf_s(buffer, 8, "%i", enemiesAlive);
            TextRendering::getInstance().printText2D(buffer, -0.70f, -0.8f, 0.125f, fontColour);
        }
		if (isPlayingSearchAndDestroy)
		{
			char scbuff[64];
			_snprintf_s(scbuff, 64, "SCORE:%i", score);
			TextRendering::getInstance().printText2D(scbuff, -0.49f, 0.91f, 0.075f, fontColour);
		}
		if (isPlayingSearchAndDestroy)
        {
            if (openingMessageTimer >= 0.0f)
            {
                char opMessageBuff[] = "DESTROY ALL ENEMY MEKS";
                TextRendering::getInstance().printText2D(opMessageBuff, -0.9, 0.3, 0.085, glm::mix(fontColour, white, openningMessageInterp));
            }
        }
		else
		{
			if (openingMessageTimer >= 0.0f)
			{
				if (openingMessageTimer >= 0.0f)
				{
					char opMessageBuff[] = "SURVIVE";
					TextRendering::getInstance().printText2D(opMessageBuff, -0.3, 0.3, 0.1, glm::mix(fontColour, white, openningMessageInterp));
				}
			}
		}
		//char amBuff[8];
		//_snprintf_s(amBuff, 8, "AMMO:%i", *currentAmmo);
		//if (*currentAmmo > 0)
		//	TextRendering::getInstance().printText2D(amBuff, 0.3f, -0.8f, 0.1f, glm::mix(red, glm::normalize(fontColour), *currentAmmo / 10.f));
		//else if (*currentAmmo == 0)
		//	TextRendering::getInstance().printText2D(amBuff, 0.3f, -0.8f, 0.1f, glm::mix(red, white, noammoInterp));
		ShieldBack->render();
		ShieldFront->cutoffPercent(shieldHealth / maxShieldHealth);
		ShieldFront->render();
		HPback->render();
		HPFront->cutoffPercent(model->health / 100.f);
		HPFront->render();
		machineIcon->render();
		shotgunIcon->render();
		bfgIcon->render();
		iconGlow->render();
		dashIconBack->render();
        if (!dashingHitZero)
            dashIconFront->cutoffPercent(dashingCooldown / maxDashingCooldown);
        else
            dashIconFront->cutoffPercent(0);
        dashIconFront->render();
        bulletTimeIconBack->render();
        if (!bulletTimeHitZero)
            bulletTimeIconFront->cutoffPercent(bulletTimeCooldown / maxBulletTimeCooldown);
        else
            bulletTimeIconFront->cutoffPercent(0);
        bulletTimeIconFront->render();

	}
	else if (gameState = VICTORYSCREEN)
	{
		if (isPlayingSearchAndDestroy)
		{
			char victoryAndScore[] = "VICTORY! YOUR SCORE: ";
			if (enemiesAlive > 1)
			{
				victoryAndScore[0] = 'D';
				victoryAndScore[1] = 'E';
				victoryAndScore[2] = 'F';
				victoryAndScore[3] = 'E';
				victoryAndScore[4] = 'A';
				victoryAndScore[5] = 'T';
				victoryAndScore[6] = '.';
				victoryAndScore[7] = ' ';
			}
			TextRendering::getInstance().printText2D(victoryAndScore, -0.9f, 0.5f, 0.07f, menuItem1Colour, red);
			char scbuff[64];
			_snprintf_s(scbuff, 64, "%i", score);
			TextRendering::getInstance().printText2D(scbuff, 0.5f, 0.5f, 0.07f, menuItem1Colour, red);
		}
		else
		{
			char victoryAndScore[] = "TARGETS KILLED: ";
			TextRendering::getInstance().printText2D(victoryAndScore, -0.6f, 0.5f, 0.07f, menuItem1Colour, red);
			char scbuff[64];
			_snprintf_s(scbuff, 64, "%i", targetsKilled);
			TextRendering::getInstance().printText2D(scbuff, 0.55f, 0.5f, 0.07f, menuItem1Colour, red);
		}
		char name[4];
		name[0] = 'A' + nameOffset[0];
		name[1] = 'A' + nameOffset[1];
		name[2] = 'A' + nameOffset[2];
		name[3] = '\0';
		switch (nameSelect)
		{
		case 0:
			characterSpaceInterpIsIncreasing ? name[0] = ' ' : false;
			break;
		case 1:
			characterSpaceInterpIsIncreasing ? name[1] = ' ' : false;
			break;
		case 2:
			characterSpaceInterpIsIncreasing ? name[2] = ' ' : false;
			break;
		default:
			break;
		}
		TextRendering::getInstance().printText2D(name, -0.4, 0.0f, 0.333f, menuItem1Colour, red);
	}
	glEnable(GL_DEPTH_TEST);

	glfwSwapBuffers(gWindow);
}
#define SHOT_CD 0.1
float shotcd = 0;	
// update the scene based on the time elapsed since last update
static void Update(float secondsElapsed) {

	if (HIJACKCONSOLE){
		manager->GetSoundManager()->FindSound("Background", "two")->PrintSoundInformation();
		//manager->DisplaySystemNode();
	}
	Tutorial();
	

 	runTime += secondsElapsed;

	glm::vec3 lInput;
	glm::vec2 rInput;
	Camera* cam = &Camera::getInstance();

	glm::vec3 f = cam->forward();
	glm::vec3 r = cam->right();
	glm::vec3 fmy = cam->forward();
	fmy.y = 0;
	bool shoot = false;

	glm::vec3 ppos = cam->position();
	manager->SetSystemPFU(cam->position(),cam->forward(),cam->up());
	manager->GetSoundManager()->GetOwnerList("Player")->UpdateListPosition(cam->position());
	manager->Update();
	
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

		if (glfwGetKey(gWindow, 'Q'))
			shieldHealth = maxShieldHealth;

		IsDashing = false;
		if (glfwGetKey(gWindow, ' ') && dashingHitZero == false && gameState == GAME)
		{
			spaceHasBeenPressed = true;
			IsDashing = true;
		}
		if (!IsDashing)
		{
			dashingCooldown += secondsElapsed / 5;
			if (dashingCooldown > maxDashingCooldown)
			{
				dashingCooldown = maxDashingCooldown;
				dashingHitZero = false;
			}
		}
		if (IsDashing && dashingCooldown >= 0.0f)
		{
			dashingCooldown -= secondsElapsed;
		}
		if (dashingCooldown < 0.0f)
		{
			IsDashing = false;
			dashingHitZero = true;
		}
		//std::cout << dashingCooldown << std::endl;

		if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT))
		{
			shoot = true;
		}
		if (glfwGetKey(gWindow, 'R') && *currentAmmo < 10 && *currentReloadTimer == 0.0f)
			*currentReloadTimer = 0.0001f;
		if (*currentAmmo <= 0 && *currentReloadTimer == 0.0f)
		{
			*currentReloadTimer = 0.0001f;
		}

		if (glfwGetKey(gWindow, '1'))
		{
			currentWeapon = machineGun;
			currentAmmo = &machineGunAmmo;
			currentReloadTimer = &machineGunReloadTimer;
			iconGlow->pos = glm::vec3(-0.2f, -0.85f, 4);
		}
		if (glfwGetKey(gWindow, '2'))
		{
			currentWeapon = shotgun;
			currentAmmo = &shotgunAmmo;
			currentReloadTimer = &shotgunReloadTimer;
			iconGlow->pos = glm::vec3(0.0f, -0.85f, 4);
		}
		if (glfwGetKey(gWindow, '3'))
		{
			currentWeapon = bfg;
			currentAmmo = &bfgAmmo;
			currentReloadTimer = &bfgReloadTimer;
			iconGlow->pos = glm::vec3(0.2f, -0.85f, 4);
		}
		if (glfwGetKey(gWindow, 'P'))
		{
			if (BackPaused){
				manager->GetSoundManager()->PauseSound("Background", "one",false);
				BackPaused = false;
			}
			else{
				manager->GetSoundManager()->PauseSound("Background", "one",true);
				BackPaused = true;
			}
		}
		if (glfwGetKey(gWindow, 'O'))
		{
			
		}
		if (glfwGetKey(gWindow, 'I'))
		{
			RunWasd = true;
			RunShield = true;
			RunEnemy = true;
			RunDash = true;
			RunSlowTime = true;
		}

		isUsingBulletTime = false;
		if (glfwGetKey(gWindow, GLFW_KEY_LEFT_SHIFT) && bulletTimeHitZero == false)
		{
			isUsingBulletTime = true;
		}
		if (isUsingBulletTime)
		{
			manager->GetSoundManager()->FastForwardAll(true);
			bulletTimeCooldown -= secondsElapsed;
			if (bulletTimeCooldown <= 0.0f)
			{
				bulletTimeCooldown = 0.0f;
				bulletTimeHitZero = true;
			}
		}
		else if (!isUsingBulletTime)
		{
			manager->GetSoundManager()->FastForwardAll(false);
			bulletTimeCooldown += secondsElapsed / 2;
			if (bulletTimeCooldown > maxBulletTimeCooldown)
			{
				bulletTimeCooldown = maxBulletTimeCooldown;
				bulletTimeHitZero = false;
			}
		}
		//std::cout << bulletTimeCooldown << std::endl;




		//rotate camera based on mouse movement
		float mouseSensitivity = 0.05f;
		zoomingIn = false;
		if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) && gameState == GAME)
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
		if (openningMessageInterpIsIncreasing)
		{
			openningMessageInterp += secondsElapsed;
			if (openningMessageInterp >= 1.0f)
			{
				openningMessageInterp = 1.0f;
				openningMessageInterpIsIncreasing = false;
			}
		}
		else if (!openningMessageInterpIsIncreasing)
		{
			openningMessageInterp -= secondsElapsed;
			if (openningMessageInterp <= 0.0f)
			{
				openningMessageInterp = 0.0f;
				openningMessageInterpIsIncreasing = true;
			}
		}
		if (openingMessageTimer >= 0.0f)
			openingMessageTimer -= secondsElapsed;

		//Begin Camera code
		model->pos += fmy * (((c->getOwner()->vel + (IsDashing ? 0.2f : 0.0f)) ) * lInput.z) + model->force;
		model->force = model->force / 1.2f;
		if (glm::length(model->force) < 0.1f)
			model->force = glm::vec3(0);
		model->pos += cam->right() * (((c->getOwner()->vel + (IsDashing ? 0.2f : 0.0f))) * lInput.x);

		cam->offsetPosition(model->pos - cam->position());

//<>*********************MORE SOUND ***************************************
		if (ppos != cam->position()){
			manager->GetSoundManager()->FindSound("Player", "Walk")->PauseSound(false);
		}
		else{
			manager->GetSoundManager()->FindSound("Player", "Walk")->PauseSound(true);
		}
//</>*********************MORE SOUND ***************************************



		c->getOwner()->dir = glm::rotateX(c->getOwner()->dir, -rInput.y);
		c->getOwner()->dir = glm::rotateY(c->getOwner()->dir, rInput.x);
		cam->offsetOrientation(-rInput.y, rInput.x);
		

		playerArms->getOwner()->pos = Camera::getInstance().position();
		playerArms->getOwner()->pos += Camera::getInstance().forward() * 0.34f;
		playerArms->getOwner()->pos -= Camera::getInstance().up() * 1.1f;
		playerArms->getOwner()->dir = glm::rotateY(Camera::getInstance().right(), 90.f);
		float angle = glm::angle(playerArms->getOwner()->dir, glm::vec3(0.f, 0.f, -1.f));
		float rightAngle = glm::dot(playerArms->getOwner()->dir, glm::vec3(-1.f, 0.f, 0.f));
		glm::mat4 rotation;
		if (rightAngle >= 0)
			rotation = glm::mat4(glm::rotate(angle + 90.f, glm::vec3(0, 1, 0)));
		else
		{
			rotation = glm::mat4(glm::rotate(-angle + 90.f, glm::vec3(0, 1, 0)));
			angle = -angle;
		}
		playerArms->rotMatrix = rotation;

		playerArms->getOwner()->dir = glm::rotateY(Camera::getInstance().forward(), -angle);
		float angle2 = glm::angle(playerArms->getOwner()->dir, glm::vec3(0.f, 0.f, -1.f));
		float rightAngle2 = glm::dot(playerArms->getOwner()->dir, glm::vec3(0.f, -1.f, 0.f));
		//glm::mat4 rotation2;
		if (rightAngle2 >= 0)
			rotation = rotation * glm::mat4(glm::rotate(-angle2, glm::vec3(0, 0, 1)));
		else
			rotation = rotation * glm::mat4(glm::rotate(angle2, glm::vec3(0, 0, 1)));
		playerArms->rotMatrix = rotation;// *glm::mat4(glm::rotate(180.f, glm::vec3(0.f, 1.0f, 0.f)));
		//playerArms->rotMatrix = glm::rotate(glm::mat4(), -Camera::getInstance()._horizontalAngle + 90.f, Camera::getInstance().up());
		//playerArms->rotMatrix = glm::rotate(glm::mat4(), Camera::getInstance()._horizontalAngle, Camera::getInstance().forward());
		ammoPlane->pos = Camera::getInstance().forward() * 0.5f - Camera::getInstance().up() * 0.15f + Camera::getInstance().right() * 0.22f + Camera::getInstance().position();
		gAmmoPlane->rotMatrix = rotation * glm::mat4(glm::rotate(glm::mat4(), 80.f, glm::vec3(0, 0, 1)));


		//End Camera code

		for (int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
			ObjectManager::instance().pMap[i]->update(secondsElapsed, isUsingBulletTime);
		for (unsigned int i = 0, s = ObjectManager::instance().enemyPMap.size(); i < s; i++)
			ObjectManager::instance().enemyPMap[i]->update(secondsElapsed, isUsingBulletTime);

		glm::vec3 p = Camera::getInstance().position();
		if (shoot && shotcd > SHOT_CD && *currentAmmo > 0 && *currentReloadTimer == 0.0f)
		{
			Projectile* pr;

			if (currentWeapon == machineGun)
			{
				p = glm::vec3(rotation * glm::vec4((leftArmFiringPosition * 0.1f), 1.0)) + p;
				pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.00f, 0.030f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.00f, 0.03f))), 25, 25, 10, manager->GetSoundManager()->FindSound("Player", "Projectile"));
				pr->go->scale = glm::vec3(0.3f); 
				ObjectManager::instance().pMap.push_back(pr);
				*currentAmmo = *currentAmmo - 1;
			}
			else if (currentWeapon == shotgun)
			{
				if (*currentAmmo >= shotgun)
				{
					p = glm::vec3(rotation * glm::vec4((rightArmFiringPosition * 0.1f), 1.0)) + p;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 30, 15, 10, manager->GetSoundManager()->FindSound("Player", "Projectile"));
					pr->go->scale = glm::vec3(0.3f);
					ObjectManager::instance().pMap.push_back(pr);
					*currentAmmo = *currentAmmo - 1;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 30, 15, 10, manager->GetSoundManager()->FindSound("Player", "Projectile"));
					pr->go->scale = glm::vec3(0.3f);
					ObjectManager::instance().pMap.push_back(pr);
					*currentAmmo = *currentAmmo - 1;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 30, 15, 10, manager->GetSoundManager()->FindSound("Player", "Projectile"));
					pr->go->scale = glm::vec3(0.3f);
					ObjectManager::instance().pMap.push_back(pr);
					*currentAmmo = *currentAmmo - 1;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 30, 15, 10, manager->GetSoundManager()->FindSound("Player", "Projectile"));
					pr->go->scale = glm::vec3(0.3f);
					ObjectManager::instance().pMap.push_back(pr);
					*currentAmmo = *currentAmmo - 1;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.2f, 0.2f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.2f, 0.2f))), 30, 15, 10, manager->GetSoundManager()->FindSound("Player", "Projectile"));
					pr->go->scale = glm::vec3(0.3f);
					ObjectManager::instance().pMap.push_back(pr);
					*currentAmmo = *currentAmmo - 1;
				}
			}
			else if (currentWeapon == bfg)
			{
				if (*currentAmmo >= bfg)
				{
					p = glm::vec3(rotation * glm::vec4((rightArmFiringPosition * 0.1f), 1.0)) + p;

					pr = new Projectile(p, glm::normalize(f + glm::vec3(randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.015f, 0.015f), randomClampedFloat(-0.015f, 0.015f))), 25, 80, 10, manager->GetSoundManager()->FindSound("Player", "rpg"));
					pr->go->scale = glm::vec3(1.4f);
					ObjectManager::instance().pMap.push_back(pr);
					*currentAmmo -= bfg;
				}
			}



			shotcd = 0;

		}

		if (machineGunReloadTimer > 0.0f)
		{
			if (!manager->GetSoundManager()->FindSound("Player", "sreload")->GetIsPlaying()){
				manager->GetSoundManager()->FindAndPlay("Player", "sreload");
			}
			machineGunReloadTimer += secondsElapsed;
			if (machineGunReloadTimer > 2.f)
			{
				machineGunReloadTimer = 0.0f;
				machineGunAmmo = 10;
				noammoInterp = 0.0f;
			}
		}
		if (shotgunReloadTimer > 0.0f)
		{
			if (!manager->GetSoundManager()->FindSound("Player", "mreload")->GetIsPlaying()){
				manager->GetSoundManager()->FindAndPlay("Player", "mreload");
			}
			shotgunReloadTimer += secondsElapsed;
			if (shotgunReloadTimer > 2.f)
			{
				shotgunReloadTimer = 0.0f;
				shotgunAmmo = 10;
				noammoInterp = 0.0f;
			}
		}
		if (bfgReloadTimer > 0.0f)
		{
			bfgReloadTimer += secondsElapsed;
			if (bfgReloadTimer > 2.f)
			{
				bfgReloadTimer = 0.0f;
				bfgAmmo = 10;
				noammoInterp = 0.0f;
			}
		}

		if (*currentAmmo == 0)
		{
			if (noammoInterpIsIncreasing)
			{
				noammoInterp += secondsElapsed;
				if (noammoInterp >= 1.0f)
				{
					noammoInterp = 1.0f;
					noammoInterpIsIncreasing = !noammoInterpIsIncreasing;
				}
			}
			else if (!noammoInterpIsIncreasing)
			{
				noammoInterp -= secondsElapsed;
				if (noammoInterp <= 0.0f)
				{
					noammoInterp = 0.0f;
					noammoInterpIsIncreasing = !noammoInterpIsIncreasing;
				}
			}
		}
		dshield += secondsElapsed;
		hitTimer += secondsElapsed;
		if (playerIsHit){
			if (hitTimer > 0.5 && shieldHealth>1){
				hitTimer = 0;
				manager->GetSoundManager()->FindAndPlay("Player", "ShieldHit");
				dShield = false;
				if (!PTUT){
					if (RunShield){
						RunShield = false;
					//	SManager->SoundVolumeAll(0.25);
						//SManager->FindSound("Tutorial", "shield")->SetVolume(1);
						manager->GetSoundManager()->FindAndPlay("Tutorial", "shield");
					}
					if (!manager->GetSoundManager()->FindSound("Tutorial", "shield")->GetIsPlaying()){
						//SManager->SoundVolumeAll(1);
					}
				}
			}
			if (hitTimer > 0.5 && shieldHealth < 0){
				dShield = true;
				manager->GetSoundManager()->FindAndPlay("Player", "NoShield");
				if (!PTUT){
					if (RunSlowTime){
						RunSlowTime = false;
						//SManager->SoundVolumeAll(0.25);
						//SManager->FindSound("Tutorial", "slowtime")->SetVolume(1);
						manager->GetSoundManager()->FindAndPlay("Tutorial", "slowtime");
					}
					if (!manager->GetSoundManager()->FindSound("Tutorial", "slowtime")->GetIsPlaying()){
						//SManager->SoundVolumeAll(1);
					}
				}

			}
			if (dShield = true && dshield >5){
				manager->GetSoundManager()->FindAndPlay("Player", "ShieldWarning");
				dShield = false;
				dshield = 0;
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

		enemiesAlive = 0;
		for (int i = 0, s = targets.size(); i < s; i++)
		{
			if (targets[i]->canSeePlayer(model->pos)){
				if (RunEnemy){
					RunEnemy = false;
					//SManager->SoundVolumeAll(0.25);
					//SManager->FindSound("Tutorial", "enemy")->SetVolume(1);
					manager->GetSoundManager()->FindAndPlay("Tutorial", "enemy");
					if (!manager->GetSoundManager()->FindSound("Tutorial", "enemy")->GetIsPlaying()){
						//SManager->SoundVolumeAll(1);
					}
				}
			}
			
			if (targets[i]->alive == true)
			{
				targets[i]->currentMaxVelocity = targets[i]->maxVelocity;
				if (isUsingBulletTime)
				{
					targets[i]->currentMaxVelocity = targets[i]->maxVelocity * timeFactor;
				}
				targets[i]->update(secondsElapsed / 5, testNaveMesh);
				targets[i]->go->pos.y = ground->HeightAtLocation(targets[i]->go->pos) - 0.05; //this moves the targets to the correct position above the ground.

				if (targets[i]->hasSpottedPlayer == false && targets[i]->alive)
				{
					targets[i]->canSeePlayer(model->pos);
				}
				if (targets[i]->go->health < 100)
					targets[i]->hasSpottedPlayer = true;
				if (targets[i]->hasSpottedPlayer)
				{
					targets[i]->go->dir = glm::normalize(model->pos - targets[i]->go->pos);
					targets[i]->hvygo->dir = targets[i]->go->dir;
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
				if (targets[i]->go->health <= 0 && targets[i]->alive)
				{
					targets[i]->hit = true;
				}
				if (targets[i]->hit && targets[i]->alive)
				{
					targets[i]->alive = false;
					targets[i]->go->pos = glm::vec3(50000.f);
					targets[i]->hvygo->pos = glm::vec3(50000.f);
					skull->play();
					targetsKilled++;
				}

				targets[i]->fireTimer += secondsElapsed * (isUsingBulletTime ? timeFactor : 1.0f);
				if (targets[i]->fireTimer >= targets[i]->fireTimeTolerance && targets[i]->alive == true && targets[i]->hasSpottedPlayer)
				{
					targets[i]->fireTimer = 0.f;
					if (targets[i]->enemyType == 0)
					{
						if (targets[i]->firingfromRightBarrel)
							targets[i]->weaponProjectile = new Projectile(targets[i]->go->pos + targets[i]->rightGunBarrel, glm::normalize((model->pos - targets[i]->go->pos) + glm::vec3(randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f))), 20, 20, 4, manager->GetSoundManager()->FindSound("Player", "Projectile"));
						else
							targets[i]->weaponProjectile = new Projectile(targets[i]->go->pos + targets[i]->leftGunBarrel, glm::normalize((model->pos - targets[i]->go->pos) + glm::vec3(randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f))), 20, 20, 4, manager->GetSoundManager()->FindSound("Player", "Projectile"));
						targets[i]->weaponProjectile->go->scale = glm::vec3(1.5);
					}
					else if (targets[i]->enemyType == 1)
					{
						if (targets[i]->firingfromRightBarrel)
							targets[i]->weaponProjectile = new Projectile(targets[i]->go->pos + targets[i]->rightGunBarrel, glm::normalize((model->pos - targets[i]->go->pos) + glm::vec3(randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f))), 10, 50, 4, manager->GetSoundManager()->FindSound("Player", "Projectile"));
						else
							targets[i]->weaponProjectile = new Projectile(targets[i]->go->pos + targets[i]->leftGunBarrel, glm::normalize((model->pos - targets[i]->go->pos) + glm::vec3(randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f), randomClampedFloat(-1.5f, 1.5f))), 10, 50, 4, manager->GetSoundManager()->FindSound("Player", "Projectile"));
						targets[i]->weaponProjectile->go->scale = glm::vec3(0.9f);
					}
					targets[i]->firingfromRightBarrel = !targets[i]->firingfromRightBarrel;
					targets[i]->weaponProjectile->go->scale = glm::vec3(1.1f);
					targets[i]->weaponProjectile->go->SetName("EnemyProjectile");
					targets[i]->weaponProjectile->handle = ObjectManager::instance().enemyPMap.size();
					ObjectManager::instance().enemyPMap.push_back(targets[i]->weaponProjectile);
					if (targets[i]->enemyType == 0)
					{
						targets[i]->fireTimeTolerance = randomClampedFloat(0.5f, 2.f);
					}
					else if (targets[i]->enemyType == 1)
					{
						targets[i]->fireTimeTolerance = randomClampedFloat(3.f, 4.f);
					}
				}
				enemiesAlive++;
			}
		}
	    if (targetSpawner1.isActive)
        {
			targetSpawner1.update(targets, isUsingBulletTime ? secondsElapsed * timeFactor : secondsElapsed);
			targetSpawner2.update(targets, isUsingBulletTime ? secondsElapsed * timeFactor : secondsElapsed);
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

	    if (isPlayingSearchAndDestroy)
        {
            if (targetsKilled == maxNumOfTargets || c->IsPressed(XINPUT_GAMEPAD_BACK))
                wonGame();
        }
        else
        {
            TargetNumberIncreaseTime -= secondsElapsed * (isUsingBulletTime ? timeFactor : 1.0f);
            if (TargetNumberIncreaseTime <= 0.0f)
            {
                TargetNumberIncreaseTime = maxTargetNumberIncreaseTimer;
                maxNumOfTargets += 1;
                if (maxNumOfTargets > 50)
                    maxNumOfTargets = 50;
            }
        }


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
		    if (glfwGetKey(gWindow, '1'))
            {
                maxNumOfTargets = 1;
                isPlayingSearchAndDestroy = true;
                startGame();
            }
            if (glfwGetKey(gWindow, '2'))
            {
                maxNumOfTargets = 6;
                isPlayingSearchAndDestroy = false;
                startGame();
            }
			if (glfwGetKey(gWindow, 'W') && currentlyPressedKey == 'n')
			{
				currentlyPressedKey = 'w';
				currentlySelectedMenuItem -= 1;
				if (currentlySelectedMenuItem < 0)
					currentlySelectedMenuItem = maxChoices - 1;
			}
			else if (glfwGetKey(gWindow, 'S') && currentlyPressedKey == 'n')
			{
				currentlyPressedKey = 's';
				currentlySelectedMenuItem += 1;
				if (currentlySelectedMenuItem >= maxChoices)
					currentlySelectedMenuItem = 0;
			}

			if (glfwGetKey(gWindow, 'W') == GLFW_RELEASE && currentlyPressedKey == 'w')
			{
				currentlyPressedKey = 'n';
			}
			else if (glfwGetKey(gWindow, 'S') == GLFW_RELEASE && currentlyPressedKey == 's')
			{
				currentlyPressedKey = 'n';
			}

			if (glfwGetKey(gWindow, GLFW_KEY_ENTER) || spaceHasBeenPressed ? false : glfwGetKey(gWindow, GLFW_KEY_SPACE))
			{
				switch (currentlySelectedMenuItem)
				{
				case 0:
					maxNumOfTargets = 6;
					isPlayingSearchAndDestroy = true;
					startGame();
					break;
				case 1:
					maxNumOfTargets = 6;
					isPlayingSearchAndDestroy = false;
					startGame();
					break;
				case 2:
					showHighScores = true;
					break;
				case 3:
					glfwSetWindowShouldClose(gWindow, GL_TRUE);
					break;
				}
			}
			if (glfwGetKey(gWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
			{
				spaceHasBeenPressed = false;
			}
		}
	}
	else if (gameState == VICTORYSCREEN)
	{
		cam->lookAt(glm::vec3(1000, 0, 0));
		cam->offsetPosition(cam->right() * 0.1f);

		if ((glfwGetKey(gWindow, 'W')) && currentlyPressedKey == 'n')
		{
			nameOffset[nameSelect] -= 1;
			if (nameOffset[nameSelect] < 0)
				nameOffset[nameSelect] = 25;
			currentlyPressedKey = 'w';
			characterSpaceInterp = 0.7f;
			characterSpaceInterpIsIncreasing = false;
			
		}
		if ((glfwGetKey(gWindow, 'S')) && currentlyPressedKey == 'n')
		{
			nameOffset[nameSelect] += 1;
			if (nameOffset[nameSelect] > 25)
				nameOffset[nameSelect] = 0;
			currentlyPressedKey = 's';
			characterSpaceInterp = 0.7f;
			characterSpaceInterpIsIncreasing = false;
		}

		if ((glfwGetKey(gWindow, 'A')) && currentlyPressedKey == 'n')
		{
			nameSelect -= 1;
			if (nameSelect < 0)
				nameSelect = 2;
			currentlyPressedKey = 'a';
		}
		if ((glfwGetKey(gWindow, 'D')) && currentlyPressedKey == 'n')
		{
			nameSelect += 1;
			if (nameSelect > 2)
				nameSelect = 0;
			currentlyPressedKey = 'd';
		}

		if (glfwGetKey(gWindow, 'W') == GLFW_RELEASE && currentlyPressedKey == 'w')
		{
			currentlyPressedKey = 'n';
		}
		if (glfwGetKey(gWindow, 'S') == GLFW_RELEASE && currentlyPressedKey == 's')
		{
			currentlyPressedKey = 'n';
		}
		if (glfwGetKey(gWindow, 'A') == GLFW_RELEASE && currentlyPressedKey == 'a')
		{
			currentlyPressedKey = 'n';
		}
		if (glfwGetKey(gWindow, 'D') == GLFW_RELEASE && currentlyPressedKey == 'd')
		{
			currentlyPressedKey = 'n';
		}

		if (glfwGetKey(gWindow, GLFW_KEY_ENTER) || glfwGetKey(gWindow, GLFW_KEY_SPACE))
		{
			gameState = MENU;
			spaceHasBeenPressed = true;
			if (isPlayingSearchAndDestroy)
			{
				string tempName;
				tempName.push_back('A' + nameOffset[0]);
				tempName.push_back('A' + nameOffset[1]);
				tempName.push_back('A' + nameOffset[2]);
				sndNames.insert(sndNames.begin() + scoreInsertionIndex, tempName);
			}
			else
			{
				string tempName;
				tempName.push_back('A' + nameOffset[0]);
				tempName.push_back('A' + nameOffset[1]);
				tempName.push_back('A' + nameOffset[2]);
				survivalNames.insert(survivalNames.begin() + scoreInsertionIndex, tempName);
			}
		}
		if (characterSpaceInterpIsIncreasing)
			characterSpaceInterp += secondsElapsed;
		else if (!characterSpaceInterpIsIncreasing)
			characterSpaceInterp -= secondsElapsed;

		if (characterSpaceInterp > 0.5f)
			characterSpaceInterpIsIncreasing = false;
		else if (characterSpaceInterp < 0.f)
			characterSpaceInterpIsIncreasing = true;
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
	InitManager();
	testNaveMesh.loadNavMesh("../Debug/models/NavMeshes/FirstLevelNavMesh-scaled.obj");
	srand(time(NULL));
	// initialise GLFW
	glfwSetErrorCallback(OnError);
	if (!glfwInit())
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
	while (glGetError() != GL_NO_ERROR) {}

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
	if (!GLEW_VERSION_4_3)
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
	loadScoreBoard(scoreTable, sndNames, survivalScoreTable, survivalNames);

	framebuff[0] = new Framebuffer();
	framebuff[0]->CreateDepthTexture(SCREEN_SIZE.x, SCREEN_SIZE.y);
	framebuff[0]->CreateColorTexture(4, SCREEN_SIZE.x, SCREEN_SIZE.y);
	framebuff[1] = new Framebuffer();
	framebuff[1]->CreateDepthTexture(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	framebuff[1]->CreateColorTexture(1, SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	framebuff[2] = new Framebuffer();
	framebuff[2]->CreateDepthTexture(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	framebuff[2]->CreateColorTexture(1, SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	framebuff[3] = new Framebuffer();
	framebuff[3]->CreateDepthTexture(SCREEN_SIZE.x, SCREEN_SIZE.y);
	framebuff[3]->CreateColorTexture(4, SCREEN_SIZE.x, SCREEN_SIZE.y);
	framebuff[4] = new Framebuffer();
	framebuff[4]->CreateDepthTexture(256, 256);
	framebuff[4]->CreateColorTexture(1, 256, 256);
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
	skull = new twodOverlayAnim("killSkull.png", 5, 0.2);
	ShieldBack = new twodOverlay("ShieldBarBackV3.png", 0, 0.85, 35);
	ShieldFront = new twodOverlay("ShieldBarMeasure2.png", 0, 0.85, 35);
	gameLogo = new twodOverlay("MainuMenu_Title.png", -0.7f, 0.5f, 10.0f);
	studioLogo = new twodOverlay("MainMenu_Logo.png", 0.83f, -0.7f, 5.0f);
	HPback = new twodOverlay("HPBarBack.png", 0, 0.84, 35);
	HPFront = new twodOverlay("HPBarMeasure2.png", 0, 0.84, 35);
	startscreen = new twodOverlay("pressStart.png", 0, 0, 10);
	machineIcon = new twodOverlay("Machine Gun Icon.png", -0.2f, -0.85f, 4);
	shotgunIcon = new twodOverlay("Shotgun Icon.png", 0, -0.85, 4);
	bfgIcon = new twodOverlay("Slug Shell Icon.png", 0.2f, -0.85f, 4);
	iconGlow = new twodOverlay("Under Glow Icon.png", -0.2f, -0.85f, 4);
	dashIconBack = new twodOverlay("Dash Not Active.png", -0.68f, 0.65f, 5);
	dashIconFront = new twodOverlay("Dash Active.png", -0.68f, 0.65f, 5);
	bulletTimeIconBack = new twodOverlay("Time Not Active.png", 0.68f, 0.65f, 5);
	bulletTimeIconFront = new twodOverlay("Time Active.png", 0.68f, 0.65f, 5);

	skull->updatePos(-0.85f, -0.75f, 4);
	skull->cycle = true;

	ground = new Terrain();
	ground->LoadHeightMap("testhm.png", 1, 5, 0.8);
	ground->InitRender();
	char* sb[6] = { "SkyBoxRight.png", "SkyBoxRight.png", "SkyBoxTop.png", "SkyBoxBottom.png", "SkyBoxRight.png", "SkyBoxRight.png" };
	//char* sb[6] = { "SkyBoxTest_Right.png", "SkyBoxTest_Left.png", "SkyBoxTest_Top.png", "SkyBoxTest_Bottom.png", "SkyBoxTest_Back.png", "SkyBoxTest_Front.png" };
	//char* Osb[6] = { "Right V3", "Left V3.png", "Top V2.png", "Top V3.png", "Back V3.png", "fr-O.png" };
	char* Osb[6] = { "JustABlackImage.png", "JustABlackImage.png", "JustABlackImage.png", "JustABlackImage.png", "SunYellow.png", "JustABlackImage.png" };
	char* Ssb[6] = { "SkyBoxTest_Right.png", "SkyBoxTest_Left.png", "SkyBoxTest_Top.png", "SkyBoxTest_Bottom.png", "SkyBoxTest_Back.png", "SkyBoxTest_Front.png" };
	sky = new Skybox(sb, Osb, Ssb);

	//skyObs = new Skybox(Osb);
	//MODEL INITS

	prepProjectiles();

	model = new GameObject(0);
	model->SetName("Player");
	gModel = new Model();
	gModel->setOwner(model);
	gModel->loadModel("models/TallCube.dae");
	arms = new GameObject(616);
	arms->SetName("PlayerArms");
	playerArms = new Model();
	playerArms->setOwner(arms);
	playerArms->loadModel("models/MEchArms2.dae");
	arms->AddComponent(GRAPHICS, playerArms);

	ammoPlane = new GameObject(617);
	ammoPlane->SetName("PlayerArms");
	gAmmoPlane = new Model();
	gAmmoPlane->setOwner(ammoPlane);
	gAmmoPlane->loadModel("models/AmmoPlane.dae");
	ammoPlane->AddComponent(GRAPHICS, gAmmoPlane);
	ammoPlane->scale = glm::vec3(0.05f);

	Component* gp = gModel;
	model->AddComponent(GRAPHICS, gp);
	gCol = new ComponentCollision();
	gCol->setOwner(model);
	gCol->setCollisionMask(gModel->getScene());
	model->pos = glm::vec3(7.5, 0.5, -11);
	model->vel = 0.1;
	model->dir = glm::vec3(1, 0, 0);
	model->scale = glm::vec3(5, 5, 5);
	gCol->type = MOVING;
	gCol->createHitboxRender();
	gp = gCol;
	model->AddComponent(PHYSICS, gp);
	ComponentInput* ci = new ComponentInput(0.05, 0.05);
	gp = ci;
	model->AddComponent(CONTROLLER, gp);

	targetSpawner2.position = glm::vec3(-55.f, 0, 70.f);
	targetSpawner2.position = targetSpawner2.position * 0.1f;

	//PROPER INIT
	for (int i = 0; i < 24; i++)
	{
		if (i != 12 && i != 20 && i != 22)
		{
			GameObject *gObject = new GameObject(goVec.size());
			Model *cModel = new Model();
			ComponentCollision *cCollision = new ComponentCollision();
			Component *c;
			float groundMod = 0;

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
				cModel->loadModel("models/Water Tower.dae");

				gObject->scale = glm::vec3(3, 3, 3);
				gObject->pos = glm::vec3(-112.75, 0, 105);
			}
			else if (i == 2)
			{
				gObject->SetName("MenuScene");
				cModel->loadModel("models/Warehouse.dae");

				gObject->scale = glm::vec3(1, 1, 1);// glm::vec3(1.6, 1.6, 1.6);
				gObject->pos = glm::vec3(30, 0, 130);
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
				cModel->loadModel("models/WallA.dae");

				gObject->scale = glm::vec3(1.5, 4.0, 1.0);
				gObject->pos = glm::vec3(0);
				groundMod = -8;
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
				cModel->loadModel("models/WallB.dae");

				gObject->scale = glm::vec3(1.0, 4.0, 1.5);
				gObject->pos = glm::vec3(0);
				groundMod = -8;
			}
			else if (i == 9)
			{
				gObject->SetName("Container 2");
				cModel->loadModel("models/Shipping Container.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-14, 0, 73);
				gObject->rot = glm::vec3(0, 65.44f, 0);
			}
			else if (i == 10)
			{
				gObject->SetName("South Wall");
				cModel->loadModel("models/WallC.dae");

				gObject->scale = glm::vec3(1.5, 4.0, 1.0);
				gObject->pos = glm::vec3(0);
				groundMod = -8;
			}
			else if (i == 11)
			{
				gObject->SetName("East Wall");
				cModel->loadModel("models/WallD.dae");

				gObject->scale = glm::vec3(1.0, 4.0, 1.5);
				gObject->pos = glm::vec3(0);
				groundMod = -8;
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
				//cModel->setActiveAnimation(cModel->getScene()->mAnimations[0]);
				//cModel->animate(0.5);
			}
			else if (i == 20)
			{
				gObject->SetName("Wall");
				cModel->loadModel("models/wallz2.dae");

				gObject->scale = glm::vec3(0.30,1,0.30);
				//gObject->pos = glm::vec3(84.727f, 0, -154.085f);
				gObject->pos = glm::vec3(0, 0, 0);
				groundMod = -5;
			}
			else if (i == 21)
			{
				gObject->SetName("Destroyed House");
				cModel->loadModel("models/Destroyed Building.dae");

				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(-35, 0, -167.582f);
				gObject->rot = glm::vec3(0, -90.f, 0);
			}
			else if (i==22)
			{
				gObject->SetName("Tunnel");
				cModel->loadModel("models/Tunnel.dae");
			
				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(0.0147653f,13.6108f,7.22379f);
				gObject->rot = glm::vec3(0, -90.f, 0);
				arms = gObject;
				cModel->setOwner(gObject);
				c = cModel;
				gObject->AddComponent(GRAPHICS, c);
				goVec.push_back(gObject);
				break;
			}
			else if (i == 23)
			{
				gObject->SetName("Sphere");
				cModel->loadModel("models/sphere.dae");
				gObject->scale = glm::vec3(1);
				gObject->pos = glm::vec3(0, 15, 0);

				cModel->setOwner(gObject);
				c = cModel;
				gObject->AddComponent(GRAPHICS, c);
				goVec.push_back(gObject);
				break;
			}
			else if (i == 24)
			{
				gObject->SetName("Guns");
				cModel->loadModel("models/1.dae");

				gObject->scale = glm::vec3(1);
				//gObject->pos = glm::vec3(7, 14, 1.582f);
				//gObject->rot = glm::vec3(0, -90.f, 0);
				arms = gObject;
				cModel->setOwner(gObject);
				c = cModel;
				gObject->AddComponent(GRAPHICS, c);
				goVec.push_back(gObject);
				break;
			}

			gObject->pos /= 10.f;
			gObject->pos.y = ground->HeightAtLocation(gObject->pos) + groundMod;

			cModel->setOwner(gObject);
			c = cModel;
			gObject->AddComponent(GRAPHICS, c);
			if (i != 200){
				gObject->AddComponent(PHYSICS, cCollision);
				cCollision->setOwner(gObject);
				cCollision->setCollisionMask(cModel->getScene());
				cCollision->type = STATIC;
				cCollision->createHitboxRender();
			}
			goVec.push_back(gObject);
		}
	}

	LoadTargets();

	//testmodel = new Model();
	//testmodel->loadModel("models/Watertower.dae");

	spotLightColour = (glm::vec3(0.25, 0.25, 0.25));
	for (int i = 0; i < 2; i++)
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
	while (!glfwWindowShouldClose(gWindow)){
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
		if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) && showHighScores == false && escapeHasBeenPressed == false){
			glfwSetWindowShouldClose(gWindow, GL_TRUE);
		}
		else if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) && showHighScores == true)
		{
			showHighScores = false;
			escapeHasBeenPressed = true;
		}

		if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
			escapeHasBeenPressed = false;
	}

	saveScoreBoard(scoreTable, sndNames, survivalScoreTable, survivalNames);

	// clean up and exit
	glfwTerminate();
}


int main(int argc, char *argv[]) {
	try {
		AppMain();
	}
	catch (const std::exception& e){
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
