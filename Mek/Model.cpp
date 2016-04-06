#include "Model.h"
#include "Interpolation.h"

#define BUFFER_OFFSET(i) (char*)0 + (i)

int Model::shadowMapTexID = 0;

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

void Model::setShadowMapID(int id)
{
	shadowMapTexID = id;
}

const aiScene* Model::getScene()
{
	return _scene;
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
	_fp = fp;
	std::string debfp = "../Debug/";
	debfp.append(fp);
	fp = (char*)debfp.c_str();

	_importer.GetIOHandler();
	_scene = (aiScene*)_importer.ReadFile(fp, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (!_scene)
		printf("Error parsing %s : %s\n", fp, _importer.GetErrorString());
	
	loadScene(_scene);
}

void Model::loadScene(aiScene* scene)
{
	_render = new Render();
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
	//boneweight.reserve(numVertices);
	boneweight.resize(numVertices);
	indices.reserve(numIndices);

	for (unsigned int i = 0, s = _entries.size(); i < s; i++)
	{ //For each mesh
		const aiMesh* mesh = scene->mMeshes[i];
		//Lets do the indices first
		for (int j = 0; j < mesh->mNumFaces; j++)
		{
			indices.push_back(mesh->mFaces[j].mIndices[0]);
			indices.push_back(mesh->mFaces[j].mIndices[1]);
			indices.push_back(mesh->mFaces[j].mIndices[2]);
		}
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{ //For each vertex
			position.push_back(glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z));
			normal.push_back(glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z));
			texcoord.push_back(mesh->HasTextureCoords(0) ? 
			/*true*/	glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y) : 
			/*false*/	glm::vec2(0,0));
			//printf("%f, %f\n", texcoord.back().x, texcoord.back().y);

			for (unsigned int k = 0; k < mesh->mNumBones; k++)
			{ //For each bone
				unsigned int index = 0;
				std::string name(mesh->mBones[k]->mName.data);

				if (_boneMap.find(mesh->mBones[k]->mName.data) == _boneMap.end())
				{
					//setup new bone
					index = _numBones;
					_numBones++;
					_boneInfo.push_back(BoneInfo());
					CopyaiMat(mesh->mBones[i]->mOffsetMatrix, _boneInfo[index].rootTransform);
					//_boneMap[(char*)name.c_str()] = index;
					_boneMap.emplace(mesh->mBones[k]->mName.data, index);
				}
				else
					index = _boneMap[mesh->mBones[k]->mName.data];

				for (unsigned int l = 0; l < mesh->mBones[k]->mNumWeights; l++)
				{ //For each affected vertex
					unsigned int vertex = _entries[i].baseVertex + mesh->mBones[k]->mWeights[l].mVertexId;
					float weight = mesh->mBones[k]->mWeights[l].mWeight;
					boneweight[vertex].add(index, weight);
				}
			}
		}
	}
	//All done with the vertex data!
	bool textured = false;
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
					//if (_render->createTexture((char*)fp.C_Str(), "gColorMap", (TextureFlag)type))//todo: fix
					//{
					//	textured = true;
					//	break;
					//}
					std::string tmpfp(_fp);
					tmpfp = tmpfp.substr(7, tmpfp.size() - 10);
					tmpfp += "png";
					std::string filepath("Textures/");
					filepath += tmpfp;
					if (_render->createTexture((char*)filepath.c_str(), "gColorMap", (TextureFlag)type))
					{
						textured = true;
						break;
					}
				}
			}
		}
	}
	//make sure we have a texture
	if (!textured)
		_render->createTexture("missingtexture.png", "gColorMap", (TextureFlag)1);
	//All done with textures

	unsigned int vao;
	unsigned int *vbo =  new unsigned int[2];

	//Manually setting the opengl stuff since the render class dosnt support struct data
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);
	//Indices first as they are the most straight forward
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
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
		if (i < boneweight.size())
		{//if this fails we do not have any bones, ouch
			memcpy(vert[i].boneid, boneweight[i].bone, sizeof(unsigned int) * 4);
			memcpy(vert[i].weight, boneweight[i].weight, sizeof(float) * 4);
		}
		else
		{//just pad with 0s to solve the issue of no bones
			memset(vert[i].boneid, 0, sizeof(unsigned int)* 4);
			memset(vert[i].weight, 0, sizeof(float)* 4);
		}
	}

	int vsize = sizeof(VertexData);

	int fsize = sizeof(float);

	/*
	layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;
	*/

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vsize * vert.size(), vert.data(), GL_STATIC_DRAW);
	
	//vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vsize, 0);
	//normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vsize, (void*)(3*fsize));
	//uvs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vsize, (void*)(6*fsize));
	//weights
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vsize, (void*)(8*fsize));
	//boneids
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, vsize, (void*)(12*fsize));

	//Pass the preset data to render
	_render->forceOverride(vao, vbo, 2, NULL, true, numIndices, numVertices);

	//Resize bone transform for animations if we have some
	if (_scene != NULL && _scene->HasAnimations())
	{
		_finalFrameTransform.resize(_boneInfo.size());
		_animated = true;
	}

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

	for (int i = 0; i < _boneInfo.size(); i++)
		_finalFrameTransform[i] = _boneInfo[i].finalTransform;
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

	if (_boneMap.find(node->mName.data) != _boneMap.end())
	{
		unsigned int BoneIndex = _boneMap[node->mName.data];
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
	printf("Error: Unable to find anim node %s\n", nodename.c_str());
	return anim->mChannels[0];
}

