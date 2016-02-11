#include "Model.h"
#include "Interpolation.h"

#define BUFFER_OFFSET(i) (char*)0 + (i)

void Model::update()
{

}

void CopyaiMat(const aiMatrix4x4& from, glm::mat4 &to)
{
	to[0][0] = from.a1; to[1][0] = from.a2;
	to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2;
	to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2;
	to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2;
	to[2][3] = from.d3; to[3][3] = from.d4;
}

void Model::BoneWeight::add(unsigned int b, float w)
{
	for (unsigned int i = 0; i < MAX_BONES_PER_VERTEX; i++)
	{
		if (weight[i] == 0.0)
		{
			bone[i] = b;
			weight[i] = w;
			return;
		}
	}
}

void Model::loadModel(char* fp)
{
	_importer.GetIOHandler();
	_scene = (aiScene*)_importer.ReadFile(fp, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (!_scene)
		printf("Error parsing %s : %s\n", fp, _importer.GetErrorString());
	
	loadScene(_scene);
}

void Model::loadScene(aiScene* scene)
{
	_render = new Render();
	_scene = scene;
	CopyaiMat(scene->mRootNode->mTransformation, _rootTransform);

	_entries.resize(scene->mNumMeshes);

	std::vector<glm::vec3> position;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> texcoord;
	std::vector<BoneWeight> boneweight;
	std::vector<unsigned int> indices;

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	for (unsigned int i = 0, s = _entries.size(); i < s; i++)
	{
		_entries[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;
		_entries[i].baseIndex = numIndices;
		_entries[i].baseVertex = numVertices;
		_entries[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;

		numVertices += scene->mMeshes[i]->mNumVertices;
		numIndices += _entries[i].numIndices;
	}

	//reserve space in vectors to prevent memory shifting
	position.reserve(numVertices);
	normal.reserve(numVertices);
	texcoord.reserve(numVertices);
	boneweight.reserve(numVertices);
	indices.reserve(numIndices);

	for (unsigned int i = 0, s = _entries.size(); i < s; i++)
	{ //For each mesh
		const aiMesh* mesh = scene->mMeshes[i];

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{ //For each vertex
			position.push_back(glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z));
			normal.push_back(glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z));
			texcoord.push_back(mesh->HasTextureCoords(0) ? 
			/*true*/	glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].x) : 
			/*false*/	glm::vec2(0,0));

			for (unsigned int k = 0; k < mesh->mNumBones; k++)
			{ //For each bone
				unsigned int index = 0;
				std::string name(mesh->mBones[k]->mName.data);

				if (_boneMap.find((char*)name.c_str()) == _boneMap.end())
				{
					//setup new bone
					index = _numBones;
					_numBones++;
					_boneInfo.push_back(BoneInfo());
					CopyaiMat(mesh->mBones[i]->mOffsetMatrix, _boneInfo[index].rootTransform);
					_boneMap[(char*)name.c_str()] = index;
				}
				else
					index = _boneMap[(char*)name.c_str()];

				for (unsigned int l = 0; l < mesh->mBones[k]->mNumWeights; l++)
				{ //For each affected vertex
					unsigned int vertex = _entries[i].baseVertex + mesh->mBones[k]->mWeights[l].mVertexId;
					float weight = mesh->mBones[k]->mWeights[l].mWeight;
					boneweight[vertex].add(index, weight);
				}
			}
			for (unsigned int k = 0; k < MAX_BONES; k++)
			{
				//store the uniform locations now
				char loc[16];
				memset(loc, 0, sizeof(loc));
				_snprintf_s(loc, sizeof(loc), "Bones[%i]", i);
				Program::getInstance().bind("standard");
				Program::getInstance().uniform(loc);
			}
		}
	}
	//All done with the vertex data!

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* mat = scene->mMaterials[i];

		for (unsigned int type = 0; type < 12; type++)
		{
			if (mat->GetTextureCount((aiTextureType)type) > 0)
			{
				aiString fp;
				if (mat->GetTexture((aiTextureType)type, 0, &fp, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					_render->createTexture((char*)fp.C_Str(), "todo", (TextureFlag)type);
					break;
				}
			}
		}
	}
	//All done with textures

	//Manually setting the opengl stuff since the render class dosnt support struct data
	glGenVertexArrays(1, &_render->_vao);
	glBindVertexArray(_render->_vao);
	glGenBuffers(2, _render->_vbo);
	_render->_numvbo = 2;
	//Indices first as they are the most straight forward
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _render->_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
	
	//Now to interleave the other data into one vbo
	/*
	elements			type
	position	= 3		GL_FLOAT
	normal		= 3		GL_FLOAT
	texcoord	= 2		GL_FLOAT
	boneid		= 4		GL_UNSIGNED_INT
	boneweight	= 4		GL_FLOAT
	*/
	struct VertexData
	{							//previousfloats * sizeof(float) = offset
		glm::vec3 pos;			//0 * 4 = 0
		glm::vec3 normal;		//3 * 4 = 12
		glm::vec2 uv;			//6 * 4 = 24
		float weight[4];		//8 * 4 = 32
		unsigned int boneid[4];	//12 * 4 = 48, after weights to prevent having to deal with swap in data type
	};
	std::vector<VertexData> vert;
	vert.resize(position.size());
	for (int i = 0, s = position.size(); i < s; i++)
	{
		vert[i].pos = position[i];
		vert[i].normal = normal[i];
		vert[i].uv = texcoord[i];
		memcpy(vert[i].boneid, boneweight[i].bone, sizeof(unsigned int)* 4);
		memcpy(vert[i].weight, boneweight[i].weight, sizeof(float)* 4);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _render->_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData)* vert.size(), vert.data(), GL_STATIC_DRAW);

	//vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0));
	//normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(12));
	//uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(24));
	//weights
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(32));
	//boneids
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(48));
}

