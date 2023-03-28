#include "PhysicsFactory.h"

#include "PhysicsWorld.h"

#include "RigidBody.h"
#include "SoftBody.h"

namespace physics
{
	PhysicsFactory::PhysicsFactory(void)
		: iPhysicsFactory()
	{
		printf("Created PhysX Physics Factory!\n");
	}

	PhysicsFactory::~PhysicsFactory(void)
	{
	}

	iPhysicsWorld* PhysicsFactory::CreateWorld(void)
	{
		PhysicsWorld* AWholeNewWorld = new PhysicsWorld();

		return AWholeNewWorld;
	}

	iRigidBody* PhysicsFactory::CreateRigidBody(const RigidBodyDesc& desc, iShape* shape)
	{
		return new RigidBody(desc, shape);
	}

	iSoftBody* PhysicsFactory::CreateSoftBody(const SoftBodyDesc& desc)
	{
		return new SoftBody(desc);
	}
};