glm::mat4 rotationMatrix(glm::vec3 &dir, glm::vec3 &baseDir, glm::vec3 &right)
{
	float angle = glm::angle(dir, baseDir);
	float rightAngle = glm::dot(dir, right);
	glm::mat4 rotation;
	if (rightAngle >= 0)
		rotation = glm::mat4(glm::rotate(angle, glm::vec3(0, 1, 0)));
	else
		rotation = glm::mat4(glm::rotate(angle * -1.f, glm::vec3(0, 1, 0)));
	return rotation;
}

void Model::render()
{
	glm::mat4 W;// = _transform;
	//W = glm::rotate(W, _owner->rot);
	W = glm::translate(W, _owner->pos);
	if (getOwner()->GetName() != "PlayerArms")
		rotMatrix = rotationMatrix(_owner->dir, glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1));
	W *= rotMatrix;
	W = glm::scale(W, 0.1f * _owner->scale);
	glm::mat4 VP;
	VP = Camera::getInstance().matrix();
	glm::mat4 WVP = VP * W;
	if (_animated)
	{
		Program::getInstance().bind("anim");
		Program::getInstance().setUniformMatrix4("gBones", &_finalFrameTransform[0][0][0], MAX_BONES);
		Program::getInstance().setUniform("gWVP", WVP);
		Program::getInstance().setUniform("gWorld", W);
		//Program::getInstance().updateLighting("anim");
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

	int nt = _render->numTextures() + 1;
	glActiveTexture(GL_TEXTURE0 + nt);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexID);
	Program::getInstance().setUniform("shadowMap", nt);
	glm::vec3 lightInvDir = glm::vec3(0.0f, -1.0f, -1.0f) * -1.f;

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProj = glm::ortho<float>(-32, 32, -32, 32, -8, 8);
	glm::mat4 depthView = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 depthMVP = depthProj * depthView;
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
	depthMVP = biasMatrix*depthMVP;
	Program::getInstance().setUniform("depthMVP", depthMVP);

	Program::getInstance().setUniform("depthMVP", depthMVP);

	for (int i = 0, s = _entries.size(); i < s; i++)
	{
		_render->draw(true, _entries[i].baseIndex, _entries[i].baseVertex);
	}
}

void Model::renderShadowPass()
{
	Program::getInstance().bind("skinnedShadow");//todo: convert to animated
	//if (_numBones > 0)
	//{
	//	Program::getInstance().setUniformMatrix4("BoneTransform", &_finalFrameTransform[0][0][0], MAX_BONES);
	//}

	glm::vec3 lightInvDir = glm::vec3(0.0f, -1.0f, -1.0f) * -1.f;

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProj = glm::ortho<float>(-32, 32, -32, 32, -8, 8);
	glm::mat4 depthView = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0), glm::vec3(0, 1, 0));

	glm::mat4 depthModel;
	depthModel = glm::translate(depthModel, _owner->pos);
	//depthModel = glm::rotate(depthModel, _owner->rot);
	depthModel = glm::scale(depthModel, 0.1f * _owner->scale);
	glm::mat4 depthMVP = depthProj * depthView * depthModel;


	Program::getInstance().setUniform("LVP", depthMVP);

	for (int i = 0, s = _entries.size(); i < s; i++)
	{
		_render->drawShadowPass(true, _entries[i].baseIndex, _entries[i].baseVertex);
	}
}

void Model::set_RenderedTexture(GLuint _passedTexture)
{
	_render->setRenderedTexture(_passedTexture);
}