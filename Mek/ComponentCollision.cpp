#include "ComponentCollision.h"
#include "lib\glm\common.hpp"
#include "include\assimp\Importer.hpp"
#include "include\assimp\scene.h"
#include "lib\glm\geometric.hpp"

ComponentCollision::ComponentCollision()
{
	collisionPackage = new CollisionPacket;
	staticObj = true;
}

void ComponentCollision::setCollisionMask(const aiScene* m)
{
	_scene = m;
	//building the collision mesh
	//puts each bone into a struct
	/*float mX, mY, mZ;
	mX = mY = mZ = 0;
	for (int i = 0; i < _scene->mNumMeshes; i++)
	{
		for (int j = 0; j < _scene->mMeshes[i]->mNumBones; j++)
		{
			Bone* b = new Bone;
			float x = _scene->mMeshes[i]->mVertices[_scene->mMeshes[i]->mBones[j]->mWeights->mVertexId].x;
			float y = _scene->mMeshes[i]->mVertices[_scene->mMeshes[i]->mBones[j]->mWeights->mVertexId].y;
			float z = _scene->mMeshes[i]->mVertices[_scene->mMeshes[i]->mBones[j]->mWeights->mVertexId].z;
			abs(x) > mX ? mX = x : x;
			abs(y) > mY ? mY = y : y;
			abs(z) > mZ ? mZ = z : z;
			b->v.push_back(glm::vec3(x, y, z));

			x = _scene->mMeshes[i]->mNormals[_scene->mMeshes[i]->mBones[j]->mWeights->mVertexId].x;
			y = _scene->mMeshes[i]->mNormals[_scene->mMeshes[i]->mBones[j]->mWeights->mVertexId].y;
			z = _scene->mMeshes[i]->mNormals[_scene->mMeshes[i]->mBones[j]->mWeights->mVertexId].z;

			b->n.push_back(glm::vec3(x, y, z));

			b->name = (char*)_scene->mMeshes[i]->mBones[j]->mName.C_Str();
			b->boneNum = j;
			b->parent = _scene->mMeshes[i];
			_cMesh.push_back(b);
		}
	}

	mX > mY ? radius = mX : radius = mY;
	mZ > radius ? radius = mZ : radius;

	radius *= 1.1;*/
}
// takes array of vec3, 8 units wide
bool OBBSatCheck(glm::vec3* aCorn, glm::vec3* bCorn, glm::vec3 axis) {

	// Handles the cross product = {0,0,0} case
	if (axis == glm::vec3())
		return true;

	float aMin = 0;
	float aMax = 0;
	float bMin = 0;
	float bMax = 0;

	// Define two intervals, a and b. Calculate their min and max values
	for (int i = 0; i < 8; i++) {
		float aDist = glm::dot(aCorn[i], axis);
		aMin = (aDist < aMin) ? aDist : aMin;
		aMax = (aDist > aMax) ? aDist : aMax;
		float bDist = glm::dot(bCorn[i], axis);
		bMin = (bDist < bMin) ? bDist : bMin;
		bMax = (bDist > bMax) ? bDist : bMax;
	}

	// One-dimensional intersection test between a and b
	float min, max;
	aMax > bMax ? max = aMax : max = bMax;
	aMin < bMin ? min = aMin : min = bMin;
	float longSpan = max - min;
	float sumSpan = aMax - aMin + bMax - bMin;
	return longSpan < sumSpan; // Change this to <= if you want the case were they are touching but not overlapping, to count as an intersection
}

