#pragma once

#include "Engine.h"
#include "World.h"

namespace gdp
{

	class CharacterAnimationDemoWorld : public World
	{
	public:
		CharacterAnimationDemoWorld();
		~CharacterAnimationDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;

		virtual void Update(float dt) override;

	private:
		void LoadAssets();
		void LoadAnimations();

		unsigned int m_SphereModelId;
		unsigned int m_CharacterModelId;
		std::string m_AnimationName;

		unsigned int m_WoodTextureId;
		unsigned int m_WoodMaterialId;

		GameObject* m_Character;
	};
}