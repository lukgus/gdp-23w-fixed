#pragma once

#include <physics\interfaces\ShapeType.h>
#include <physics\interfaces\iShape.h>
#include <physics\interfaces\iRigidBody.h>
#include <physics\interfaces\iSoftBody.h>
#include <physics\interfaces\iPhysicsWorld.h>
#include <physics\interfaces\iPhysicsFactory.h>
#include <physics\interfaces\SphereShape.h>
#include <physics\interfaces\PlaneShape.h>
#include <physics\interfaces\BoxShape.h>

#define Bullet_Physics

#if defined GDP_Physics
#pragma comment(lib, "GDPPhysics.lib")
#include <physics\gdp\PhysicsFactory.h>

#elif defined Bullet_Physics
#pragma comment(lib, "BulletSource.lib")
#pragma comment(lib, "BulletPhysics.lib")
#include <physics\bullet\PhysicsFactory.h>

#elif defined PhysX_Physics
//#define PX_PHYSX_STATIC_LIB
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysXCooking_64.lib")
#pragma comment(lib, "PVDRuntime_64.lib")
#pragma comment(lib, "SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXPhysics.lib")
#include <physics\physx\PhysicsFactory.h>
#endif

typedef physics::PhysicsFactory PhysicsFactoryType;