unsigned int Model::findScalingKey(float animTime, aiNodeAnim* animNode)
{
	for (unsigned int i = 0; i < animNode->mNumScalingKeys - 1; i++)
	{
		if (animTime < (float)animNode->mScalingKeys[i + 1].mTime)
			return i;
	}
}

unsigned int Model::findRotationKey(float animTime, aiNodeAnim* animNode)
{
	for (unsigned int i = 0; i < animNode->mNumRotationKeys - 1; i++)
	{
		if (animTime < (float)animNode->mRotationKeys[i + 1].mTime)
			return i;
	}
}

unsigned int Model::findPositionKey(float animTime, aiNodeAnim* animNode)
{
	for (unsigned int i = 0; i < animNode->mNumPositionKeys - 1; i++)
	{
		if (animTime < (float)animNode->mPositionKeys[i + 1].mTime)
			return i;
	}
}

void Model::interpolateScaling(glm::vec3& out, float animTime, aiNodeAnim* animNode)
{
	if (animNode->mNumScalingKeys == 1)
	{
		aiVector3D* aiV = &animNode->mScalingKeys[0].mValue;
		out = glm::vec3(aiV->x, aiV->y, aiV->z);
		return;
	}

	unsigned int currentKey = findScalingKey(animTime, animNode);
	unsigned int nextKey = currentKey + 1;
	
	float animLength = (float)animNode->mScalingKeys[nextKey].mTime - animNode->mScalingKeys[currentKey].mTime;
	//scale t inbetween 0 and 1 for lerp
	float t = (animTime - (float)animNode->mScalingKeys[currentKey].mTime) / animLength;
	aiVector3D& current = animNode->mScalingKeys[currentKey].mValue;
	aiVector3D& next = animNode->mScalingKeys[nextKey].mValue;
	aiVector3D delta = next - current;
	aiVector3D result = current + t * delta;
	out = glm::vec3(result.x, result.y, result.z);
}

void Model::interpolateTranslation(glm::vec3& out, float animTime, aiNodeAnim* animNode)
{
	if (animNode->mNumScalingKeys == 1)
	{
		aiVector3D* aiV = &animNode->mPositionKeys[0].mValue;
		out = glm::vec3(aiV->x, aiV->y, aiV->z);
		return;
	}

	unsigned int currentKey = findPositionKey(animTime, animNode);
	unsigned int nextKey = currentKey + 1;

	float animLength = (float)animNode->mPositionKeys[nextKey].mTime - animNode->mPositionKeys[currentKey].mTime;
	//scale t inbetween 0 and 1 for lerp
	float t = (animTime - (float)animNode->mPositionKeys[currentKey].mTime) / animLength;
	aiVector3D& current = animNode->mPositionKeys[currentKey].mValue;
	aiVector3D& next = animNode->mPositionKeys[nextKey].mValue;
	aiVector3D delta = next - current;
	aiVector3D result = current + t * delta;
	out = glm::vec3(result.x, result.y, result.z);
}

