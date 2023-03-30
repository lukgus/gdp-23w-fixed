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

		unsigned int m_PlaneModelId;
		unsigned int m_WoodTextureId;
		unsigned int m_GroundMaterialId;

		Ragdoll* m_Ragdoll;
	};
}