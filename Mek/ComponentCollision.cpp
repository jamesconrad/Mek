#include "ComponentCollision.h"
#include "ComponentInput.h"
#include "lib\glm\common.hpp"
#include "include\assimp\Importer.hpp"
#include "include\assimp\scene.h"
#include "lib\glm\geometric.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"


#include "Program.h"
#include "Camera.h"
#include "Projectile.h"

//#include "ShieldVariables.h"
extern float shieldHealth;
extern float maxShieldHealth;
extern float shieldRechargeTimer;
extern float shieldMaxTimer;
extern float shieldRechargeAmount;

extern bool  playerIsHit;
extern float hitInvulnTimer;
extern float maxInvulnTime;


ComponentCollision::ComponentCollision()
{
	//collisionPackage = new CollisionPacket;
	type = STATIC;
}

glm::mat4 rotMatrix(glm::vec3 &dir, glm::vec3 &baseDir, glm::vec3 &right)
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

void ComponentCollision::updateFrame(void* boneInfoLocation, bool bones)
{
	//if (bones)
	//{
	//	BoneInfo* boneInfo = static_cast<BoneInfo*>(boneInfoLocation);
	//
	//	for (int i = 0, s = _cMesh.size(); i < s; i++)
	//	{
	//		boneInfo[i].FinalTransformation = glm::scale(boneInfo[i].FinalTransformation, glm::vec3(0.1, 0.1, 0.1));
	//		_cMesh[i]->fmin = (glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->min, 0)) + _owner->pos);// *_owner->scale;
	//		_cMesh[i]->fmax = (glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->max, 0)) + _owner->pos);// *_owner->scale;
	//		_cMesh[i]->fuN = glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->uN, 0));
	//		_cMesh[i]->frN = glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->rN, 0));
	//		_cMesh[i]->ffN = glm::vec3(boneInfo[i].FinalTransformation *glm::vec4(_cMesh[i]->fN, 0));
	//		_cMesh[i]->fc = (glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->centre, 0)) + _owner->pos);//  *_owner->scale;
	//	}
	//}
	//else
	//{
		for (int i = 0, s = _cMesh.size(); i < s; i++)
		{
			glm::mat4 h = glm::translate(glm::mat4(), _owner->pos);
			h = glm::scale(glm::mat4(), glm::vec3(0.1, 0.1, 0.1));
			h = glm::scale(glm::mat4(), _owner->scale);
			h *= rotMatrix(_owner->dir, glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1));

			_cMesh[i]->fmin = (_cMesh[i]->min * 0.1f) + _owner->pos;// *_owner->scale;
			_cMesh[i]->fmax = (_cMesh[i]->max * 0.1f) + _owner->pos;// *_owner->scale;
			_cMesh[i]->fuN = _cMesh[i]->uN;
			_cMesh[i]->frN = _cMesh[i]->rN;
			_cMesh[i]->ffN = _cMesh[i]->fN;
			_cMesh[i]->fhwF = _cMesh[i]->hwF * 0.1f *_owner->scale.z;
			_cMesh[i]->fhwU = _cMesh[i]->hwU;// *0.1f * _owner->scale.y;
			_cMesh[i]->fhwR = _cMesh[i]->hwR * 0.1f *_owner->scale.x;
			_cMesh[i]->fc = (_cMesh[i]->centre + _owner->pos);// *_owner->scale;
		}
	//}
}

