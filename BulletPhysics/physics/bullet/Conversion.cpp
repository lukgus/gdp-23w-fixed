#include "Conversion.h"

// Shapes
#include <physics/interfaces/iShape.h>
#include <physics/interfaces/BoxShape.h>
#include <physics/interfaces/SphereShape.h>
#include <physics/interfaces/PlaneShape.h>
#include <physics/interfaces/CylinderShape.h>
#include <physics/interfaces/CapsuleShape.h>

// Constraints
#include <physics/interfaces/iConstraint.h>
#include <physics/interfaces/Point2PointConstraint.h>

// Bodies
#include "RigidBody.h"
#include "Vehicle.h"
#include "CharacterController.h"

namespace physics
{
	void CastGLMQuat(const btQuaternion& in, glm::quat* out)
	{
		CastFloat(in.x(), &out->x);
		CastFloat(in.y(), &out->y);
		CastFloat(in.z(), &out->z);
		CastFloat(in.w(), &out->w);
	}

	void CastGLMVec3(const btVector3& in, glm::vec3* out)
	{
		CastFloat(0.0f, &out->p);
		CastFloat(0.0f, &out->r);
		CastFloat(0.0f, &out->s);
		CastFloat(0.0f, &out->t);
		CastFloat(in.x(), &out->x);
		CastFloat(in.y(), &out->y);
		CastFloat(in.z(), &out->z);
	}

	void CastFloat(const btScalar& in, float* out)
	{
		*out = in;
	}

	void CastGLMMat4(const btTransform& in, glm::mat4* out)
	{
	}

	void CastBulletQuaternion(const glm::quat& in, btQuaternion* out)
	{
		out->setX(in.x);
		out->setY(in.y);
		out->setZ(in.z);
		out->setW(in.w);
	}

	void CastBulletVector3(const glm::vec3& in, btVector3* out)
	{
		out->setX(in.x);
		out->setY(in.y);
		out->setZ(in.z);
	}

	void CastBulletScalar(const float in, btScalar* out)
	{
		*out = in;
	}

	btCollisionShape* CastBulletShape(iShape* shape)
	{
		switch (shape->GetShapeType())
		{
		case ShapeType::Ghost:
		{
			btEmptyShape* btEmpty = new btEmptyShape();

			return btEmpty;
		} break;

		case ShapeType::Box:
		{
			BoxShape* box = BoxShape::Cast(shape);

			btVector3 halfExtents;

			CastBulletVector3(box->GetHalfExtents(), &halfExtents);

			btBoxShape* btBox = new btBoxShape(halfExtents);

			return btBox;
		} break;

		case ShapeType::Plane:
		{
			PlaneShape* plane = PlaneShape::Cast(shape);

			btVector3 normal;
			btScalar planeConstant;

			CastBulletVector3(plane->GetNormal(), &normal);
			CastBulletScalar(plane->GetDotProduct(), &planeConstant);

			btStaticPlaneShape* btPlane = new btStaticPlaneShape(normal, planeConstant);

			return btPlane;
		} break;

		case ShapeType::Sphere:
		{
			SphereShape* sphere = SphereShape::Cast(shape);

			btScalar radius;

			CastBulletScalar(sphere->GetRadius(), &radius);

			btSphereShape* btSphere = new btSphereShape(radius);

			return btSphere;

		} break;

		case ShapeType::Cylinder:
		{
			CylinderShape* mesh = CylinderShape::Cast(shape);

			btVector3 btHalfExtents;

			CastBulletVector3(mesh->GetHalfExtents(), &btHalfExtents);

			btCylinderShape* btCylinder = new btCylinderShape(btHalfExtents);

			return btCylinder;
		} break;

		//case ShapeType::TriangleMesh:
		//{
		//	TriangleMeshShape* mesh = TriangleMeshShape::Cast(shape);

		//	btTriangleMesh* btMesh = new btTriangleMesh();

		//	unsigned int numTriangles = mesh->GetNumTriangles();
		//	glm::vec3 a;
		//	glm::vec3 b;
		//	glm::vec3 c;
		//	btVector3 btA;
		//	btVector3 btB;
		//	btVector3 btC;
		//	for (unsigned int triIndex = 0; triIndex != numTriangles; triIndex++)
		//	{
		//		mesh->GetTriangleAt(triIndex, &a, &b, &c);

		//		CastBulletVector3(a, &btA);
		//		CastBulletVector3(b, &btB);
		//		CastBulletVector3(c, &btC);

		//		btMesh->addTriangle(btA, btB, btC);
		//	}

		//	btBvhTriangleMeshShape* btShape = new btBvhTriangleMeshShape(btMesh, false);
		//	return btShape;
		//} break;

		default:

			break;
		}

		return NULL;
	}

	btConvexShape* CastBulletConvexShape(iConvexShape* shape)
	{

		switch (shape->GetShapeType())
		{
		case ShapeType::Capsule:
		{
			CapsuleShape* capsule = CapsuleShape::Cast(shape);

			btScalar radius;
			btScalar height;

			CastBulletScalar(capsule->GetRadius(), &radius);
			CastBulletScalar(capsule->GetHeight(), &height);

			btCapsuleShape* btCapsule = new btCapsuleShape(radius, height);

			return btCapsule;
		}
		break;

		default:
			// Not a valid shape
			assert(0);
			break;
		}

		return nullptr;
	}

	btRigidBody* CastBulletRigidBody(iCollisionBody* body)
	{
		if (body == nullptr)
			return nullptr;

		return dynamic_cast<RigidBody*>(body)->GetBulletBody();
	}

	btRaycastVehicle* CastBulletVehicle(iVehicle* vehicle)
	{
		return dynamic_cast<Vehicle*>(vehicle)->GetBulletVehicle();
	}

	btCharacterControllerInterface* CastBulletCharacterController(
		iCharacterController* characterController)
	{
		return dynamic_cast<CharacterController*>(characterController)->GetBulletCharacterController();
	}

	btTypedConstraint* CastBulletConstraint(iConstraint* constraint)
	{
		ConstraintType type = constraint->GetConstraintType();

		switch (type)
		{
		case physics::ConstraintType::ConeTwist:
			assert(0); // not implemented
			break;

		case physics::ConstraintType::Gear:
			assert(0); // not implemented
			break;

		case physics::ConstraintType::Hinge:
			assert(0); // not implemented
			break;

		case physics::ConstraintType::Point2Point:
		{
			Point2PointConstraint* p2p = Point2PointConstraint::Cast(constraint);

			btRigidBody* btBodyA = CastBulletRigidBody(p2p->GetRididBodyA());
			btRigidBody* btBodyB = CastBulletRigidBody(p2p->GetRididBodyB());

			btVector3 btVecA;
			btVector3 btVecB;
			CastBulletVector3(p2p->GetPointA(), &btVecA);
			CastBulletVector3(p2p->GetPointB(), &btVecB);

			btPoint2PointConstraint* btP2P;
			if (btBodyB == nullptr)
			{
				btP2P = new btPoint2PointConstraint(*btBodyA, btVecA);
			}
			else
			{
				btP2P = new btPoint2PointConstraint(*btBodyA, *btBodyB, btVecA, btVecB);
			}

			return btP2P;

			break;
		}
		case physics::ConstraintType::Slider:
			assert(0); // not implemented
			break;

		default:
			assert(0); // Don't know what this is
			break;
		}


		assert(0); // Shouldn't get here
		return nullptr;
	}
}