#include "CollisionHandler.h"

#include "RigidBody.h"

#include <glm/gtx/projection.hpp>


namespace physics
{
	// Page 224 Chapter 5 Basic Primitive Tests
	// Christer Ericson - Real-time collision detection
	bool TestMovingSphereSphere(
		const Vector3& s0Center, const float s0Radius,
		const Vector3& s1Center, const float s1Radius,
		const Vector3& v0, const Vector3& v1, float& t)
	{
		Vector3 s = s1Center - s0Center;	// Vector between sphere centers
		Vector3 v = v1 - v0;				// Relative motion of s1 with respect to stationary s0
		float r = s1Radius + s0Radius;		// Sum of sphere radii
		float c = glm::dot(s, s) - r * r;

		if (c < 0.f) {
			// Already intersecting, not moving towards a collision
			t = 0.f;
			return true;
		}

		float a = glm::dot(v, v);
		if (a < std::numeric_limits<float>::epsilon())
			return false;		// Spheres not moving relative to eachother

		float b = glm::dot(v, s);
		if (b >= 0.f)
			return false;		// Spheres not moving towards eachother

		float d = (b * b) - (a * c);
		if (d < 0.f)
			return false;		// No real-valued root, spheres do not intersect

		t = (-b - sqrt(d)) / a;
		return true;
	}

	// Page #? Chapter 5 Basic Primitive Tests
	// Christer Ericson - Real-time collision detection
	bool TestMovingSpherePlane(const Vector3& prevPosition, const Vector3& currPosition, float radius, const Vector3& norm, float dotProduct)
	{
		// Get distance for both a and b from the plane
		float adist = glm::dot(prevPosition, norm) - dotProduct;
		float bdist = glm::dot(currPosition, norm) - dotProduct;

		// Intersects if on different sides of a plane (distance have different signs)
		if (adist * bdist < 0.f)
			return true;

		// Intersects if start or end position within radius from plane
		if (abs(adist) < radius || abs(bdist) <= radius)
			return true;

		// No intersection
		return false;
	}

	CollisionHandler::CollisionHandler()
	{

	}

	CollisionHandler::~CollisionHandler()
	{

	}

	// From Christer Ericson book
	glm::vec3 CollisionHandler::ClosestPtPointPlane(const glm::vec3& pt, const glm::vec3& planeNormal, float planeDotProduct)
	{
		float t = glm::dot(planeNormal, pt) - planeDotProduct;
		return pt - t * planeNormal;
	}