bool ComponentCollision::checkVs(ComponentCollision* c)
{
	bool ret = false;
	float d = glm::distance(c->_owner->pos, _owner->pos);
	if (d <= c->radius + radius && false)
	{
		//we have spherical collision
		for (int i = 0, s = _cMesh.size(); i < s; i++)
		{
			//iterate through all bones on this object
			for (int j = 0, ss = c->_cMesh.size(); j < ss; j++)
			{
				//iterate through all bones on the other object

				//now to iterate through the axis
				for (int k = 0; k < 15; k++)
				{
					if (OBBSatCheck(&_cMesh[i]->v[0], &c->_cMesh[j]->v[0], glm::cross(_cMesh[i]->n[k], c->_cMesh[i]->n[k])))
					{
						printf("NOTICE: SAT PASS between: %s.%s and %s.%s\n", _owner->GetName(), _cMesh[i]->name, c->_owner->GetName(), c->_cMesh[j]->name);
						ret = true;
						break;
					}
					else if (OBBSatCheck(&_cMesh[i]->v[0], &c->_cMesh[j]->v[0], _cMesh[i]->n[k]))
					{
						printf("NOTICE: SAT PASS between: %s.%s and %s.%s\n", _owner->GetName(), _cMesh[i]->name, c->_owner->GetName(), c->_cMesh[j]->name);
						ret = true;
						break;
					}
					else if (OBBSatCheck(&_cMesh[i]->v[0], &c->_cMesh[j]->v[0], c->_cMesh[j]->n[k]))
					{
						printf("NOTICE: SAT PASS between: %s.%s and %s.%s\n", _owner->GetName(), _cMesh[i]->name, c->_owner->GetName(), c->_cMesh[j]->name);
						ret = true;
						break;
					}
				}

			}
		}
	}
	if (ret)
		printf("NOTICE: Collision between: %s and %s\n", _owner->GetName(), c->_owner->GetName());
	return ret;
}

void ComponentCollision::setCollisionElip(glm::vec3 r)
{
	collisionPackage->eRadius = r;
}

//Yay for collision code
bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc)
{
	glm::vec3 e10 = pb - pa;
	glm::vec3 e20 = pc - pa;
	float a = glm::dot(e10, e10);
	float b = glm::dot(e10, e20);
	float c = glm::dot(e20, e20);
	float ac_bb = (a*c) - (b*b);
	glm::vec3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);
	float d = glm::dot(vp, e10);
	float e = glm::dot(vp, e20);
	float x = (d*c) - (e*b);
	float y = (e*a) - (d*b);
	float z = x + y - ac_bb;
	return (((unsigned int&)(z)& ~((unsigned int&)(x) | (unsigned int&)(y))) & 0x80000000);
}

bool getLowestRoot(float a, float b, float c, float maxR, float* root) {
	// Check if a solution exists
	float determinant = b*b - 4.0f*a*c;
	// If determinant is negative it means no solutions.
	if (determinant < 0.0f) return false;
	// calculate the two roots: (if determinant == 0 then
	// x1==x2 but let�s disregard that slight optimization)
	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);
	// Sort so x1 <= x2
	if (r1 > r2) {
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}
	// Get lowest root:
	if (r1 > 0 && r1 < maxR) {
		*root = r1;
		return true;
	}
	// It is possible that we want x2 - this can happen
	// if x1 < 0
	if (r2 > 0 && r2 < maxR) {
		*root = r2;
		return true;
	}
	// No (valid) solutions
	return false;
}

