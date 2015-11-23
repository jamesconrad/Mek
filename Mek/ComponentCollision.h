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

	glm::vec3 fmin, fmax, fuN, frN, ffN, fc;
};
/*
class PLANE {
public:
	float equation[4];
	glm::vec3 origin;
	glm::vec3 normal;
	PLANE(const glm::vec3& origin, const glm::vec3& normal);
	PLANE(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	bool isFrontFacingTo(const glm::vec3& direction) const;
	float signedDistanceTo(const glm::vec3& point) const;
};

struct CollisionPacket {
	glm::vec3 eRadius; // ellipsoid radius
	// Information about the move being requested: (in R3)
	glm::vec3 R3Velocity;
	glm::vec3 R3Position;
	// Information about the move being requested: (in eSpace)
	glm::vec3 velocity;
	glm::vec3 normalizedVelocity;
	glm::vec3 basePoint;
	// Hit information
	bool foundCollision;
	float nearestDistance;
	glm::vec3 intersectionPoint;
};
*/
class CollisionManager;

class ComponentCollision : public Component
{
public:
	ComponentCollision();
	//WARNING: MUST RECIEVE A CompoentGraphics::getBoneInfoRef()
	void updateFrame(void* boneInfoLocation, bool bones);
	
	bool checkVs(ComponentCollision* c);
	void setCollisionMask(const aiScene* _scene);

	void ComponentCollision::collideAndSlide(const glm::vec3& vel, const glm::vec3& gravity, int vs);
	glm::vec3 ComponentCollision::collideWithWorld(const glm::vec3& pos, const glm::vec3& vel, int vs);
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


};

class CollisionManager
{
public:
	//bool checkCollision(CollisionPacket*, int handle, int vs);
	void addObject(ComponentCollision*);

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