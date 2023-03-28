#include "PhysicsWorld.h"

#include <iostream>

#include <algorithm>
#include <math.h>

#include <pvd/PxPvd.h>
#include <extensions/PxSimpleFactory.h>

namespace physics
{
	PhysicsWorld::PhysicsWorld(void)
	{
		printf("PhysX PhysicsWorld!\n");


		// Foundation and Physics
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback,
			m_DefaultErrorCallback);
		if (!m_Foundation)
			throw("PxCreateFoundation failed!");

		bool recordMemoryAllocations = true;

		m_Pvd = PxCreatePvd(*m_Foundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation,
			PxTolerancesScale(), recordMemoryAllocations, m_Pvd);
		if (!m_Physics)
			throw("PxCreatePhysics failed!");

		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f);
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = m_Dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		m_Scene = m_Physics->createScene(sceneDesc);

		// PVD
		m_PVDClient = m_Scene->getScenePvdClient();
		if (m_PVDClient)
		{
			m_PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			m_PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			m_PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}


		//// Cooking
		//m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxCookingParams(PxTolerancesScale()));
		//if (!m_Cooking)
		//	throw("PxCreateCooking failed!");

		// Extensions
		if (!PxInitExtensions(*m_Physics, m_Pvd))
			throw("PxInitExtensions failed!");



		// Test here quickly adding an Actor
		m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
		physx::PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, physx::PxPlane(0, 1, 0, 0), *m_Material);
		m_Scene->addActor(*groundPlane);


		PxShape* sphereShape = m_Physics->createShape(PxSphereGeometry(1.0f), *m_Material);
		PxTransform transform(0, 10, 0);
		PxRigidDynamic* sphereBody = m_Physics->createRigidDynamic(transform);
		sphereBody->attachShape(*sphereShape);
		m_Scene->addActor(*sphereBody);

		for (int i = 0; i < 100; i += 3)
		{
			PxTransform transform(sin(i)*0.01f, 3 + i, cos(i) * 0.01f);
			PxRigidDynamic* sphereBody = m_Physics->createRigidDynamic(transform);
			sphereBody->attachShape(*sphereShape);
			m_Scene->addActor(*sphereBody);
		}



		//PxCreatePlane(*m_Physics, 

		//PxActor* actor;
	}

	PhysicsWorld::~PhysicsWorld(void)
	{
		m_Physics->release();
		m_Foundation->release();
	}

	void PhysicsWorld::TimeStep(float dt)
	{
		//if (m_Pvd->isConnected())
		//{
		//	printf("Connected!\n");
		//}
		//else
		//{
		//	printf("NOT Connected!\n");
		//}
		m_Scene->simulate(dt);
		m_Scene->fetchResults(true);
	}

	void PhysicsWorld::SetGravity(const Vector3& gravity)
	{

	}

	void PhysicsWorld::AddBody(iCollisionBody* body)
	{

	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{

	}
};
