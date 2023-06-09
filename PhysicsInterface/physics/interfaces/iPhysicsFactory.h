#pragma once

#include "iPhysicsWorld.h"
#include "iRigidBody.h"
#include "iSoftBody.h"
#include "RigidBodyDesc.h"
#include "SoftBodyDesc.h"
#include "iShape.h"
#include "iVehicle.h"

namespace physics
{
	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {}

		virtual iPhysicsWorld* CreateWorld() = 0;
		virtual iRigidBody* CreateRigidBody(const RigidBodyDesc& desc, iShape* shape) = 0;
		virtual iSoftBody* CreateSoftBody(const SoftBodyDesc& desc) = 0;
		virtual iVehicle* CreateVehicle(iRigidBody* chassis) = 0;
		virtual iCharacterController* CreateCharacterController(
			iConvexShape* shape, float stepHeight, const Vector3& up) = 0;

	protected:
		iPhysicsFactory() {}

	private:
		iPhysicsFactory(const iPhysicsFactory&) {}
		iPhysicsFactory& operator=(const iPhysicsFactory&) {
			return *this;
		}
	};
}
