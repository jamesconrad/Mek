#pragma once
// engine includes
#include "GameObject.h"
#include "Render.h"

// assimp includes
#include "include\assimp\Importer.hpp"
#include "include\assimp\postprocess.h"
#include "include\assimp\scene.h"

// devil includes
#include "include\IL\il.h"

// gl includes
#include "include\GL\glfw3.h"
#include "include\GL\glew.h"
#include "lib\glm\glm.hpp"
#include "lib\glm\gtx\vector_angle.hpp"
#include "lib\glm\gtx\quaternion.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"

// std includes
#include <map>
#include <vector>

#define MAX_BONES_PER_VERTEX 4
#define MAX_BONES 32

/*
Material Implementation Checklist:

Uncomplete:
	specular
	ambient
	emissive
	height
	shininess
	displacement
	lightmap
	reflection

Complete:
	diffuse
	normal

*/


class Model : public Component
{
public:
	void update();
	void loadModel(char* fp);
	void loadScene(aiScene* scene);
	void render();
	void renderShadowPass();
	void setShadowMapID(int id);

	void setActiveAnimation(aiAnimation* animation);
	void setActiveAnimation(aiAnimation* anim1, aiAnimation* anim2, float factor);
	
	void animate(float animTime);

	const aiScene* getScene();

	glm::mat4 rotMatrix;

	void set_RenderedTexture(GLuint _passedTexture);

	float cut = 1.0f;
	void setFilepath(char* fp) { _fp = fp; }
private:
	struct Entry
	{
		Entry() : numIndices(0), baseIndex(0), baseVertex(0), materialIndex(0) {}
		unsigned int
			numIndices,
			baseIndex,
			baseVertex,
			materialIndex;
	};
	struct BoneInfo
	{
		glm::mat4 rootTransform;
		glm::mat4 finalTransform;
	};
	struct BoneWeight
	{
		unsigned int bone[MAX_BONES_PER_VERTEX];
		float weight[MAX_BONES_PER_VERTEX];
		void add(unsigned int b, float w);
	};
	//Animation Functions
	void interpolateScaling(glm::vec3& out, float animTime, aiNodeAnim* animNode);
	void interpolateTranslation(glm::vec3& out, float animTime, aiNodeAnim* animNode);
	void interpolateRotation(glm::quat& out, float animTime, aiNodeAnim* animNode);
	unsigned int findScalingKey(float animTime, aiNodeAnim* animNode);
	unsigned int findRotationKey(float animTime, aiNodeAnim* animNode);
	unsigned int findPositionKey(float animTime, aiNodeAnim* animNode);
	aiNodeAnim* findAnimNode(aiAnimation* anim, std::string nodeName);
	void processAnimation(float animTime, aiAnimation* anim,
		bool blend = false,	aiAnimation* anim2 = nullptr, 
		float factor = 0);
	void calcFinalTransform(float animTime, aiAnimation* anim,
		aiNode* node, glm::mat4& parent, 
		bool blend = false,	aiAnimation* anim2 = nullptr, 
		float factor = 0);
	//TODO:
	//inverseKinematic
	
	void renderCreateTexture(unsigned int usage);

	bool _animBlending;
	aiAnimation* _activeAnim[3];
	float _blendFactor;

	static int shadowMapTexID;

	bool _animated;
	Render* _render;

	aiScene* _scene;
	Assimp::Importer _importer;

	unsigned int _numBones;
	std::map<std::string, unsigned int> _boneMap;
	std::vector<BoneInfo> _boneInfo;
	std::vector<Entry> _entries;

	std::vector<glm::mat4> _finalFrameTransform;

	glm::mat4 _rootTransform;

	char* _fp;
};