// Assumes: p1,p2 and p3 are given in ellisoid space:
void checkTriangle(CollisionPacket* colPackage, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	// Make the plane containing this triangle.
	PLANE trianglePlane(p1, p2, p3);
	// Is triangle front-facing to the velocity glm::vec3?
	// We only check front-facing triangles
	// (your choice of course)
	if (trianglePlane.isFrontFacingTo(colPackage->normalizedVelocity))
	{
		// Get interval of plane intersection:
		float t0, t1;
		bool embeddedInPlane = false;
		// Calculate the signed distance from sphere
		// position to triangle plane
		float signedDistToTrianglePlane = trianglePlane.signedDistanceTo(colPackage->basePoint);
		// cache this as we�re going to use it a few times below:
		float normalDotVelocity = glm::dot(trianglePlane.normal, colPackage->velocity);
		// if sphere is travelling parrallel to the plane:
		if (normalDotVelocity == 0.0f)
		{
			if (fabs(signedDistToTrianglePlane) >= 1.0f)
			{
				// Sphere is not embedded in plane.
				// No collision possible:
				return;
			}
			else
			{
				// sphere is embedded in plane.
				// It intersects in the whole range [0..1]
				embeddedInPlane = true;
				t0 = 0.0;
				t1 = 1.0;
			}
		}
		else
		{
			// N dot D is not 0. Calculate intersection interval:
			t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity;
			t1 = (1.0 - signedDistToTrianglePlane) / normalDotVelocity;
			// Swap so t0 < t1
			if (t0 > t1)
			{
				float temp = t1;
				t1 = t0;
				t0 = temp;
			}
			// Check that at least one result is within range:
			if (t0 > 1.0f || t1 < 0.0f)
			{
				// Both t values are outside values [0,1]
				// No collision possible:
				return;
			}
			// Clamp to [0,1]
			if (t0 < 0.0) t0 = 0.0;
			if (t1 < 0.0) t1 = 0.0;
			if (t0 > 1.0) t0 = 1.0;
			if (t1 > 1.0) t1 = 1.0;
		}
		// OK, at this point we have two time values t0 and t1
		// between which the swept sphere intersects with the
		// triangle plane. If any collision is to occur it must
		// happen within this interval.
		glm::vec3 collisionPoint;
		bool foundCollison = false;
		float t = 1.0;
		// First we check for the easy case - collision inside
		// the triangle. If this happens it must be at time t0
		// as this is when the sphere rests on the front side
		// of the triangle plane. Note, this can only happen if
		// the sphere is not embedded in the triangle plane.
		if (!embeddedInPlane) 
		{
			glm::vec3 planeIntersectionPoint = (colPackage->basePoint - trianglePlane.normal) + t0 * colPackage->velocity;
			if (checkPointInTriangle(planeIntersectionPoint, p1, p2, p3))
			{
				foundCollison = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}
		// if we haven�t found a collision already we�ll have to
		// sweep sphere against points and edges of the triangle.
		// Note: A collision inside the triangle (the check above)
		// will always happen before a vertex or edge collision!
		// This is why we can skip the swept test if the above
		// gives a collision!
		if (foundCollison == false)
		{
			// some commonly used terms:
			glm::vec3 velocity = colPackage->velocity;
			glm::vec3 base = colPackage->basePoint;
			float velocitySquaredLength = pow(velocity.length(),2);
			float a, b, c; // Params for equation
			float newT;
			// For each vertex or edge a quadratic equation have to
			// be solved. We parameterize this equation as
			// a*t^2 + b*t + c = 0 and below we calculate the
			// parameters a,b and c for each test.
			// Check against points:
			a = velocitySquaredLength;
			// P1
			b = 2.0*(glm::dot(velocity, base - p1));
			c = (p1 - base).length() - 1.0;
			if (getLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p1;
			}
			// P2
			b = 2.0*(glm::dot(velocity, (base - p2)));
			c = pow((p2 - base).length(),2) - 1.0;
			if (getLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p2;
			}
			// P3
			b = 2.0*glm::dot(velocity, (base - p3));
			c = pow((p3 - base).length(),2) - 1.0;
			if (getLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p3;
			}
			// Check against edges:
			// p1 -> p2:
			glm::vec3 edge = p2 - p1;
			glm::vec3 baseToVertex = p1 - base;
			float edgeSquaredLength = pow(edge.length(),2);
			float edgeDotVelocity = glm::dot(edge,velocity);
			float edgeDotBaseToVertex = glm::dot(edge,baseToVertex);
			// Calculate parameters for equation
			a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
			b = edgeSquaredLength*(2 * glm::dot(velocity,baseToVertex)) - 2.0*edgeDotVelocity*edgeDotBaseToVertex;
			c = edgeSquaredLength*(1 - pow(baseToVertex.length(),2)) + edgeDotBaseToVertex*edgeDotBaseToVertex;
			// Does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT)) {
				// Check if intersection is within line segment:
				float f = (edgeDotVelocity*newT - edgeDotBaseToVertex) / edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					// intersection took place within segment.
					t = newT;
					foundCollison = true;
					collisionPoint = p1 + f*edge;
				}
			}
			// p2 -> p3:
			edge = p3 - p2;
			baseToVertex = p2 - base;
			edgeSquaredLength = pow(edge.length(),2);
			edgeDotVelocity = glm::dot(edge,velocity);
			edgeDotBaseToVertex = glm::dot(edge,baseToVertex);
			a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
			b = edgeSquaredLength*(2 * glm::dot(velocity,baseToVertex)) - 2.0*edgeDotVelocity*edgeDotBaseToVertex;
			c = edgeSquaredLength*(1 - pow(baseToVertex.length(),2)) + edgeDotBaseToVertex*edgeDotBaseToVertex;
			if (getLowestRoot(a, b, c, t, &newT)) {
				float f = (edgeDotVelocity*newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					t = newT;
					foundCollison = true;
					collisionPoint = p2 + f*edge;
				}
			}
			// p3 -> p1:
			edge = p1 - p3;
			baseToVertex = p3 - base;
			edgeSquaredLength = pow(edge.length(),2);
			edgeDotVelocity = glm::dot(edge, velocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);
			a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
			b = edgeSquaredLength*(2 * glm::dot(velocity, baseToVertex)) - 2.0*edgeDotVelocity*edgeDotBaseToVertex;
			c = edgeSquaredLength*(1 - pow(baseToVertex.length(), 2)) + edgeDotBaseToVertex*edgeDotBaseToVertex;
			if (getLowestRoot(a, b, c, t, &newT)) {
				float f = (edgeDotVelocity*newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					t = newT;
					foundCollison = true;
					collisionPoint = p3 + f*edge;
				}
			}
		}
		// Set result:
		if (foundCollison == true)
		{
			// distance to collision: �t� is time of collision
			float distToCollision = t*colPackage->velocity.length();
			// Does this triangle qualify for the closest hit?
			// it does if it�s the first hit or the closest
			if (colPackage->foundCollision == false || distToCollision < colPackage->nearestDistance)
			{
				// Collision information nessesary for sliding
				colPackage->nearestDistance = distToCollision;
				colPackage->intersectionPoint = collisionPoint;
				colPackage->foundCollision = true;
			}
		}
	} // if not backface
}


