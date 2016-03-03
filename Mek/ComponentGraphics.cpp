#include "ComponentGraphics.h"
#include "Program.h"
#include "lib\glm\gtc\type_ptr.hpp"
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
	std::string debfp = "../Debug/";
	debfp.append(filepath);
	filepath = (char*)debfp.c_str();


	// Create the VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(_buffers), _buffers);

	_importer.GetIOHandler();
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

void ComponentGraphics::loadModel(aiScene* scene)
{
	// Create the VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(_buffers), _buffers);

	
	_scene = scene;

	if (_scene)
	{
		CopyaiMat(&_scene->mRootNode->mTransformation, _transform);
		_transform = glm::inverse(_transform);
		initFromScene(_scene, "");
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", "Pre-Defined Scene", _importer.GetErrorString());
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

	//Program::getInstance().use("skinning");
	
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

	//Program::getInstance().stopUsing("skinning");
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

	_indices = Indices;
}

GLint GetUniformLocation(const char* pUniformName)
{

	GLuint Location = glGetUniformLocation(Program::getInstance().object("skinning"), pUniformName);

	if (Location == 0xffffffff) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
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
	// init the gBones array requriments for the shader
	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(_boneLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gBones[%d]", i);
		_boneLocation[i] = GetUniformLocation(Name);
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
				Bitmap bmp;
				std::string fp = "";
				fp.append(Path.data);
				_textures[i] = new Texture((char*)fp.c_str());
				if (_textures[i]->originalWidth() == 0)
				{
					GLuint o = _textures[i]->object();
					glDeleteTextures(1, &o);
					free(_textures[i]);
					_textures[i] = new Texture("missingtexture.png");
				}
			}
		}
		else
		{
			_textures[i] = new Texture("missingtexture.png");
		}
	}
}

void ComponentGraphics::render()
{
	//
	if (_scene->mMeshes[0]->mNumBones > 0)
	{
		Program::getInstance().use("anim");
		Program::getInstance().updateLighting("anim");
	}
	else
	{
		Program::getInstance().use("skinning");
		Program::getInstance().updateLighting("skinning");
	}
	updateShader();
	for (unsigned i = 0, s = _frameBoneTransforms.size(); i < s; ++i)
		glUniformMatrix4fv(_boneLocation[i], 1, GL_FALSE, (const GLfloat*)&_frameBoneTransforms[i]);
	//
	glBindVertexArray(_vao);
	for (unsigned int i = 0; i < _entries.size(); i++)
	{
		const unsigned int MaterialIndex = _entries[i].MaterialIndex;

		assert(MaterialIndex < _textures.size());

		if (_textures[MaterialIndex])
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _textures[MaterialIndex]->object());
		}
		glDrawElementsBaseVertex(GL_TRIANGLES,
			_entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int)* _entries[i].BaseIndex),
			_entries[i].BaseVertex);

		//glDrawArrays(GL_TRIANGLES,
		//	_entries[i].NumIndices,
		//	_entries[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	if (_scene->mMeshes[0]->mNumBones > 0)
		Program::getInstance().stopUsing("anim");
	else
		Program::getInstance().stopUsing("skinning");
}

unsigned int ComponentGraphics::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


unsigned int ComponentGraphics::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


unsigned int ComponentGraphics::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


void ComponentGraphics::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


void ComponentGraphics::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}


void ComponentGraphics::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


void ComponentGraphics::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = _scene->mAnimations[0];

	glm::mat4 NodeTransformation;
	CopyaiMat(&pNode->mTransformation, NodeTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		glm::mat4 ScalingM;
		ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		glm::mat4 RotationM;
		CopyaiMat(&aiMatrix4x4(RotationQ.GetMatrix()),RotationM);

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM;
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));
		
		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;
	
	if (_boneMapping.find(NodeName) != _boneMapping.end())
	{
		unsigned int BoneIndex = _boneMapping[NodeName];
		_boneInfo[BoneIndex].FinalTransformation = GlobalTransformation * _boneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}


void ComponentGraphics::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
{
	glm::mat4 Identity;

	float TicksPerSecond = (float)(_scene->mAnimations[0]->mTicksPerSecond != 0 ? _scene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)_scene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(AnimationTime, _scene->mRootNode, Identity);

	Transforms.resize(_numBones);

	for (unsigned int i = 0; i < _numBones; i++)
	{
		Transforms[i] = _boneInfo[i].FinalTransformation;
	}

	_frameBoneTransforms = Transforms;
}


const aiNodeAnim* ComponentGraphics::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++)
	{
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName)
			return pNodeAnim;
	}

	return NULL;
}

void ComponentGraphics::updateShader()
{
	glm::mat4 W;// = _transform;
	W = glm::translate(W, _owner->pos);
	//W = glm::rotate(W, _owner->rot);
	W = glm::scale(W, 0.1f * _owner->scale);
	
	glm::mat4 VP;
	VP = Camera::getInstance().matrix();

	glm::mat4 WVP = VP * W;
	if (_scene->mMeshes[0]->mNumBones > 0)
	{
		Program::getInstance().bind("anim");
		Program::getInstance().setUniform("gWVP", WVP);
		Program::getInstance().setUniform("gWorld", W);
		Program::getInstance().updateLighting("anim");
	}
	else
	{
		Program::getInstance().bind("skinning");
		Program::getInstance().setUniform("gWVP", WVP);
		Program::getInstance().setUniform("gWorld", W);
		Program::getInstance().setUniform("gEyeWorldPos", Camera::getInstance().position());
		Program::getInstance().setUniform("EyeViewVec", Camera::getInstance().forward());
		Program::getInstance().updateLighting("skinning");
	}
}