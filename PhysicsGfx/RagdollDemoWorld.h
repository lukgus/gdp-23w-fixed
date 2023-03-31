#pragma once

#include "Engine.h"
#include "World.h"
#include "Ragdoll.h"

namespace gdp
{
	class RagdollDemoWorld : public World
	{
	public:
		RagdollDemoWorld();
		~RagdollDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;

		virtual void Update(float dt) override;

	private:
		void LoadAssets();
		void CreateGround();

		GameObject* m_Character;
		unsigned int m_CharacterModelId;
		std::string m_AnimationName;

		unsigned int m_PlaneModelId;
		unsigned int m_WoodTextureId;
		unsigned int m_WoodMaterialId;

		Ragdoll* m_Ragdoll;
	};
}