	bool CollisionHandler::CollideSphereSphere(float dt, RigidBody* bodyA, SphereShape* sphereA,
		RigidBody* bodyB, SphereShape* sphereB)
	{
		if (bodyA->IsStatic() && bodyB->IsStatic())
			return false;

		// TestMovingSphereSphere
		// if There is no a collision detected we will return false
		Vector3 v0 = bodyA->m_Position - bodyA->m_PreviousPosition;
		Vector3 v1 = bodyB->m_Position - bodyB->m_PreviousPosition;
		float t = 0.f;

		if (!TestMovingSphereSphere(
			bodyA->m_PreviousPosition, // We want PreviousPosition
			sphereA->GetRadius(),
			bodyB->m_PreviousPosition, // We want PreviousPosition
			sphereB->GetRadius(),
			v0, v1, t))
		{
			// There is no collision or future collision
			return false;
		}

		if (t > dt) {
			// Collision will not happen this time step
			return false;
		}

		// Otherwise, we will handle the collision here

		// calculate what percent of DT to reverse the Verlet Step

		// This might be wrong...
		float fractDT = t / (glm::length(v0) + glm::length(v1));
		float revDT = (1.0f - fractDT) * dt;
		bodyA->VerletStep1(-revDT);
		bodyB->VerletStep1(-revDT);

		// p = mv
		// p: momentum
		// m: mass
		// v: velocity

		// Total Mass & Momentum calculation
		float totalMass = bodyB->m_Mass + bodyA->m_Mass;
		float aFactor = bodyB->m_Mass / totalMass;
		float bFactor = bodyA->m_Mass / totalMass;

		Vector3 aMomentum = bodyA->m_LinearVelocity * bodyA->m_Mass;
		Vector3 bMomentum = bodyB->m_LinearVelocity * bodyB->m_Mass;
		Vector3 totalMomentum = aMomentum + bMomentum;

		aMomentum = totalMomentum * aFactor;
		bMomentum = totalMomentum * bFactor;

		// Check if any part of the objects are still overlapping.
		// We need to nudge the objects back more if they are still overlapping.
		Vector3 vec = bodyB->m_Position - bodyA->m_Position;
		float vecLength = glm::length(vec);
		float overlap = vecLength - (sphereA->GetRadius() + sphereB->GetRadius());

		if (overlap < std::numeric_limits<float>::epsilon())
		{
			Vector3 overlapVec = vec;
			overlapVec = glm::normalize(overlapVec);
			overlapVec *= -overlap;

			// Nudge out of the collision a bit more
			if (!bodyA->m_IsStatic) bodyA->m_Position -= overlapVec * aFactor;
			if (!bodyB->m_IsStatic) bodyB->m_Position += overlapVec * bFactor;

			vec = bodyB->m_Position - bodyA->m_Position;
			vecLength = glm::length(vec);
		}
		vec /= vecLength;

		// Calculate elastic and inelastic momentum for both bodyA and bodyB
		float elasticity = 0.4f;

		Vector3 aElasticMomentum = vec * (glm::length(aMomentum) * elasticity);
		Vector3 aInelasticMomentum = vec * glm::length(aMomentum) * (1.f - elasticity);

		Vector3 bElasticMomentum = vec * (glm::length(bMomentum) * elasticity) * -1.f;
		Vector3 bInelasticMomentum = vec * glm::length(bMomentum) * (1.f - elasticity);

		// Finally use our elastic & inelastic momentums to calculate a new velocity
		bodyA->m_LinearVelocity -= (aElasticMomentum + aInelasticMomentum) * bodyA->m_InvMass * bodyA->m_Restitution;
		bodyB->m_LinearVelocity += (bElasticMomentum + bInelasticMomentum) * bodyB->m_InvMass * bodyB->m_Restitution;

		// Verlet integrate
		bodyA->VerletStep1(revDT);
		bodyB->VerletStep1(revDT);

		return true;
	}


