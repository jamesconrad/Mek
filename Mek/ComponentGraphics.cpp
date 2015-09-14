#include "ComponentGraphics.h"
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

// helper to determine proper VBO index
#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4


void ComponentGraphics::update()
{

}
// Helper function, should only be needed for this one file
void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to)
{
	to[0][0] = from->a1; to[1][0] = from->a2;
	to[2][0] = from->a3; to[3][0] = from->a4;
	to[0][1] = from->b1; to[1][1] = from->b2;
	to[2][1] = from->b3; to[3][1] = from->b4;
	to[0][2] = from->c1; to[1][2] = from->c2;
	to[2][2] = from->c3; to[3][2] = from->c4;
	to[0][3] = from->d1; to[1][3] = from->d2;
	to[2][3] = from->d3; to[3][3] = from->d4;
}

void ComponentGraphics::VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++)
	{
		if (Weights[i] == 0.0)
		{
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	assert(0);
}

void ComponentGraphics::loadModel(char* filepath)
{
	// Create the VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vbo);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(_buffers), _buffers);

	_scene = _importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (_scene)
	{
		CopyaiMat(&_scene->mRootNode->mTransformation, _transform);
		_transform = glm::inverse(_transform);
		initFromScene(_scene, filepath);
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", filepath, _importer.GetErrorString());
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}

void ComponentGraphics::initFromScene(const aiScene* scene, const char* filepath)
{
	_entries.resize(scene->mNumMeshes);
	_textures.resize(scene->mNumMaterials);

	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<VertexBoneData> Bones;
	std::vector<unsigned int> Indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	// Count the number of vertices and indices
	for (unsigned int i = 0; i < _entries.size(); i++)
	{
		_entries[i].MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
		_entries[i].NumIndices = scene->mMeshes[i]->mNumFaces * 3;
		_entries[i].BaseVertex = NumVertices;
		_entries[i].BaseIndex = NumIndices;

		NumVertices += scene->mMeshes[i]->mNumVertices;
		NumIndices += _entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Bones.resize(NumVertices);
	Indices.reserve(NumIndices);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < _entries.size(); i++)
	{
		const aiMesh* taiMesh = scene->mMeshes[i];
		initMesh(i, taiMesh, Positions, Normals, TexCoords, Bones, Indices);
	}

	initMaterials(scene);

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

void ComponentGraphics::initMesh(unsigned int MeshIndex,
	const aiMesh* paiMesh,
	std::vector<glm::vec3>& Positions,
	std::vector<glm::vec3>& Normals,
	std::vector<glm::vec2>& TexCoords,
	std::vector<VertexBoneData>& Bones,
	std::vector<unsigned int>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	loadBones(MeshIndex, paiMesh, Bones);

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}


void ComponentGraphics::loadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
	for (unsigned int i = 0; i < pMesh->mNumBones; i++)
	{
		unsigned int BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (_boneMapping.find(BoneName) == _boneMapping.end())
		{
			// Allocate an index for a new bone
			BoneIndex = _numBones;
			_numBones++;
			BoneInfo bi;
			_boneInfo.push_back(bi);
			CopyaiMat(&pMesh->mBones[i]->mOffsetMatrix, _boneInfo[BoneIndex].BoneOffset);
			_boneMapping[BoneName] = BoneIndex;
		}
		else
		{
			BoneIndex = _boneMapping[BoneName];
		}

		for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
		{
			unsigned int VertexID = _entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}


void ComponentGraphics::initMaterials(const aiScene* pScene)
{
	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		_textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
			{
				std::string p(Path.data);

				Bitmap bmp;
				bmp.bitmapFromFile(p);
				bmp.flipVertically();
				_textures[i] = new Texture(bmp);
			}
		}
	}
}

void ComponentGraphics::render()
{
	glBindVertexArray(_vao);

	for (unsigned int i = 0; i < _entries.size(); i++)
	{
		const unsigned int MaterialIndex = _entries[i].MaterialIndex;

		assert(MaterialIndex < _textures.size());

		if (_textures[MaterialIndex])
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE0, _textures[MaterialIndex]->object());
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			_entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * _entries[i].BaseIndex),
			_entries[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}