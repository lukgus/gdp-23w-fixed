#pragma once

#include "Math.h"
#include "iCollisionBody.h"
#include "iVehicle.h"
#include "iCharacterController.h"
#include "iConstraint.h"

namespace physics
{
	class iCollisionListener
	{
	public:
		virtual ~iCollisionListener() {}

		virtual void NotifyCollision(iCollisionBody* bodyA, iCollisionBody* bodyB) = 0;

	protected:
		iCollisionListener() {}
	};

	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual void SetGravity(const Vector3& gravity) = 0;

		virtual void AddBody(iCollisionBody* body) = 0;
		virtual void RemoveBody(iCollisionBody* body) = 0;

		virtual void AddVehicle(iVehicle* vehicle) = 0;
		virtual void RemoveVehicle(iVehicle* vehicle) = 0;

		virtual void AddCharacterController(iCharacterController* characterController) = 0;
		virtual void RemoveCharacterController(iCharacterController* characterController) = 0;

		virtual void AddConstraint(iConstraint* constraint) = 0;
		virtual void RemoveConstraint(iConstraint* constraint) = 0;

		virtual void TimeStep(float dt) = 0;

		virtual void DebugDraw() { }
		virtual void RegisterCollisionListener(iCollisionListener* listener) = 0;

	protected:
		iPhysicsWorld() {}

	private:
		iPhysicsWorld(const iPhysicsWorld&) {}
		iPhysicsWorld& operator=(const iPhysicsWorld&) {
			return *this;
		}
	};
}