	bool CollisionHandler::CollideSpherePlane(float dt, RigidBody* sphere, SphereShape* sphereShape,
		RigidBody* plane, PlaneShape* planeShape)
	{
		// TestMovingSpherePlane
		if (!TestMovingSpherePlane(sphere->m_PreviousPosition, sphere->m_Position, sphereShape->GetRadius(),
			planeShape->GetNormal(), planeShape->GetDotProduct()))
		{
			return false;
		}

		glm::vec3 closestPoint = ClosestPtPointPlane(sphere->m_Position, planeShape->GetNormal(), planeShape->GetDotProduct());
		glm::vec3 overlapVector = closestPoint - sphere->m_Position;
		float overlapLength = glm::length(overlapVector);
		float linearVelocityLength = glm::length(sphere->m_LinearVelocity);
		float angularVelocityLength = glm::length(sphere->m_AngularVelocity);

		if (linearVelocityLength > 0.f || angularVelocityLength > 0.f)
		{
			float velocity = glm::length(sphere->m_LinearVelocity);
			float fractDt = 0.f;
			if (velocity != 0.0f)
			{
				fractDt = sphereShape->GetRadius() * ((sphereShape->GetRadius() / overlapLength) - 1.0f) / velocity;
			}
			float partialDt = (1.f - fractDt) * dt;

			// Reverse the sphere out of the plane
			sphere->VerletStep1(-partialDt);

			// calculate the reflection (Bounce) off the plane
			glm::vec3 prevVelocity = sphere->m_LinearVelocity;
			glm::vec3 reflect = glm::reflect(sphere->m_LinearVelocity, planeShape->GetNormal());
			sphere->m_LinearVelocity = reflect;

			// calculate impact info
			glm::vec3 impactComponent = glm::proj(sphere->m_LinearVelocity, planeShape->GetNormal());
			glm::vec3 impactTangent = sphere->m_LinearVelocity - impactComponent;


			glm::vec3 relativePoint = glm::normalize(closestPoint - sphere->m_Position) * sphereShape->GetRadius();
			float surfaceVelocity = sphereShape->GetRadius() * glm::length(sphere->m_AngularVelocity);
			glm::vec3 rotationDirection = glm::normalize(glm::cross(relativePoint - sphere->m_Position, sphere->m_AngularVelocity));



			// Detect if we are bouncing off the plane, or "moving" along it.
			if (glm::dot(impactTangent, planeShape->GetNormal()) > 0.f)
			{
				sphere->ApplyImpulseAtPoint(-2.f * impactComponent * sphere->m_Mass, relativePoint);
				//sphere->ApplyImpulseAtPoint( , relativePoint);
			}
			else
			{
				glm::vec3 impactForce = impactTangent * -1.f * sphere->m_Mass * plane->m_Friction;
				sphere->ApplyForceAtPoint(impactForce, relativePoint);
			}

			if (glm::dot(sphere->m_LinearVelocity, planeShape->GetNormal()) == 0.0f)
			{
				glm::vec3 force = surfaceVelocity * rotationDirection * sphere->m_Mass * plane->m_Friction;
				sphere->ApplyForce(force);
			}

			sphere->UpdateAcceleration();

			// Move the sphere into the new direction
			sphere->VerletStep1(partialDt);

			//if (glm::length(impactTangent) > 0.001f)
			//{
			//	sphere->m_Velocity += impactTangent * 0.1f;
			//}

			// Here we ensure we are on the right side of the plane
			closestPoint = ClosestPtPointPlane(sphere->m_Position, planeShape->GetNormal(), planeShape->GetDotProduct());
			overlapVector = closestPoint - sphere->m_Position;
			overlapLength = glm::length(overlapVector);
			if (overlapLength < sphereShape->GetRadius())
			{
				// we are still colliding!!!

				sphere->m_Position += planeShape->GetNormal() * (sphereShape->GetRadius() - overlapLength);

				float velocityDotNormal = glm::dot(sphere->m_LinearVelocity, planeShape->GetNormal());

				if (velocityDotNormal < 0.f)
				{
					sphere->m_LinearVelocity -= planeShape->GetNormal() * velocityDotNormal;
				}
			}
			else
			{
				sphere->m_LinearVelocity *= sphere->m_Restitution;
			}

		}
		else
		{
			return false;
		}

		return true;
	}


	void CollisionHandler::Collide(float dt, std::vector<iCollisionBody*>& bodies, std::vector<CollidingBodies>& collisions)
	{
		int bodyCount = bodies.size();
		bool collision = false;
		for (int idxA = 0; idxA < bodyCount - 1; idxA++)
		{
			iCollisionBody* bodyA = bodies[idxA];

			for (int idxB = idxA + 1; idxB < bodyCount; idxB++)
			{
				iCollisionBody* bodyB = bodies[idxB];

				collision = false;

				// CHECK WHICH BODY TYPES...
				if (bodyA->GetBodyType() == BodyType::RigidBody)
				{
					if (bodyB->GetBodyType() == BodyType::RigidBody)
					{
						collision = CollideRigidRigid(dt, RigidBody::Cast(bodyA), RigidBody::Cast(bodyB));
					}
					else if (bodyB->GetBodyType() == BodyType::SoftBody)
					{
						collision = CollideRigidSoft(dt, RigidBody::Cast(bodyA), SoftBody::Cast(bodyB));
					}
					else
					{
						// We don't know this type of body
					}
				}
				else if (bodyA->GetBodyType() == BodyType::SoftBody)
				{
					if (bodyB->GetBodyType() == BodyType::RigidBody)
					{
						collision = CollideRigidSoft(dt, RigidBody::Cast(bodyB), SoftBody::Cast(bodyA));
					}
					else if (bodyB->GetBodyType() == BodyType::SoftBody)
					{
						collision = CollideSoftSoft(dt, SoftBody::Cast(bodyA), SoftBody::Cast(bodyB));
					}
					else
					{
						// We don't know this type of body
					}
				}
				else
				{
					// We don't know this type of body
				}


				if (collision)
				{
					collisions.push_back(CollidingBodies(bodyA, bodyB));
				}
			}
		}
	}