void ComponentCollision::setCollisionMask(const aiScene* m)
{
	_scene = m;
	//building the collision mesh
	//puts each bone into a struct

	for (int i = 0; i < m->mNumMeshes; i++)
	{
		if (m->mMeshes[i]->mNumBones != 0)
		{
			for (int boneIndex = 0; boneIndex < m->mMeshes[i]->mNumBones; boneIndex++)
			{
				aiBone* bone = m->mMeshes[i]->mBones[boneIndex];
				bool skip = !(bone->mNumWeights > 0);
				for (int skipIter = 0, s = _cMesh.size(); skipIter < s && !skip; skipIter++)
				{
					//if (_cMesh[skipIter]->name.find(std::string(bone->mName.C_Str())) != std::string::npos) 
					if (strstr(bone->mName.C_Str(), _cMesh[skipIter]->name.c_str()))
						skip = true;
				}

				if (!skip)
				{
					BoneBox* bb = new BoneBox;
					bb->name = bone->mName.C_Str();
					bb->boneNum = boneIndex;

					bool minmaxInit = false;

					for (int vertexIndex = 0; vertexIndex < bone->mNumWeights; vertexIndex++)
					{
						if (bone->mWeights[vertexIndex].mWeight > 0.75)
						{
							aiVector3D aiV = m->mMeshes[i]->mVertices[bone->mWeights[vertexIndex].mVertexId];
							if (!minmaxInit)
							{
								bb->min = bb->max = glm::vec3(aiV.x, aiV.y, aiV.z);
								minmaxInit = true;
							}
							//check vs max
							aiV.x > bb->max.x ? bb->max.x = aiV.x : (aiV.x < bb->min.x ? bb->min.x = aiV.x : aiV.x);
							aiV.y > bb->max.y ? bb->max.y = aiV.y : (aiV.y < bb->min.y ? bb->min.y = aiV.y : aiV.y);
							aiV.z > bb->max.z ? bb->max.z = aiV.z : (aiV.z < bb->min.z ? bb->min.z = aiV.z : aiV.z);
						}
					}
					//compute normals
					glm::vec3 a, b, c;
					//up = normal(MxMyMz, mx,My,mz, Mx,My,mz)
					a = glm::vec3(bb->max.x, bb->max.y, bb->max.z);
					b = glm::vec3(bb->min.x, bb->max.y, bb->min.z);
					c = glm::vec3(bb->max.x, bb->max.y, bb->min.z);
					bb->uN = glm::normalize(glm::cross(b - a, c - a));

					//right = normal(Mx,my,mz, Mx,my,Mz, Mx,My,Mz)
					a = glm::vec3(bb->max.x, bb->min.y, bb->min.z);
					b = glm::vec3(bb->max.x, bb->min.y, bb->max.z);
					c = glm::vec3(bb->max.x, bb->max.y, bb->max.z);
					bb->rN = glm::normalize(glm::cross(b - a, c - a));

					//forward = normal(mx,my,mz, Mx,my,mz, Mx,My,mz
					a = glm::vec3(bb->min.x, bb->min.y, bb->min.z);
					b = glm::vec3(bb->max.x, bb->min.y, bb->min.z);
					c = glm::vec3(bb->max.x, bb->max.y, bb->min.z);
					bb->fN = glm::normalize(glm::cross(b - a, c - a));

					//compute centre
					bb->hwR = abs((bb->max.x - bb->min.x) / 2);
					bb->hwU = abs((bb->max.y - bb->min.y) / 2);
					bb->hwF = abs((bb->max.z - bb->min.z) / 2);

					bb->centre.x = bb->min.x + bb->hwR;
					bb->centre.y = bb->min.y + bb->hwU;
					bb->centre.z = bb->min.z + bb->hwF;

					_cMesh.push_back(bb);
				}
			}
		}
		else
		{
			BoneBox* bb = new BoneBox;
			bb->name = "Mesh";
			bb->name += (char)('0' + i);
			bb->boneNum = -1;
			for (int vertexIndex = 0; vertexIndex < m->mMeshes[i]->mNumVertices; vertexIndex++)
			{
				aiVector3D aiV = m->mMeshes[i]->mVertices[vertexIndex];
				//check vs max
				aiV.x > bb->max.x ? bb->max.x = aiV.x : (aiV.x < bb->min.x ? bb->min.x = aiV.x : aiV.x);
				aiV.y > bb->max.y ? bb->max.y = aiV.y : (aiV.y < bb->min.y ? bb->min.y = aiV.y : aiV.y);
				aiV.z > bb->max.z ? bb->max.z = aiV.z : (aiV.z < bb->min.z ? bb->min.z = aiV.z : aiV.z);
			}
			//compute normals
			glm::vec3 a, b, c;
			//up = normal(MxMyMz, mx,My,mz, Mx,My,mz)
			a = glm::vec3(bb->max.x, bb->max.y, bb->max.z);
			b = glm::vec3(bb->min.x, bb->max.y, bb->min.z);
			c = glm::vec3(bb->max.x, bb->max.y, bb->min.z);
			bb->uN = glm::normalize(glm::cross(b - a, c - a));

			//right = normal(Mx,my,mz, Mx,my,Mz, Mx,My,Mz)
			a = glm::vec3(bb->max.x, bb->min.y, bb->min.z);
			b = glm::vec3(bb->max.x, bb->min.y, bb->max.z);
			c = glm::vec3(bb->max.x, bb->max.y, bb->max.z);
			bb->rN = glm::normalize(glm::cross(b - a, c - a));

			//forward = normal(mx,my,mz, Mx,my,mz, Mx,My,mz
			a = glm::vec3(bb->min.x, bb->min.y, bb->min.z);
			b = glm::vec3(bb->max.x, bb->min.y, bb->min.z);
			c = glm::vec3(bb->max.x, bb->max.y, bb->min.z);
			bb->fN = glm::normalize(glm::cross(b - a, c - a));

			//compute centre
			bb->hwR = abs((bb->max.x - bb->min.x) / 2);
			bb->hwU = abs((bb->max.y - bb->min.y) / 2);
			bb->hwF = abs((bb->max.z - bb->min.z) / 2);

			bb->centre.x = bb->min.x + bb->hwR;
			bb->centre.y = bb->min.y + bb->hwU;
			bb->centre.z = bb->min.z + bb->hwF;

			_cMesh.push_back(bb);
		}
	}

	
}
// takes array of vec3, 8 units wide
bool OBBSatCheck(BoneBox& a, BoneBox& b, glm::vec3& mtd)
{
	//ok this is gonna be a tad long
	//must check the 15 cases that represent 3d space collision

	glm::vec3 T = b.fc - a.fc;

	glm::vec3 mtdb;

	float Rxx = abs(glm::dot(a.frN, b.frN));
	float Rxy = abs(glm::dot(a.frN, b.fuN));
	float Rxz = abs(glm::dot(a.frN, b.ffN));

	float Ryx = abs(glm::dot(a.fuN, b.frN));
	float Ryy = abs(glm::dot(a.fuN, b.fuN));
	float Ryz = abs(glm::dot(a.fuN, b.ffN));

	float Rzx = abs(glm::dot(a.ffN, b.frN));
	float Rzy = abs(glm::dot(a.ffN, b.fuN));
	float Rzz = abs(glm::dot(a.ffN, b.ffN));

	float l;
	float r;
	float lr;

	float smallestPen;
	glm::vec3 penAxis;
	//CASE 1
	//L = Ax
	l = abs(glm::dot(T, a.frN));
	r = a.fhwR + b.fhwR * Rxx + b.fhwU * Rxy + b.fhwF * Rxz;
	lr = l - r;
	mtd.x = l - r;

	smallestPen = l - r;
	penAxis = a.frN;

	if (l > r)
		return false;
	//CASE 2
	//L = Ay
	l = abs(glm::dot(T, a.fuN));
	r = a.fhwU + b.fhwR * Ryx + b.fhwU * Ryy + b.fhwF * Ryz;
	mtd.y = l - r;

	if (abs(l - r) < abs(smallestPen))
	{
		smallestPen = l - r;
		penAxis = a.fuN;
	}

	if (l > r)
		return false;
	//CASE 3
	//L = Az
	l = abs(glm::dot(T, a.ffN));
	r = a.fhwF + b.fhwR * Rzx + b.fhwU * Rzy + b.fhwF * Rzz;
	mtd.z = l - r;

	if (abs(l - r) < abs(smallestPen))
	{
		smallestPen = l - r;
		penAxis = a.ffN;
	}

	if (l > r)
		return false;
	//CASE 4
	//L = Bx
	l = abs(glm::dot(T, b.frN));
	r = b.fhwR + a.fhwR * Rxx + a.fhwU * Rxy + a.fhwF * Rxz;
	mtdb.x = l - r;

	if (abs(l - r) < abs(smallestPen))
	{
		smallestPen = l - r;
		penAxis = a.frN;
	}

	if (l > r)
		return false;
	//CASE 5
	//L = By
	l = abs(glm::dot(T, b.fuN));
	r = b.fhwU + a.fhwR * Ryx + a.fhwU * Ryy + a.fhwF * Ryz;
	mtdb.y = l - r;

	if (abs(l - r) < abs(smallestPen))
	{
		smallestPen = l - r;
		penAxis = a.fuN;
	}

	if (l > r)
		return false;
	//CASE 6
	//L = Bz
	l = abs(glm::dot(T, b.ffN));
	r = b.fhwF + a.fhwR * Rzx + a.fhwU * Rzy + a.fhwF * Rzz;
	mtdb.z = l - r;

	if (abs(l - r) < abs(smallestPen))
	{
		smallestPen = l - r;
		penAxis = a.ffN;
	}

	if (l > r)
		return false;
	//CASE 7
	//L = Ax x Bx
	l = abs(glm::dot(T, a.ffN) * Ryx - glm::dot(T, a.fuN) * Rzx);
	r = a.fhwU * Rzx + a.fhwF * Ryx + b.fhwU * Rxz + b.fhwF * Rxy;
	//if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.frN, b.frN);
	//}

	if (l > r)
		return false;
	//CASE 8
	//L = Ax x By
	l = abs(glm::dot(T, a.ffN) * Ryy - glm::dot(T, a.fuN) * Rzy);
	r = a.fhwU * Rzy + a.fhwF * Ryy + b.fhwR * Rxz + b.fhwF * Rxx;
	if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.frN, b.fuN);
	//}

	if (l > r)
		return false;
	//CASE 9
	//L = Ax x Bz
	l = abs(glm::dot(T, a.ffN) * Ryz - glm::dot(T, a.fuN) * Rzz);
	r = a.fhwU * Rzz + a.fhwF * Ryz + b.fhwR * Rxy + b.fhwU * Rxx;
	//if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.frN, b.ffN);
	//}
	if (l > r)
		return false;
	//CASE 10
	//L = Ay x Bx
	l = abs(glm::dot(T, a.frN) * Rzx - glm::dot(T, a.ffN) * Rxx);
	r = a.fhwU * Rzx + a.fhwF * Rxx + b.fhwU * Ryz + b.fhwF * Ryy;
	//if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.fuN, b.ffN);
	//}
	if (l > r)
		return false;
	//CASE 11
	//L = Ay x By
	l = abs(glm::dot(T, a.frN) * Rzy - glm::dot(T, a.ffN) * Rxy);
	r = a.fhwU * Rzy + a.fhwF * Rxy + b.fhwR * Ryz + b.fhwF * Ryx;
	//if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.fuN, b.fuN);
	//}
	if (l > r)
		return false;
	//CASE 12
	//L = Ay x Bz
	l = abs(glm::dot(T, a.frN) * Rzz - glm::dot(T, a.ffN) * Rxz);
	r = a.fhwU * Rzz + a.fhwF * Rxz + b.fhwR * Ryy + b.fhwU * Ryx;
	//if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.fuN, b.ffN);
	//}
	if (l > r)
		return false;
	//CASE 13
	//L = Az x Bx
	l = abs(glm::dot(T, a.fuN) * Rxx - glm::dot(T, a.frN) * Ryx);
	r = a.fhwR * Ryx + a.fhwU * Rxx + b.fhwU * Rzz + b.fhwF * Rzy;
	//if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.ffN, b.frN);
	//}
	if (l > r)
		return false;
	//CASE 14
	//L = Az x By
	l = abs(glm::dot(T, a.fuN) * Rxy - glm::dot(T, a.frN) * Ryy);
	r = a.fhwR * Ryy + a.fhwU * Rxy + b.fhwR * Rzz + b.fhwF * Rzx;
	//if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.ffN, b.fuN);
	//}
	if (l > r)
		return false;
	//CASE 15
	//L = Az x Bz
	l = abs(glm::dot(T, a.fuN) * Rxz - glm::dot(T, a.frN) * Ryz);
	r = a.fhwR * Ryz + a.fhwU * Rxz + b.fhwR * Rzy + b.fhwU * Rzx;
	//if (abs(l - r) < abs(smallestPen))
	//{
	//	smallestPen = l - r;
	//	penAxis = glm::cross(a.ffN, b.ffN);
	//}
	if (l > r)
		return false;


	//We have collision

	//Old MTD
	if (abs(mtdb.x) < abs(mtd.x))
		mtd.x = mtdb.x; 
	if (abs(mtdb.y) < abs(mtd.y))
		mtd.y = mtdb.y;
	if (abs(mtdb.z) < abs(mtd.z))
		mtd.z = mtdb.z;
	
	//abs(mtd.x) > abs(mtd.y) ? mtd.x = 0 : mtd.y = 0;
	abs(mtd.x) > abs(mtd.z) ? mtd.x = 0 : mtd.z = 0;
	//abs(mtd.y) > abs(mtd.z) && mtd.z != 0 ? mtd.y = 0 : mtd.z = 0;
	

	//new MTD
	//mtd = glm::normalize(penAxis) * smallestPen;

	mtd.x && T.x < 0 ? mtd.x *= -1 : T.x;
	mtd.y && T.y < 0 ? mtd.y *= -1 : T.y;
	mtd.z && T.z < 0 ? mtd.z *= -1 : T.z;



	return true;
}

