#pragma once

#include "GameObject.h"
#include "include\assimp\scene.h"
#include "lib\glm\glm.hpp"


//IMPORTANT
//Collision dosn't do animation scaling!

enum ColType
{
	PROJ,
	STATIC,
	MOVING
};

struct BoneBox
{
	int boneNum;
	std::string name;
	glm::vec3 min, max;
	glm::vec3 uN, rN, fN;

	glm::vec3 centre;
	float hwU, hwR, hwF;

	float fhwU, fhwR, fhwF;
	glm::vec3 fmin, fmax, fuN, frN, ffN, fc;
};

class CollisionManager;

class ComponentCollision : public Component
{
public:
	ComponentCollision();
	//WARNING: MUST RECIEVE A CompoentGraphics::getBoneInfoRef()
	void updateFrame(void* boneInfoLocation, bool bones);
	
	bool checkVs(ComponentCollision* c);
	void setCollisionMask(const aiScene* _scene);

	void createHitboxRender();
	void renderHitbox();

	void update() {}
	
	void setCollisionElip(glm::vec3) {}

	float radius;
	std::vector<BoneBox*> _cMesh;
	const aiScene* _scene;
	int collisionRecursionDepth;
	ColType type;
private:

	struct BoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
	};

	struct gHitbox
	{
		float* vertexData;
	};

	bool _hitboxRender = false;
	unsigned int *_vao, *_vbo;
	std::vector<gHitbox> gHitboxes;
};

class CollisionManager
{
public:
	void checkAll();

	//singleton
	static CollisionManager& instance()
	{
		static CollisionManager instance;
		return instance;
	}

private:
	CollisionManager() {}
	//void checkTriangle(CollisionPacket* colPackage, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
};