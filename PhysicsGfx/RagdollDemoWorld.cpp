#include "RagdollDemoWorld.h"

namespace gdp
{
	RagdollDemoWorld::RagdollDemoWorld()
	{

	}

	RagdollDemoWorld::~RagdollDemoWorld()
	{

	}

	void RagdollDemoWorld::Initialize()
	{
		LoadAssets();
		CreateGround();
		m_Ragdoll = new Ragdoll(1.f, 1.f);
	}

	void RagdollDemoWorld::Destroy()
	{

	}

	void RagdollDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0, 8, 14));
		SetCameraFacingDirection(glm::vec3(0, -.2, -0.8));

	}

	void RagdollDemoWorld::LoadAssets()
	{
		// Models
		GDP_LoadModel(m_PlaneModelId, "assets/models/plane.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_GroundMaterialId, m_WoodTextureId, glm::vec3(1.0f));
	}

	void RagdollDemoWorld::CreateGround()
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
		desc.position = glm::vec3(0, -1, 0);
		desc.linearVelocity = glm::vec3(0.f);
		ground->PhysicsBody = GDP_CreateRigidBody(desc, planeShape);
		GDP_GetPhysicsWorld()->AddBody(ground->PhysicsBody);
	}

}