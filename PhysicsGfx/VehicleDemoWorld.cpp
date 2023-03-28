#include "VehicleDemoWorld.h"

#include "GL.h"
#include "Model.h"
#include "vertex_types.h"
#include "physics.h"

#include <physics/interfaces/SphereShape.h>
#include <physics/interfaces/PlaneShape.h>
#include <physics/interfaces/CapsuleShape.h>
#include <physics/interfaces/Point2PointConstraint.h>

namespace gdp
{
	VehicleDemoWorld::VehicleDemoWorld()
	{

	}

	VehicleDemoWorld::~VehicleDemoWorld()
	{

	}

	void VehicleDemoWorld::Initialize()
	{
		GDP_GetPhysicsWorld()->SetGravity(glm::vec3(0, -9.81, 0));

		LoadAssets();
		CreateGround();

		// Create a CharacterController
		float radius = 1.f;
		float height = 3.f;
		physics::iConvexShape* capsule = new physics::CapsuleShape(radius, height);
		float stepHeight = 0.25f;
		glm::vec3 up(0, 1, 0);
		m_CharacterController = GDP_CreateCharacterController(capsule, stepHeight, up);
		GDP_GetPhysicsWorld()->AddCharacterController(m_CharacterController);

		// Create some boxes

		glm::vec3 boxHalfExtents(0.5f, 0.5f, 0.5f);
		physics::BoxShape* staticBoxShape = new physics::BoxShape(boxHalfExtents);

		physics::RigidBodyDesc boxBodyADesc;
		boxBodyADesc.isStatic = true;
		boxBodyADesc.mass = 1;
		boxBodyADesc.position = glm::vec3(-3.f, 0.5f, 5.f);
		boxBodyADesc.linearVelocity = glm::vec3(0.f);
		physics::iRigidBody* boxABody = GDP_CreateRigidBody(boxBodyADesc, staticBoxShape);
		GDP_GetPhysicsWorld()->AddBody(boxABody);

		GameObject* boxAGO = GDP_CreateGameObject();
		boxAGO->Position = glm::vec3(-3.f, 0.125f, 5.f);
		boxAGO->Scale = glm::vec3(1.f, 0.25f, 1.f);
		boxAGO->Rotation = glm::quat(1, 0, 0, 0);
		boxAGO->Renderer.ShaderId = 1;
		boxAGO->Renderer.MeshId = m_ChassisModelId;
		boxAGO->Renderer.MaterialId = m_ChassisMaterialId;
		boxAGO->useModelMatrix = true;

		physics::RigidBodyDesc boxBodyBDesc;
		boxBodyBDesc.isStatic = true;
		boxBodyBDesc.mass = 1;
		boxBodyBDesc.position = glm::vec3(-5.f, 0.5f, 5.f);
		boxBodyBDesc.linearVelocity = glm::vec3(0.f);
		physics::iRigidBody* boxBBody = GDP_CreateRigidBody(boxBodyBDesc, staticBoxShape);
		GDP_GetPhysicsWorld()->AddBody(boxBBody);

		GameObject* boxBGO = GDP_CreateGameObject();
		boxBGO->Position = glm::vec3(-5.f, 0.125f, 5.f);
		boxBGO->Scale = glm::vec3(1.f, 0.25f, 1.f);
		boxBGO->Rotation = glm::quat(1, 0, 0, 0);
		boxBGO->Renderer.ShaderId = 1;
		boxBGO->Renderer.MeshId = m_ChassisModelId;
		boxBGO->Renderer.MaterialId = m_ChassisMaterialId;
		boxBGO->useModelMatrix = true;

		// TODO: Add Visual object
		// ..

		// Create vehicle
		// 1. Create a Chassis
		physics::Vector3 halfExtents(1, 1, 2);
		physics::iShape* boxShape = new physics::BoxShape(halfExtents);
		physics::RigidBodyDesc desc;
		desc.isStatic = false;
		desc.mass = 20;
		desc.position = glm::vec3(0.f, 10.f, 0.f);
		desc.linearVelocity = glm::vec3(0.f);
		physics::iRigidBody* chassis = GDP_CreateRigidBody(desc, boxShape);

		// Add our chassis rigid body to the physics world
		GDP_GetPhysicsWorld()->AddBody(chassis);

		// Create Vehicle
		m_Vehicle = GDP_CreateVehicle(chassis);
		
		// Add Vehicle to Physics World
		GDP_GetPhysicsWorld()->AddVehicle(m_Vehicle);

		// Add wheels

		// We want z forward and y up
		physics::Vector3 wheelAxle(-1.f, 0.f, 0.f);
		physics::Vector3 wheelDir(0.f, -1.f, 0.f);		// Raycast & Spring direction
		float wheelRadius = 1.5f;
		float suspension = 0.6f;

		m_Vehicle->AddWheel(
			physics::Vector3(1.25f, 0.25f, 2.f),
			wheelDir,
			wheelAxle,
			suspension,
			wheelRadius,
			true);
		m_Vehicle->AddWheel(
			physics::Vector3(-1.25f, 0.25f, 2.f),
			wheelDir,
			wheelAxle,
			suspension,
			wheelRadius,
			true);
		m_Vehicle->AddWheel(
			physics::Vector3(1.25f, 0.25f, -2.f),
			wheelDir,
			wheelAxle,
			suspension,
			wheelRadius,
			false);
		m_Vehicle->AddWheel(
			physics::Vector3(-1.25f, 0.25f, -2.f),
			wheelDir,
			wheelAxle,
			suspension,
			wheelRadius,
			false);

		// Create our Vehicle graphics
		m_VehicleGameObject = GDP_CreateGameObject();
		m_VehicleGameObject->Renderer.ShaderId = 1;
		m_VehicleGameObject->Renderer.MeshId = m_ChassisModelId;
		m_VehicleGameObject->Renderer.MaterialId = m_ChassisMaterialId;
		m_VehicleGameObject->useModelMatrix = true;

		// Create our Vehicle graphics
		GameObject* wheel;
		wheel = GDP_CreateGameObject();
		wheel->Renderer.ShaderId = 1;
		wheel->Renderer.MeshId = m_WheelModelId;
		wheel->Renderer.MaterialId = m_WheelMaterialId;
		wheel->useModelMatrix = true;
		m_Wheels.push_back(wheel);

		wheel = GDP_CreateGameObject();
		wheel->Renderer.ShaderId = 1;
		wheel->Renderer.MeshId = m_WheelModelId;
		wheel->Renderer.MaterialId = m_WheelMaterialId;
		wheel->useModelMatrix = true;
		m_Wheels.push_back(wheel);
		
		wheel = GDP_CreateGameObject();
		wheel->Renderer.ShaderId = 1;
		wheel->Renderer.MeshId = m_WheelModelId;
		wheel->Renderer.MaterialId = m_WheelMaterialId;
		wheel->useModelMatrix = true;
		m_Wheels.push_back(wheel);
		
		wheel = GDP_CreateGameObject();
		wheel->Renderer.ShaderId = 1;
		wheel->Renderer.MeshId = m_WheelModelId;
		wheel->Renderer.MaterialId = m_WheelMaterialId;
		wheel->useModelMatrix = true;
		m_Wheels.push_back(wheel);

		// Add a constraint
		physics::iConstraint* bodyJoint = new physics::Point2PointConstraint(
			boxABody, boxBBody, physics::Vector3(2, 0, 0), physics::Vector3(-2, 0, 0));

		physics::iConstraint* chassisJoint = new physics::Point2PointConstraint(
			chassis, boxABody, physics::Vector3(3, 0, 0), physics::Vector3(-2, 0, 0));

		GDP_GetPhysicsWorld()->AddConstraint(bodyJoint);
		GDP_GetPhysicsWorld()->AddConstraint(chassisJoint);
	}

