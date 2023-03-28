#pragma once

#include "World.h"
#include "Engine.h"

namespace gdp
{
	class AIDemoWorld : public World
	{
	public:
		AIDemoWorld();
		~AIDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Update(float dt) override;

	private:
		void LoadAssets();
		GameObject* CreateAgent(const glm::vec3& position);
		GameObject* CreateObstacle(const glm::vec3& position, const glm::vec3& size);

		unsigned int m_SphereModelId;
		unsigned int m_WoodTextureId;
		unsigned int m_BallMaterialId;
		unsigned int m_SeekMaterialId;
		unsigned int m_FleeMaterialId;
		unsigned int m_ObstacleMaterialId;

		GameObject* m_Player;
		GameObject* m_NearDebugSphere;
		GameObject* m_FarDebugSphere;
		GameObject* m_ClosestDebugSphere;
		GameObject* m_TotalSteeringDebugSphere;
		GameObject* m_SeekSteeringDebugSphere;
		GameObject* m_AvoidanceSteeringDebugSphere;
		glm::vec3 m_playerFaceDirection;
		std::vector<GameObject*> m_Enemies;
		std::vector<GameObject*> m_Obstacles;
	};
}