bool ComponentCollision::checkVs(ComponentCollision* c)
{
	for (int i = 0, s = _cMesh.size(); i < s; i++)
	{
		//iterate through all bones on this object
		for (int j = 0, ss = c->_cMesh.size(); j < ss; j++)
		{
			//iterate through all bones on the other object
			glm::vec3 mtd;
			if (OBBSatCheck(*_cMesh[i], *c->_cMesh[j], mtd))
			{
				printf("%f, %f, %f\n", mtd.x, mtd.y, mtd.z);
				if (type == MOVING && c->type != PROJ)
					_owner->pos += mtd;
				else if (c->type == PROJ)
				{
					//this is where the hit would be done
					if (strcmp(_owner->GetName(), "Target") == 0)
					{
						_owner->health -= c->_owner->dmg;
						_owner->force = c->_owner->dir * 30.f;
						//_owner->scale = glm::vec3(0, 0, 0);
					}
					if (strcmp(c->_owner->GetName(), "PlayerProjectile") == 0)
						ObjectManager::instance().pMap[c->_owner->handle]->alive = false;
					else if (strcmp(c->_owner->GetName(), "EnemyProjectile") == 0)
					{
						if (c->_owner->handle >= ObjectManager::instance().enemyPMap.size())
						{
							int handlol = c->_owner->handle;
							int epmapSize = ObjectManager::instance().enemyPMap.size();
							std::cout << "Thar be a problem here";
						}
						ObjectManager::instance().enemyPMap[c->_owner->handle]->alive = false;
					}
				}
				printf("NOTICE: %s.%s and %s.%s\n", _owner->GetName(), _cMesh[i]->name.c_str(), c->_owner->GetName(), c->_cMesh[j]->name.c_str());
				return true;
			}
		}
	}
	return false;
}

