#include "PhysicsFactory.h"

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "SoftBody.h"
#include "Vehicle.h"
#include "CharacterController.h"
#include "Conversion.h"

#include <bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace physics
{
	PhysicsFactory::PhysicsFactory()
		: iPhysicsFactory()
	{
		printf("Created Bullet Physics Factory!\n");
	}

	PhysicsFactory::~PhysicsFactory()
	{ 
	}

	iPhysicsWorld* PhysicsFactory::CreateWorld()
	{
		PhysicsWorld* world = new PhysicsWorld();

		m_VehicleRaycaster = world->GetVehicleRaycaster();
		m_DynamicsWorld = world->GetDynamicsWorld();
		return world;
	}

	iRigidBody* PhysicsFactory::CreateRigidBody(const RigidBodyDesc& desc, iShape* shape)
	{
		return new RigidBody(desc, shape);
	}

	iSoftBody* PhysicsFactory::CreateSoftBody(const SoftBodyDesc& desc)
	{
		return new SoftBody(desc);
	}

	iVehicle* PhysicsFactory::CreateVehicle(iRigidBody* chassis)
	{
		// We may need to create our Tuning values here
		btRigidBody* btChassis = CastBulletRigidBody(chassis);
		return new Vehicle(btChassis, m_VehicleRaycaster);
	}

	iCharacterController* PhysicsFactory::CreateCharacterController(
		iConvexShape* shape, float stepHeight, const Vector3& up)
	{
		btPairCachingGhostObject* btGhostObject = new btPairCachingGhostObject();
		btConvexShape* btShape;
		btScalar btStepHeight;
		btVector3 btUp;

		btShape = CastBulletConvexShape(shape);
		CastBulletScalar(stepHeight, &btStepHeight);
		CastBulletVector3(up, &btUp);

		iCharacterController* cc = new CharacterController(btGhostObject, btShape, btStepHeight, btUp);

		// Do this after Creating the Character Controller
		btGhostObject->setCollisionShape(btShape);
		btGhostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-5, 5, -5)));

		return cc;
	}
}