void Model::interpolateRotation(glm::quat& out, float animTime, aiNodeAnim* animNode)
{
	if (animNode->mNumRotationKeys == 1)
	{
		aiQuaternion* aiC = &animNode->mRotationKeys[0].mValue;
		out = glm::quat(aiC->w, aiC->x, aiC->y, aiC->z);
	}

	unsigned int currentKey = findRotationKey(animTime, animNode);
	unsigned int nextKey = findRotationKey(animTime, animNode);

	float animLength = (float)animNode->mRotationKeys[nextKey].mTime - animNode->mRotationKeys[currentKey].mTime;
	//scale t inbwetween 0 and 1 for lerp
	float t = (animTime - (float)animNode->mRotationKeys[currentKey].mTime) / animLength;
	aiQuaternion& current = animNode->mRotationKeys[currentKey].mValue;
	aiQuaternion& next = animNode->mRotationKeys[nextKey].mValue;
	aiQuaternion result;
	aiQuaternion::Interpolate(result, current, next, t);
	out = glm::quat(result.w, result.x, result.y, result.z);
}

void Model::setActiveAnimation(aiAnimation* anim)
{
	_activeAnim[0] = anim;
	_animBlending = false;
}

void Model::setActiveAnimation(aiAnimation* anim1, aiAnimation* anim2, float f)
{
	_activeAnim[1] = anim1;
	_activeAnim[2] = anim2;
	_animBlending = true;
	_blendFactor = f;
}

void Model::processAnimation(float animTime, aiAnimation* anim, bool blend, aiAnimation* anim2, float factor)
{
	float tps = (float)(anim->mTicksPerSecond != 0 ? anim->mTicksPerSecond : 30.f);
	float tit = animTime * tps;
	float at = fmod(tit, (float)anim->mDuration);

	glm::mat4 identity;

	calcFinalTransform(animTime, anim, _scene->mRootNode, identity, blend, anim2, factor);
}

void Model::animate(float animTime)
{
	if (!_animBlending)
	{
		processAnimation(animTime, _activeAnim[0]);
	}
	else
	{
		processAnimation(animTime, _activeAnim[1], true, _activeAnim[2], _blendFactor);
	}
}

void Model::calcFinalTransform(float animTime, aiAnimation* anim, aiNode* node, glm::mat4& parent, bool blend, aiAnimation* anim2, float factor)
{
	aiNodeAnim* nodeanim = findAnimNode(anim, std::string(node->mName.data));

	glm::mat4 nodetransform;
	CopyaiMat(node->mTransformation, nodetransform);

	if (nodeanim)
	{
		glm::vec3 scale;
		interpolateScaling(scale, animTime, nodeanim);

		glm::vec3 trans;
		interpolateTranslation(scale, animTime, nodeanim);

		glm::quat rotat;
		interpolateRotation(rotat, animTime, nodeanim);

		if (_animBlending)
		{
			glm::vec3 scale2;
			interpolateScaling(scale2, animTime, nodeanim);
			scale = lerp(scale, scale2, factor);
			glm::vec3 trans2;
			interpolateTranslation(scale2, animTime, nodeanim);
			trans = lerp(trans, trans2, factor);
			glm::quat rotat2;
			interpolateRotation(rotat2, animTime, nodeanim);
			rotat = slerp(rotat, rotat2, factor);
		}

		nodetransform = glm::translate(glm::mat4(), trans) * glm::mat4_cast(rotat) * glm::scale(glm::mat4(), scale);
	}

	glm::mat4 globaltransform = parent * nodetransform;

	if (_boneMap.find((char*)node->mName.C_Str()) != _boneMap.end())
	{
		unsigned int BoneIndex = _boneMap[(char*)node->mName.C_Str()];
		_boneInfo[BoneIndex].finalTransform = globaltransform * _boneInfo[BoneIndex].rootTransform;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		calcFinalTransform(animTime, anim, node->mChildren[i], globaltransform, blend, anim2, factor);
	}
}

aiNodeAnim* Model::findAnimNode(aiAnimation* anim, std::string nodename)
{
	for (unsigned int i = 0; i < anim->mNumChannels; i++)
	{
		aiNodeAnim* nodeanim = anim->mChannels[i];
		if (std::string(nodeanim->mNodeName.data) == nodename)
			return nodeanim;
	}
}

void Model::render()
{
	//todo shaderwork
}