	bool CollisionHandler::CollideRigidRigid(float dt, RigidBody* rigidA, RigidBody* rigidB)
	{
		iShape* shapeB = rigidA->GetShape();
		iShape* shapeA = rigidB->GetShape();

		bool collision = false;

		if (shapeA->GetShapeType() == ShapeType::Sphere)
		{
			if (shapeB->GetShapeType() == ShapeType::Sphere)
			{
				collision = CollideSphereSphere(dt, rigidA, SphereShape::Cast(shapeA), rigidB, SphereShape::Cast(shapeB));
			}
			else if (shapeB->GetShapeType() == ShapeType::Plane)
			{
				collision = CollideSpherePlane(dt, rigidA, SphereShape::Cast(shapeA), rigidB, PlaneShape::Cast(shapeB));
			}
			else if (shapeB->GetShapeType() == ShapeType::Box)
			{
				// CollideSphereBox(dt, bodyA, SphereShape::Cast(shapeA), bodyB, BoxShape::Cast(shapeB));
			}
			else
			{
				// We don't have this handled at the moment.
			}
		}
		else if (shapeA->GetShapeType() == ShapeType::Plane)
		{
			if (shapeB->GetShapeType() == ShapeType::Sphere)
			{
				collision = CollideSpherePlane(dt, rigidB, SphereShape::Cast(shapeB), rigidA, PlaneShape::Cast(shapeA));
			}
			else if (shapeB->GetShapeType() == ShapeType::Plane)
			{
				// nope...
			}
			else
			{
				// We don't have this handled at the moment.
			}
		}
		else
		{
			// We don't have this handled at the moment.
		}

		return collision;
	}

	bool CollisionHandler::CollideRigidSoft(float dt, RigidBody* rigidA, SoftBody* softB)
	{
		iShape* shapeA = rigidA->m_Shape;

		bool collision = false;

		if (shapeA->GetShapeType() == ShapeType::Sphere)
		{
			unsigned int numNodes = softB->GetNumNodes();
			for (unsigned int i = 0; i < numNodes; i++)
			{
				if (CollideSphereSphere(dt, rigidA, SphereShape::Cast(shapeA),
					softB->m_Nodes[i], SphereShape::Cast(softB->m_Nodes[i]->m_Shape)))
				{
					collision = true;
				}
			}
		}
		else if (shapeA->GetShapeType() == ShapeType::Plane)
		{
			unsigned int numNodes = softB->GetNumNodes();
			for (unsigned int i = 0; i < numNodes; i++)
			{
				if (CollideSpherePlane(dt,
					softB->m_Nodes[i], SphereShape::Cast(softB->m_Nodes[i]->m_Shape),
					rigidA, PlaneShape::Cast(shapeA)))
				{
					collision = true;
				}
			}
		}
		else
		{
			// Implement more shapes!
		}

		return collision;
	}

	bool CollisionHandler::CollideSoftSoft(float dt, SoftBody* softA, SoftBody* softB)
	{
		glm::vec3 minBoundsA = softA->GetMinBounds();
		glm::vec3 maxBoundsA = softA->GetMaxBounds();
		glm::vec3 minBoundsB = softB->GetMinBounds();
		glm::vec3 maxBoundsB = softB->GetMaxBounds();

		if (maxBoundsA.x < minBoundsB.x) return false;
		if (maxBoundsA.y < minBoundsB.y) return false;
		if (maxBoundsA.z < minBoundsB.z) return false;

		if (minBoundsA.x > maxBoundsB.x) return false;
		if (minBoundsA.y > maxBoundsB.y) return false;
		if (minBoundsA.z > maxBoundsB.z) return false;

		return true;
	}

	void CollisionHandler::CollideInternalSoftBody(float dt, SoftBody* softBody)
	{
		unsigned int numNodes = softBody->m_Nodes.size();

		for (unsigned int idxA = 0; idxA < numNodes - 1; idxA++)
		{
			SoftBodyNode* nodeA = softBody->m_Nodes[idxA];
			for (unsigned int idxB = idxA + 1; idxB < numNodes; idxB++)
			{
				SoftBodyNode* nodeB = softBody->m_Nodes[idxB];

				if (!nodeA->IsNeighbour(nodeB))
				{
					CollideSphereSphere(dt, nodeA, SphereShape::Cast(nodeA->GetShape()), nodeB, SphereShape::Cast(nodeB->GetShape()));
				}
			}
		}
		softBody->UpdateBoundaries();
	}
}