void ComponentCollision::collideAndSlide(const glm::vec3& vel, const glm::vec3& gravity, int vs)
{
	// Do collision detection:
	collisionPackage->R3Position = _owner->pos;
	collisionPackage->R3Velocity = vel;
	// calculate position and velocity in eSpace
	glm::vec3 eSpacePosition = collisionPackage->R3Position / collisionPackage->eRadius;
	glm::vec3 eSpaceVelocity = collisionPackage->R3Velocity / collisionPackage->eRadius;
	// Iterate until we have our final position.
	collisionRecursionDepth = 0;
	glm::vec3 finalPosition = collideWithWorld(eSpacePosition,eSpaceVelocity, vs);
	// Add gravity pull:
	// To remove gravity uncomment from here .....
	// Set the new R3 position (convert back from eSpace to R3
	collisionPackage->R3Position = finalPosition*collisionPackage->eRadius;
	collisionPackage->R3Velocity = gravity;
	eSpaceVelocity = gravity / collisionPackage->eRadius;
	collisionRecursionDepth = 0;
	finalPosition = collideWithWorld(finalPosition,eSpaceVelocity, vs);
	// ... to here
	// Convert final result back to R3:
	finalPosition = finalPosition*collisionPackage->eRadius;
	// Move the entity
	//MoveTo(finalPosition);
	_owner->pos = finalPosition;
}

