#pragma once

#include <physics/interfaces/iPhysicsWorld.h>

#include "RigidBody.h"
#include "SoftBody.h"
#include "Vehicle.h"

#include <vector>

#include <btBulletDynamicsCommon.h>

namespace physics
{
	class OverlappingPairCallback : public btOverlappingPairCallback
	{
	public:
		virtual btBroadphasePair* addOverlappingPair(
			btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) override
		{
			return nullptr;
		}

		virtual void* removeOverlappingPair(btBroadphaseProxy* proxy0,
			btBroadphaseProxy* proxy1, btDispatcher* dispatcher) override
		{
			return nullptr;
		}

		virtual void removeOverlappingPairsContainingProxy(
			btBroadphaseProxy* proxy0, btDispatcher* dispatcher) override
		{
		}
	};

	class PhysicsWorld : public iPhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		virtual void SetGravity(const Vector3& gravity) override;

		virtual void AddBody(iCollisionBody* body) override;
		virtual void RemoveBody(iCollisionBody* body) override;

		virtual void AddVehicle(iVehicle* vehicle) override;
		virtual void RemoveVehicle(iVehicle* vehicle) override;

		virtual void AddCharacterController(iCharacterController* characterController) override;
		virtual void RemoveCharacterController(iCharacterController* characterController) override;

		virtual void AddConstraint(iConstraint* constraint) override;
		virtual void RemoveConstraint(iConstraint* constraint) override;

		virtual void TimeStep(float dt) override;

		virtual void DebugDraw() override;
		virtual void RegisterCollisionListener(iCollisionListener* listener) override {}

		btDefaultVehicleRaycaster* GetVehicleRaycaster();
		btDiscreteDynamicsWorld* GetDynamicsWorld() { return m_DynamicsWorld; }
	private:
		btDefaultCollisionConfiguration* m_CollisionConfiguration;
		btCollisionDispatcher* m_Dispatcher;
		btBroadphaseInterface* m_Broadphase;
		btSequentialImpulseConstraintSolver* m_Solver;
		btDiscreteDynamicsWorld* m_DynamicsWorld;
		OverlappingPairCallback* m_OverlappingPairCallback;

		btDefaultVehicleRaycaster* m_VehicleRaycaster;

		btIDebugDraw* m_DebugDrawer;

		PhysicsWorld(const PhysicsWorld&) {}
		PhysicsWorld& operator=(const PhysicsWorld&) {
			return *this;
		}
	};
}
