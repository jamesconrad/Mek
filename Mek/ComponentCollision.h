#include "GameObject.h"
#include "include\assimp\scene.h"


//outdated, new method below
struct Bone
{
	//this totally isnt the 10th time ive made and deleted this
	//note values are in modelspace
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> n;
	int boneNum;
	aiMesh* parent;
	char* name;
};

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

class CollisionManager;

class ComponentCollision : public Component
{
public:
	ComponentCollision();
	
	bool checkVs(ComponentCollision* c);
	void setCollisionMask(const aiScene* _scene);

	void ComponentCollision::collideAndSlide(const glm::vec3& vel, const glm::vec3& gravity, int vs);
	glm::vec3 ComponentCollision::collideWithWorld(const glm::vec3& pos, const glm::vec3& vel, int vs);
	void update();
	
	void setCollisionElip(glm::vec3);

	float radius;
	std::vector<Bone*> _cMesh;
	CollisionPacket* collisionPackage;
	const aiScene* _scene;
	int collisionRecursionDepth;
	bool staticObj;
private:
};

class CollisionManager
{
public:
	bool checkCollision(CollisionPacket*, int handle, int vs);
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