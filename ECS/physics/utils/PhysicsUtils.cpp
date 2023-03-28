#include "PhysicsUtils.h"

#include <physics/physicsworld.h>


PhysicsWorld* PhysicsUtils::s_ActivePhysicsWorld;

PhysicsWorld* PhysicsUtils::GetActiveWorld()
{
	return s_ActivePhysicsWorld;
}

void PhysicsUtils::SetActiveWorld(PhysicsWorld* world)
{
	s_ActivePhysicsWorld = world;
}