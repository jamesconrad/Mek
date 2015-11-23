#include "ComponentCollision.h"
#include "ComponentInput.h"
#include "lib\glm\common.hpp"
#include "include\assimp\Importer.hpp"
#include "include\assimp\scene.h"
#include "lib\glm\geometric.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"

#include "Projectile.h"

ComponentCollision::ComponentCollision()
{
	//collisionPackage = new CollisionPacket;
	type = STATIC;
}

void ComponentCollision::updateFrame(void* boneInfoLocation, bool bones)
{
	if (bones)
	{
		BoneInfo* boneInfo = static_cast<BoneInfo*>(boneInfoLocation);

		for (int i = 0, s = _cMesh.size(); i < s; i++)
		{
			boneInfo[i].FinalTransformation = glm::scale(boneInfo[i].FinalTransformation, glm::vec3(0.1, 0.1, 0.1));
			_cMesh[i]->fmin = glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->min, 0)) + _owner->pos;
			_cMesh[i]->fmax = glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->max, 0)) + _owner->pos;
			_cMesh[i]->fuN = glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->uN, 0));
			_cMesh[i]->frN = glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->rN, 0));
			_cMesh[i]->ffN = glm::vec3(boneInfo[i].FinalTransformation *glm::vec4(_cMesh[i]->fN, 0));
			_cMesh[i]->fc = glm::vec3(boneInfo[i].FinalTransformation * glm::vec4(_cMesh[i]->centre, 0)) + _owner->pos;
		}
	}
	else
	{
		glm::mat4 h = glm::translate(h, _owner->pos);
		h = glm::scale(h, glm::vec3(0.1, 0.1, 0.1));
		_cMesh[0]->fmin = (_cMesh[0]->min * 0.1f + _owner->pos);
		_cMesh[0]->fmax = (_cMesh[0]->max * 0.1f + _owner->pos);
		_cMesh[0]->fuN = _cMesh[0]->uN;
		_cMesh[0]->frN = _cMesh[0]->rN;
		_cMesh[0]->ffN = _cMesh[0]->fN;
		_cMesh[0]->fc = (_cMesh[0]->centre * 0.1f + _owner->pos);
	}
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
				BoneBox* bb = new BoneBox;
				bb->name = bone->mName.C_Str();
				bb->boneNum = boneIndex;

				for (int vertexIndex = 0; vertexIndex < bone->mNumWeights; vertexIndex++)
				{
					if (bone->mWeights[vertexIndex].mWeight > 0.75)
					{
						aiVector3D aiV = m->mMeshes[i]->mVertices[bone->mWeights[vertexIndex].mVertexId];
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
		else
		{
			BoneBox* bb = new BoneBox;
			bb->name = "Boneless Object";
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

	//CASE 1
	//L = Ax
	l = abs(glm::dot(T, a.frN));
	r = a.hwR + b.hwR * Rxx + b.hwU * Rxy + b.hwF * Rxz;
	lr = l - r;
	mtd.x = l - r;
	if (l > r)
		return false;
	//CASE 2
	//L = Ay
	l = abs(glm::dot(T, a.fuN));
	r = a.hwU + b.hwR * Ryx + b.hwU * Ryy + b.hwF * Ryz;
	mtd.y = l - r;
	if (l > r)
		return false;
	//CASE 3
	//L = Az
	l = abs(glm::dot(T, a.ffN));
	r = a.hwF + b.hwR * Rzx + b.hwU * Rzy + b.hwF * Rzz;
	mtd.z = l - r;
	if (l > r)
		return false;
	//CASE 4
	//L = Bx
	l = abs(glm::dot(T, b.frN));
	r = b.hwR + a.hwR * Rxx + a.hwU * Rxy + a.hwF * Rxz;
	mtdb.x = l - r;
	if (l > r)
		return false;
	//CASE 5
	//L = By
	l = abs(glm::dot(T, b.fuN));
	r = b.hwU + a.hwR * Ryx + a.hwU * Ryy + a.hwF * Ryz;
	mtdb.y = l - r;
	if (l > r)
		return false;
	//CASE 6
	//L = Bz
	l = abs(glm::dot(T, b.ffN));
	r = b.hwF + a.hwR * Rzx + a.hwU * Rzy + a.hwF * Rzz;
	mtdb.z = l - r;
	if (l > r)
		return false;
	//CASE 7
	//L = Ax x Bx
	if (abs(glm::dot(T, a.ffN) * Ryx - glm::dot(T, a.fuN) * Rzx) > a.hwU * Rzx + a.hwF * Ryx + b.hwU * Rxz + b.hwF * Rxy)
		return false;
	//CASE 8
	//L = Ax x By
	if (abs(glm::dot(T, a.ffN) * Ryy - glm::dot(T, a.fuN) * Rzy) > a.hwU * Rzy + a.hwF * Ryy + b.hwR * Rxz + b.hwF * Rxx)
		return false;
	//CASE 9
	//L = Ax x Bz
	if (abs(glm::dot(T, a.ffN) * Ryz - glm::dot(T, a.fuN) * Rzz) > a.hwU * Rzz + a.hwF * Ryz + b.hwR * Rxy + b.hwU * Rxx)
		return false;
	//CASE 10
	//L = Ay x Bx
	if (abs(glm::dot(T, a.frN) * Rzx - glm::dot(T, a.ffN) * Rxx) > a.hwU * Rzx + a.hwF * Rxx + b.hwU * Ryz + b.hwF * Ryy)
		return false;
	//CASE 11
	//L = Ay x By
	if (abs(glm::dot(T, a.frN) * Rzy - glm::dot(T, a.ffN) * Rxy) > a.hwU * Rzy + a.hwF * Rxy + b.hwR * Ryz + b.hwF * Ryx)
		return false;
	//CASE 12
	//L = Ay x Bz
	if (abs(glm::dot(T, a.frN) * Rzz - glm::dot(T, a.ffN) * Rxz) > a.hwU * Rzz + a.hwF * Rxz + b.hwR * Ryy + b.hwU * Ryx)
		return false;
	//CASE 13
	//L = Az x Bx
	if (abs(glm::dot(T, a.fuN) * Rxx - glm::dot(T, a.frN) * Ryx) > a.hwR * Ryx + a.hwU * Rxx + b.hwU * Rzz + b.hwF * Rzy)
		return false;
	//CASE 14
	//L = Az x By
	if (abs(glm::dot(T, a.fuN) * Rxy - glm::dot(T, a.frN) * Ryy) > a.hwR * Ryy + a.hwU * Rxy + b.hwR * Rzz + b.hwF * Rzx)
		return false;
	//CASE 15
	//L = Az x Bz
	if (abs(glm::dot(T, a.fuN) * Rxz - glm::dot(T, a.frN) * Ryz) > a.hwR * Ryz + a.hwU * Rxz + b.hwR * Rzy + b.hwU * Rzx)
		return false;

	if (abs(mtdb.x) < abs(mtd.x))
		mtd.x = mtdb.x; 
	if (abs(mtdb.y) < abs(mtd.y))
		mtd.y = mtdb.y;
	if (abs(mtdb.z) < abs(mtd.z))
		mtd.z = mtdb.z;

	abs(mtd.x) > abs(mtd.y) ? mtd.x = 0 : mtd.y = 0;
	abs(mtd.x) > abs(mtd.z) ? mtd.x = 0 : mtd.z = 0;
	abs(mtd.y) > abs(mtd.z) && mtd.z != 0 ? mtd.y = 0 : mtd.z = 0;

	mtd.x && T.x < 0 ? mtd.x *= -1 : T.x;
	mtd.y && T.y < 0 ? mtd.y *= -1 : T.y;
	mtd.z && T.z < 0 ? mtd.z *= -1 : T.z;

	return true;
}

bool ComponentCollision::checkVs(ComponentCollision* c)
{
	bool ret = false;
	for (int i = 0, s = _cMesh.size(); i < s; i++)
	{
		//iterate through all bones on this object
		for (int j = 0, ss = c->_cMesh.size(); j < ss; j++)
		{
			//iterate through all bones on the other object
			glm::vec3 mtd = glm::vec3(0,0,0);
			if (OBBSatCheck(*_cMesh[i], *c->_cMesh[j], mtd))
			{
				//printf("%f, %f, %f\n", mtd.x, mtd.y, mtd.z);
				if (type == MOVING)
					_owner->pos += mtd;
				else if (type == PROJ)
				{
					//this is where the hit would be done
					if (strcmp(c->getOwner()->GetName(), "Target"))
						c->getOwner()->scale = glm::vec3(0, 0, 0);
					ObjectManager::instance().removeProjectile(_owner->handle);
					printf("TARGET HIT\n");
				}
				printf("NOTICE: s.%s and %s.%s\n", _owner->GetName(), _cMesh[i]->name.c_str(), c->_owner->GetName(), c->_cMesh[j]->name.c_str());
				ret = true;
				break;
			}
		}
	}
	return ret;
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

					if (thatCC->type == STATIC)
						thisCC->checkVs(thatCC);
				}
			}
		}
	}
	
	//STAGE 2 SCAN PROJECTILES VS ALL

	for (int i = 0, s = ObjectManager::instance().pMap.size(); i < s; i++)
	{
		
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
}