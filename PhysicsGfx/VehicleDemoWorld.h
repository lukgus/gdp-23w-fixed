#pragma once

#include "Engine.h"
#include "World.h"
#include <physics/interfaces/iVehicle.h>

namespace gdp
{
	class VehicleDemoWorld : public World
	{
	public:
		VehicleDemoWorld();
		~VehicleDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;

		virtual void Update(float dt) override;

	private:
		void LoadAssets();
		void CreateGround();

		unsigned int m_PlaneModelId;
		unsigned int m_ChassisModelId;
		unsigned int m_WheelModelId;

		unsigned int m_WoodTextureId;

		unsigned int m_GroundMaterialId;
		unsigned int m_ChassisMaterialId;
		unsigned int m_WheelMaterialId;

		float m_EngineForce;
		float m_Steering;
		float m_BrakeValue;

		glm::vec3 m_WalkDirection;

		physics::iVehicle* m_Vehicle;
		physics::iCharacterController* m_CharacterController;
		GameObject* m_VehicleGameObject;
		std::vector<GameObject*> m_Wheels;
	};
}