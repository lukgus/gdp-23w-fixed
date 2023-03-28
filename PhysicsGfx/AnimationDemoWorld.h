#pragma once

#include "Engine.h"
#include "World.h"

namespace gdp
{

	class AnimationDemoWorld : public World
	{
	public:
		AnimationDemoWorld();
		~AnimationDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;

		virtual void Update(float dt) override;

	private:
		void LoadAssets();
		void LoadAnimations();
		GameObject* CreateBall(const glm::vec3& position, float size);

		unsigned int m_SphereModelId;
		unsigned int m_WoodTextureId;
		unsigned int m_BallMaterialId;

		GameObject* m_PlayerBall;
		std::vector<GameObject*> m_GameObjects;
	};
}