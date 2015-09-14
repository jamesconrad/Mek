#pragma once
// engine includes
#include "Component.h"
#include "Texture.h"

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
#include "lib\glm\gtc\matrix_transform.hpp"

// std includes
#include <map>
#include <vector>


class ComponentGraphics : public Component
{
public:
	void update();
	void loadModel(char* model);
	void render();
private:
	// skeleton structs
#define NUM_BONES_PER_VEREX 4

	struct BoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
	};

	struct VertexBoneData
	{
		unsigned int IDs[NUM_BONES_PER_VEREX];
		float Weights[NUM_BONES_PER_VEREX];

		VertexBoneData()
		{
			Reset();
		};

		void Reset()
		{
			memset(IDs, 0, sizeof(IDs));
			memset(Weights, 0, sizeof(Weights));
		}

		void AddBoneData(unsigned int BoneID, float Weight);
	};
	enum VB_TYPES {
		INDEX_BUFFER,
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		BONE_VB,
		NUM_VBs
	};
	struct MeshEntry {
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = 0;
		}

		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
	};

	// model loader functions
	void initFromScene(const aiScene* scene, const char* filepath);
	void initMesh(unsigned int MeshIndex,
		const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<VertexBoneData>& Bones,
		std::vector<unsigned int>& Indices);
	void loadBones(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
	void initMaterials(const aiScene* pScene);

	// model loader vars
	const aiScene* _scene;
	Assimp::Importer _importer;
	std::map<std::string, GLuint*> _textureIdMap;
	GLuint* _textureIds;

	// model rendering vars
	GLuint _vbo;
	GLuint _vao;
	GLuint _buffers[NUM_VBs];
	unsigned int _numBones;
	std::map<std::string, unsigned int> _boneMapping; // maps a bone name to its index
	std::vector<BoneInfo> _boneInfo;
	std::vector<MeshEntry> _entries;
	std::vector<Texture*> _textures;
	glm::mat4 _transform;
};