	void VehicleDemoWorld::Destroy()
	{

	}

	void VehicleDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0, 12, 30));
		SetCameraFacingDirection(glm::vec3(0, -.2, -0.8));


		float force = 5.f;
		float steering = 1.f;
		glm::vec3 direction(0.f);

		if (GDP_IsKeyHeldDown('w'))
		{
			//m_WalkDirection.z -= 1.f * dt;

			m_EngineForce += force * dt;
			m_Vehicle->ApplyEngineForce(m_EngineForce, 0);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 1);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 2);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 3);
		}
		else if (GDP_IsKeyHeldDown('s'))
		{
			//m_WalkDirection.z += 1.f * dt;
			m_EngineForce -= force * dt;
			m_Vehicle->ApplyEngineForce(m_EngineForce, 0);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 1);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 2);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 3);
		}
		else
		{
			//m_WalkDirection.z = 0.f;
			m_EngineForce = 0;
			m_Vehicle->ApplyEngineForce(m_EngineForce, 0);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 1);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 2);
			m_Vehicle->ApplyEngineForce(m_EngineForce, 3);
		}
		if (GDP_IsKeyHeldDown('a'))
		{
			//m_WalkDirection.x -= 1.f * dt;
			m_Steering -= steering * dt;
			m_Vehicle->SetSteering(m_Steering, 0);
			m_Vehicle->SetSteering(m_Steering, 1);
		}
		else if (GDP_IsKeyHeldDown('d'))
		{
			//m_WalkDirection.x += 1.f * dt;
			m_Steering += steering * dt;
			m_Vehicle->SetSteering(m_Steering, 0);
			m_Vehicle->SetSteering(m_Steering, 1);
		}
		else
		{
			m_WalkDirection.x = 0.f;
		}

		m_CharacterController->SetWalkDirection(m_WalkDirection);

		if (GDP_IsKeyPressed(' '))
		{
			//m_CharacterController->Jump();
			m_BrakeValue += force * dt;
			m_Vehicle->SetBrake(m_BrakeValue, 0);
			m_Vehicle->SetBrake(m_BrakeValue, 1);
			m_Vehicle->SetBrake(m_BrakeValue, 2);
			m_Vehicle->SetBrake(m_BrakeValue, 3);
		}
		if (GDP_IsKeyReleased(' '))
		{
			m_BrakeValue = 0.f;
			m_Vehicle->SetBrake(m_BrakeValue, 0);
			m_Vehicle->SetBrake(m_BrakeValue, 1);
			m_Vehicle->SetBrake(m_BrakeValue, 2);
			m_Vehicle->SetBrake(m_BrakeValue, 3);
		}

		glm::mat4 modelMatrix;
		// Update chassis position
		m_Vehicle->GetChassisTransform(m_VehicleGameObject->modelMatrix);

		// Update wheel positions
		for (int i = 0; i < 4; i++)
		{
			m_Vehicle->GetWheelTransform(m_Wheels[i]->modelMatrix, i);
		}

	}

	void VehicleDemoWorld::LoadAssets()
	{
		// Models
		GDP_LoadModel(m_PlaneModelId, "assets/models/plane.fbx");
		GDP_LoadModel(m_ChassisModelId, "assets/models/Chassis.fbx");
		GDP_LoadModel(m_WheelModelId, "assets/models/Wheel.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_GroundMaterialId, m_WoodTextureId, glm::vec3(1.0f));
		GDP_CreateMaterial(m_ChassisMaterialId, m_WoodTextureId, glm::vec3(1.0f, 0.f, 0.f));
		GDP_CreateMaterial(m_WheelMaterialId, m_WoodTextureId, glm::vec3(0.0f));
	}

	void VehicleDemoWorld::CreateGround()
	{
		GameObject* ground = GDP_CreateGameObject();
		ground->Position = glm::vec3(0, 0, 0);
		ground->Scale = glm::vec3(16, 16, 1);
		ground->Rotation = glm::angleAxis(glm::radians(-90.f), glm::vec3(1, 0, 0));
		ground->Renderer.ShaderId = 1;
		ground->Renderer.MeshId = m_PlaneModelId;
		ground->Renderer.MaterialId = m_GroundMaterialId;

		physics::iShape* planeShape = new physics::PlaneShape(0.0f, glm::vec3(0.f, 1.f, 0.f));
		physics::RigidBodyDesc desc;
		desc.isStatic = true;
		desc.mass = 0;
		desc.position = glm::vec3(0.f);
		desc.linearVelocity = glm::vec3(0.f);
		ground->PhysicsBody = GDP_CreateRigidBody(desc, planeShape);
		GDP_GetPhysicsWorld()->AddBody(ground->PhysicsBody);
	}

}