// Make sure to update this when it seems fit
const float unitsPerMeter = 100.0f;
glm::vec3 ComponentCollision::collideWithWorld(const glm::vec3& pos, const glm::vec3& vel, int vs)
{
	// All hard-coded distances in this function is
	// scaled to fit the setting above..
	float unitScale = unitsPerMeter / 100.0f;
	float veryCloseDistance = 0.005f * unitScale;
	// do we need to worry?
	if (collisionRecursionDepth > 5)
		return pos;
	// Ok, we need to worry:
	collisionPackage->velocity = vel;
	collisionPackage->normalizedVelocity = glm::normalize(vel);
	collisionPackage->basePoint = pos;
	collisionPackage->foundCollision = false;

	// Check for collision (calls the collision routines)
	CollisionManager::instance().checkCollision(collisionPackage, _owner->handle, vs);
	
	// If no collision we just move along the velocity
	if (collisionPackage->foundCollision == false) {
		return pos + vel;
	}
	// *** Collision occured ***
	printf("COLLIDE %f, %f, %f\n", collisionPackage->intersectionPoint.x, collisionPackage->intersectionPoint.y, collisionPackage->intersectionPoint.z);
	// The original destination point
	glm::vec3 destinationPoint = pos + vel;
	glm::vec3 newBasePoint = pos;
	// only update if we are not already very close
	// and if so we only move very close to intersection..not
	// to the exact spot.
	if (collisionPackage->nearestDistance >= veryCloseDistance)
	{
		glm::vec3 V = vel;
		V = glm::normalize(V) * (collisionPackage->nearestDistance - veryCloseDistance);
		newBasePoint = collisionPackage->basePoint + V;
		// Adjust polygon intersection point (so sliding
		// plane will be unaffected by the fact that we
		// move slightly less than collision tells us)
		V = glm::normalize(V);
		collisionPackage->intersectionPoint -= veryCloseDistance * V;
	}
	// Determine the sliding plane
	glm::vec3 slidePlaneOrigin = collisionPackage->intersectionPoint;
	glm::vec3 slidePlaneNormal = newBasePoint - collisionPackage->intersectionPoint;
	slidePlaneNormal = glm::normalize(slidePlaneNormal);
	PLANE slidingPlane(slidePlaneOrigin, slidePlaneNormal);
	glm::vec3 newDestinationPoint = destinationPoint - slidingPlane.signedDistanceTo(destinationPoint) * slidePlaneNormal;
	// Generate the slide glm::vec3, which will become our new
	// velocity glm::vec3 for the next iteration
	glm::vec3 newVelocityV = newDestinationPoint - collisionPackage->intersectionPoint;
	// Recurse:
	// dont recurse if the new velocity is very small
	if (newVelocityV.length() < veryCloseDistance)
	{
		return newBasePoint;
	}
	collisionRecursionDepth++;
	return collideWithWorld(newBasePoint, newVelocityV, vs);
}


PLANE::PLANE(const glm::vec3& origin, const glm::vec3& normal)
{
	this->normal = normal;
	this->origin = origin;
	equation[0] = normal.x;
	equation[1] = normal.y;
	equation[2] = normal.z;
	equation[3] = -(normal.x*origin.x + normal.y*origin.y
		+ normal.z*origin.z);
}
// Construct from triangle:
PLANE::PLANE(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	normal = glm::cross((p2 - p1), (p3 - p1));
	normal = glm::normalize(normal);
	origin = p1;
	equation[0] = normal.x;
	equation[1] = normal.y;
	equation[2] = normal.z;
	equation[3] = -(normal.x*origin.x + normal.y*origin.y
		+ normal.z*origin.z);
}
bool PLANE::isFrontFacingTo(const glm::vec3& direction) const
{
	float dot = glm::dot(normal, direction);
	return (dot <= 0);
}
float PLANE::signedDistanceTo(const glm::vec3& point) const
{
	return (glm::dot(point, normal)) + equation[3];
}

void ComponentCollision::update()
{
	_owner->pos = collisionPackage->R3Position;
	_owner->vel = collisionPackage->R3Velocity;
}

//FOR COLLISON CHECK:
//CALL collideAndSlide with the null values for each object that can move
//this call will then call the managers checkCollision
//check collision will have to check using checkTriangle and object meshes
//handle passed is the moving object that is asking for a check

/*
void checkCollision(CollisionPacket*, int handle);
void addObject(ComponentCollision*);
*/

void CollisionManager::checkAll()
{
	ComponentCollision* c;
	for (int i = 0, s = ObjectManager::instance().colMap.size(); i < s; i++)
	{
		GameObject* it = ObjectManager::instance().gMap[ObjectManager::instance().colMap[i]];
		if (it->GetComponent(PHYSICS))
		{
			//c is now a pointer to the object we are moving
			c = static_cast<ComponentCollision*>(it->GetComponent(PHYSICS));
			if (c->staticObj == false)
			{
				for (int j = 0, ss = ObjectManager::instance().colMap.size(); j < ss; j++)
				{
					if (j != ObjectManager::instance().colMap[i])
					{
						//at this point i is our object, and j is the object we are checking against
						//static objects from i have already been discarded
						//Time to start cascading the functions
						//This call will inevitably call managers checkCollision
						//we pass j as the vs variable so we know what object we are checking against
						c->update();
						c->collideAndSlide(glm::vec3(0.1, 0, 0), glm::vec3(0, 0, 0), j);
					}
				}
			}
		}
	}
}

