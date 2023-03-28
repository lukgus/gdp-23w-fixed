#ifndef PhysicsFactory_H
#define PhysicsFactory_H
// Interface
#include <physics/interfaces/iPhysicsFactory.h>
#include <physics/interfaces/iRigidBody.h>

namespace physics
{
	class PhysicsFactory : public iPhysicsFactory
	{
	public:
		PhysicsFactory(void);
		virtual ~PhysicsFactory(void);

		virtual iPhysicsWorld* CreateWorld(void);

		virtual iRigidBody* CreateRigidBody(const RigidBodyDesc& desc, iShape* shape);
		virtual iSoftBody* CreateSoftBody(const SoftBodyDesc& desc);
	};
};

#endif