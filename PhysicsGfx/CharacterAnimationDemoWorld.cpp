#include "CharacterAnimationDemoWorld.h"
#include "Animation.h"

namespace gdp
{
	const char* ANIMATION1 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Idle.FBX";
	const char* ANIMATION2 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Walk.FBX";
	const char* ANIMATION3 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Run.FBX";
	const char* ANIMATION4 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Jump.FBX";
	const char* ANIMATION5 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Attack01.FBX";
	const char* ANIMATION6 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Attack02.FBX";
	const char* ANIMATION7 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Die.FBX";
	const char* ANIMATION8 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@Spellcast.FBX";
	const char* ANIMATION9 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@BlastedForward.FBX";
	const char* ANIMATION0 = "assets/models/characters/Adventurer Aland/Animations/Adventurer Aland@BlastedBackward.FBX";

	CharacterAnimationDemoWorld::CharacterAnimationDemoWorld()
	{

	}

	CharacterAnimationDemoWorld::~CharacterAnimationDemoWorld()
	{

	}

	void CharacterAnimationDemoWorld::Initialize()
	{
		LoadAssets();
		LoadAnimations();
		
		//GameObject* character = GDP_CreateGameObject();
		std::vector<std::string> animations;
		animations.push_back(ANIMATION1);
		animations.push_back(ANIMATION2);
		animations.push_back(ANIMATION3);
		animations.push_back(ANIMATION4);
		animations.push_back(ANIMATION5);
		animations.push_back(ANIMATION6);
		animations.push_back(ANIMATION7);
		animations.push_back(ANIMATION8);
		animations.push_back(ANIMATION9);
		animations.push_back(ANIMATION0);
		GameObject* character = GDP_CreateAnimatedCharacter(ANIMATION1, animations);
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

	void CharacterAnimationDemoWorld::Destroy()
	{
		if (m_Character != nullptr)
			delete m_Character;
	}

	void CharacterAnimationDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0.f, 0, 500.f));
		SetCameraFacingDirection(glm::vec3(0.f, 0.f, -1.f));
	}

	void CharacterAnimationDemoWorld::LoadAssets()
	{
		// Model
		//GDP_LoadFBXFile(m_CharacterModelId, m_AnimationName, "assets/models/neutral.fbx");
		GDP_LoadFBXFile(m_CharacterModelId, m_AnimationName, "assets/models/characters/Adventurer Aland/FBX/Adventurer Aland@Attack01.FBX");
		
		GDP_LoadModel(m_SphereModelId, "assets/models/uvsphere.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_WoodMaterialId, m_WoodTextureId, glm::vec3(1.0f));

		SetDebugModelInfo(m_SphereModelId, m_WoodMaterialId);
	}

	void CharacterAnimationDemoWorld::LoadAnimations()
	{
	}
}