bool CollisionManager::checkCollision(CollisionPacket* p, int handle, int vs)
{
	//now for checking p against vs.GRAPHICS.mesh
	//iterate through vs mesh for every 3 verticies in every bone
	ComponentCollision* c = static_cast<ComponentCollision*>(ObjectManager::instance().gMap[ObjectManager::instance().colMap[vs]]->GetComponent(PHYSICS));
	aiMesh* m;
	for (int i = 0, s = c->_scene->mNumMeshes; i < s; i++)
	{
		m = c->_scene->mMeshes[i];
		if (m->mNumBones == 0)
		{
			for (int j = 0, ss = m->mNumVertices; j < ss; j += 3)
			{
				aiVector3D aiA = m->mVertices[j];
				aiVector3D aiB = m->mVertices[j+1];
				aiVector3D aiC = m->mVertices[j+2];

				//Collision isn't working!
				//Suspects:
				//the d vector used to convert the _scene data into worldspace
				//_scene data is in R3 space not eSpace
				//verticies passed to checkTriangle in correct order? (must be clockwise)
				//handle/vs off
				//manager maps off
				//velocity at 0.1 works, but 0.01 dosnt
				std::vector<GameObject*> q = ObjectManager::instance().gMap;
				std::vector<unsigned int> qq = ObjectManager::instance().colMap;
				//have to move all of the verticies by vs.v
				glm::vec3 d = ObjectManager::instance().gMap[ObjectManager::instance().colMap[handle]]->pos;
				//convert to glm because that is the standard
				glm::vec3 a(aiA.x, aiA.y, aiA.z);
				glm::vec3 b(aiB.x, aiB.y, aiB.z);
				glm::vec3 c(aiC.x, aiC.y, aiC.z);
				//do the actual translation
				//a += d;
				//b += d;
				//c += d;

				//no effect
				//d /= p->eRadius;
				//d /= p->eRadius;
				//d /= p->eRadius;
				a += d;
				b += d;
				c += d;
				
				checkTriangle(p, a, b, c);
			}
		}
		else
		{
			for (int j = 0, ss = m->mNumBones; j < ss; j++)
			{
				for (int k = 0, sss = m->mBones[j]->mNumWeights - 3; k < sss; k += 3)
				{
					aiVector3D aiA = m->mVertices[k];
					aiVector3D aiB = m->mVertices[k + 1];
					aiVector3D aiC = m->mVertices[k + 2];
					//have to move all of the verticies by vs.v
					glm::vec3 d = ObjectManager::instance().gMap[ObjectManager::instance().colMap[handle]]->pos;
					//convert to glm because that is the standard
					glm::vec3 a(aiA.x, aiA.y, aiA.z);
					glm::vec3 b(aiB.x, aiB.y, aiB.z);
					glm::vec3 c(aiC.x, aiC.y, aiC.z);
					//do the actual translation
					a += d;
					b += d;
					c += d;

					checkTriangle(p, a, b, c);
				}
			}
		}
	}
	return p->foundCollision;
}

/*
for (int j = 0, ss = c->_scene->mMeshes[i]->mNumBones - 3; j < ss; j += 3)
{
aiVector3D aiA = c->_scene->mMeshes[i]->mVertices[c->_scene->mMeshes[i]->mBones[j]->mWeights->mVertexId];
aiVector3D aiB = c->_scene->mMeshes[i]->mVertices[c->_scene->mMeshes[i]->mBones[j + 1]->mWeights->mVertexId];
aiVector3D aiC = c->_scene->mMeshes[i]->mVertices[c->_scene->mMeshes[i]->mBones[j + 2]->mWeights->mVertexId];

//have to move all of the verticies by vs.v - h.v

glm::vec3 a(aiA.x, aiA.y, aiA.z);
glm::vec3 b(aiB.x, aiB.y, aiB.z);
glm::vec3 c(aiC.x, aiC.y, aiC.z);
checkTriangle(p, a, b, c);
}
*/