void CollisionManager::checkAll()
{
	//STAGE 1 SCAN NONSTATIC VS STATIC

	for (int i = 0, s = ObjectManager::instance().colMap.size(); i < s; i++)
	{
		GameObject* thisGO = static_cast<GameObject*>(ObjectManager::instance().gMap[ObjectManager::instance().colMap[i]]);
		ComponentCollision* thisCC = static_cast<ComponentCollision*>(thisGO->GetComponent(PHYSICS));
		
		thisCC->updateFrame(thisGO->GetComponent(GRAPHICS)->getBoneInfoRef(), (thisCC->_cMesh[0]->boneNum == -1 ? false : true));

		if (thisCC->type == MOVING)
		{
			for (int j = 0; j < s; j++)
			{
				if (j != i)
				{
					GameObject* thatGO = static_cast<GameObject*>(ObjectManager::instance().gMap[ObjectManager::instance().colMap[j]]);
					ComponentCollision* thatCC = static_cast<ComponentCollision*>(thatGO->GetComponent(PHYSICS));

					thatCC->updateFrame(thatGO->GetComponent(GRAPHICS)->getBoneInfoRef(), (thatCC->_cMesh[0]->boneNum == -1 ? false : true));
					if (thatCC->getOwner()->GetName() == "Target" && thatCC->getOwner()->dir.x != thatCC->getOwner()->dir.x)
					{
						thatCC->getOwner()->dir = glm::vec3(1.f, 0.f, 0.f);
					}
					if (thatCC->type == STATIC)
						thisCC->checkVs(thatCC);
				}
			}
		}   
	}
	
	//STAGE 2.1 ALL VS PROJECTILES

	for (int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
	{
		ObjectManager::instance().pMap[i]->cc->updateFrame(NULL, false);
		
		for (int j = 0, ss = ObjectManager::instance().colMap.size(); j < ss; j++)
		{
			if (j != i)
			{
				GameObject* thatGO = static_cast<GameObject*>(ObjectManager::instance().gMap[ObjectManager::instance().colMap[j]]);
				ComponentCollision* thatCC = static_cast<ComponentCollision*>(thatGO->GetComponent(PHYSICS));
	
				thatCC->updateFrame(thatGO->GetComponent(GRAPHICS)->getBoneInfoRef(), (thatCC->_cMesh[0]->boneNum == -1 ? false : true));
	
				if (thatCC->type == STATIC)
					thatCC->checkVs(ObjectManager::instance().pMap[i]->cc);
				
			}
		}
	}

	//STAGE 2.2 ALL VS ENEMEY PROJECTILES
	for (int i = 0, s = ObjectManager::instance().enemyPMap.size(); i < s; i++)
	{
		ObjectManager::instance().enemyPMap[i]->cc->updateFrame(NULL, false);
		ObjectManager::instance().enemyPMap[i]->cc->getOwner()->handle = i;
		for (int j = 0, ss = ObjectManager::instance().colMap.size(); j < ss; j++)
		{
			if (j != i)
			{
				GameObject* thatGO = static_cast<GameObject*>(ObjectManager::instance().gMap[ObjectManager::instance().colMap[j]]);
				ComponentCollision* thatCC = static_cast<ComponentCollision*>(thatGO->GetComponent(PHYSICS));

				thatCC->updateFrame(thatGO->GetComponent(GRAPHICS)->getBoneInfoRef(), (thatCC->_cMesh[0]->boneNum == -1 ? false : true));

				if (thatCC->type == STATIC && thatGO->GetName() != "Target")
					thatCC->checkVs(ObjectManager::instance().enemyPMap[i]->cc);

			}
		}
	}

	// STAGE 3 PlAYER VS ENEMY PROJECTILES
	{
		if (!playerIsHit)
		{
			for (int i = 0, s = ObjectManager::instance().enemyPMap.size(); i < s; i++)
			{
				ObjectManager::instance().enemyPMap[i]->cc->updateFrame(NULL, FALSE);

				GameObject* playerGO = static_cast<GameObject*>(ObjectManager::instance().gMap[0]);
				ComponentCollision* playerCC = static_cast<ComponentCollision*>(playerGO->GetComponent(PHYSICS));

				playerCC->updateFrame(playerGO->GetComponent(GRAPHICS)->getBoneInfoRef(), (playerCC->_cMesh[0]->boneNum == -1 ? false : true));

				if (ObjectManager::instance().enemyPMap[i]->go->health > 0)
				{
					ObjectManager::instance().enemyPMap[i]->cc->getOwner()->handle = i;
					if (playerCC->checkVs(ObjectManager::instance().enemyPMap[i]->cc))
					{
						shieldHealth -= ObjectManager::instance().enemyPMap[i]->dmg;
						playerIsHit = true;
						hitInvulnTimer = maxInvulnTime;
						shieldRechargeTimer = shieldMaxTimer;
						if (shieldHealth < 0)
							playerGO->health -= ObjectManager::instance().enemyPMap[i]->dmg;
						playerGO->force = ObjectManager::instance().enemyPMap[i]->go->dir * 0.105f;

					}
				}
			}
		}
	}
}
void ComponentCollision::createHitboxRender()
{
	unsigned int s = _cMesh.size();

	_vao = new GLuint[s];
	glGenVertexArrays(s, _vao);

	_vbo = new GLuint[s];

	for (int i = 0; i < s; i++)
	{
		glBindVertexArray(_vao[i]);
		glGenBuffers(1, &_vbo[i]);
		gHitbox hb;
		float sx = 0.1f;// * _owner->scale.x;
		float sy = 0.1f;// * _owner->scale.y;
		float sz = 0.1f;// * _owner->scale.z;
		hb.vertexData = new GLfloat[108]{
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->min.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->min.x * sx, _cMesh[i]->max.y * sy, _cMesh[i]->max.z * sz,
			_cMesh[i]->max.x * sx, _cMesh[i]->min.y * sy, _cMesh[i]->max.z * sz
		};
		gHitboxes.push_back(hb);	
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[i]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBufferData(GL_ARRAY_BUFFER, 108*sizeof(float), gHitboxes[i].vertexData, GL_STATIC_DRAW);
	}	
	glBindVertexArray(0);
	Program::getInstance().createShader("hitbox", GL_VERTEX_SHADER, "shaders/vertex-shader.vert");
	Program::getInstance().createShader("hitbox", GL_FRAGMENT_SHADER, "shaders/fragment-shader.frag");

	_hitboxRender = true;
}

void ComponentCollision::renderHitbox()
{
	if (_hitboxRender)
	{
		//update shader vars
		Program::getInstance().use("hitbox");
		for (int i = 0, s = gHitboxes.size(); i < s; i++)
		{
			glBindVertexArray(_vao[i]);
			glm::mat4 m = glm::translate(glm::mat4(), _owner->pos) * glm::scale(glm::mat4(), _owner->scale) * rotMatrix(_owner->dir, glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1));;
			Program::getInstance().setUniform("model", m);
			Program::getInstance().setUniform("camera", Camera::getInstance().matrix());
			Program::getInstance().setUniform("colour", glm::normalize(_owner->pos));

			//glBindBuffer(GL_ARRAY_BUFFER, _vbo[i]);
			glDrawArrays(GL_LINE_STRIP, 0, 108);
		}
		glBindVertexArray(0);
	}
}