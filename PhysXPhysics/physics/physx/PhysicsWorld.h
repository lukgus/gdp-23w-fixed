#pragma once
#include <vector>

// Physics Interfaces
#include <physics/interfaces/iPhysicsWorld.h>
#include <physics/interfaces/iCollisionBody.h>
#include <physics/interfaces/iRigidBody.h>

#include <glm/vec3.hpp>

#include <physx/PxPhysicsAPI.h>
#include <physx/foundation/PxFoundation.h>
#include <physx/foundation/PxErrorCallback.h>
#include <physx/foundation/PxAllocator.h>
#include <physx/foundation/PxAllocatorCallback.h>

using namespace physx;

namespace physics
{
	class UserErrorCallback : public PxErrorCallback
	{
	public:
		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
		{
		}
	};

	class UserAllocatorCallback : public PxAllocatorCallback
	{
	public:
		virtual ~UserAllocatorCallback() {}
		virtual void* allocate(size_t size, const char* typeName, const char* filename,
			int line)
		{
			return 0;
		}
		virtual void deallocate(void* ptr)
		{
		}
	};

	class PhysicsWorld : public iPhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		virtual void TimeStep(float dt) override;

		virtual void SetGravity(const Vector3& gravity) override;

		virtual void AddBody(iCollisionBody* body) override;
		virtual void RemoveBody(iCollisionBody* body) override;

		virtual void RegisterCollisionListener(iCollisionListener* listener) override
		{
			m_CollisionListener = listener;
		}

	private:
		PxDefaultAllocator m_DefaultAllocatorCallback;
		PxDefaultErrorCallback m_DefaultErrorCallback;
		PxFoundation* m_Foundation = nullptr;
		PxPhysics* m_Physics = nullptr;
		PxDefaultCpuDispatcher* m_Dispatcher = nullptr;
		PxScene* m_Scene = nullptr;
		PxMaterial* m_Material = nullptr;
		PxPvd* m_Pvd = nullptr;
		PxPvdSceneClient* m_PVDClient = nullptr;
		//PxTolerancesScale m_ToleranceScale;
		//PxCooking* m_Cooking = nullptr;

		iCollisionListener* m_CollisionListener;

		PhysicsWorld(const PhysicsWorld& other) { ; }
		PhysicsWorld& operator=(const PhysicsWorld& other) { return *this; }
	};
}