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
		m_Ragdoll = GDP_CreateRagdoll(1.0f);
		const char* ANIMATION1 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Idle.FBX";

		std::vector<std::string> animations;
		animations.push_back(ANIMATION1);
		GameObject* character = GDP_CreateAnimatedCharacter(ANIMATION1, animations);
		character->Scale *= 0.025f;
		character->Animation.IsCharacterAnimation = true;
		character->Animation.AnimationTime = 0.f;
		character->Animation.IsLooping = true;
		character->Animation.IsPlaying = true;
		character->Animation.AnimationType = m_AnimationName;
		character->Animation.Speed = 1.f;
		character->Position = glm::vec3(0.f, 0.f, 0.f);
		character->Renderer.MaterialId = m_WoodMaterialId;
		character->HasBones = true;
	}

	void RagdollDemoWorld::Destroy()
	{
		if (m_Character != nullptr)
			delete m_Character;
	}

	void RagdollDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0, 8, 14));
		SetCameraFacingDirection(glm::vec3(0, -.2, -0.8));

		if (GDP_IsKeyPressed(' '))
		{
			m_Ragdoll->GetBodyPart(BodyPart_Head)->ApplyImpulse(Vector3(0, 40, 0));
		}

	}

	void RagdollDemoWorld::LoadAssets()
	{
		// Model
		//GDP_LoadFBXFile(m_CharacterModelId, m_AnimationName, "assets/models/neutral.fbx");
		GDP_LoadFBXFile(m_CharacterModelId, m_AnimationName, "assets/models/characters/Adventurer Aland/FBX/Adventurer Aland@Attack01.FBX");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Models
		GDP_LoadModel(m_PlaneModelId, "assets/models/plane.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_WoodMaterialId, m_WoodTextureId, glm::vec3(1.0f));
	}

	void RagdollDemoWorld::CreateGround()
	{
		physics::iShape* planeShape = new physics::PlaneShape(0.0f, glm::vec3(0.f, 1.f, 0.f));
		physics::RigidBodyDesc desc;
		desc.isStatic = true;
		desc.mass = 0;
		desc.position = glm::vec3(0, -1, 0);
		desc.linearVelocity = glm::vec3(0.f);
		GDP_GetPhysicsWorld()->AddBody(GDP_CreateRigidBody(desc, planeShape));
	}

}