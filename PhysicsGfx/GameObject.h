#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "MeshRenderer.h"
#include "Animation.h"
#include "BoneHierarchy.h"
#include <physics/interfaces/iCollisionBody.h>

namespace gdp
{
	struct GameObjectBoneData
	{
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::quat Rotation;
		glm::mat4 ModelMatrix;
	};

	class GameObject {
	public:
		GameObject()
			: Position(0.0f)
			, Velocity(0.0f)
			, Steering(0.0f)
			, Scale(1.0f)
			, Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
			, Renderer()
			//, PhysicsBody(nullptr)
			, Enabled(true)
			, HasParent(false)
			, useModelMatrix(false)
		{ }

		bool IsBonedObject;
		bool useModelMatrix;
		glm::mat4 modelMatrix;

		// Transformations
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::quat Rotation;

		// Artificial Intelligence
		glm::vec3 Velocity;
		glm::vec3 Steering;

		MeshRenderer Renderer;
		bool Enabled;

		// Physics & Simulation
		physics::iCollisionBody* PhysicsBody;

		// Animation
		bool HasParent;
		bool HasBones;

		// Character Animation
		std::vector<glm::mat4> BoneModelMatrices;
		std::vector<glm::mat4> GlobalTransformations;
		glm::mat4 BoneRotationMatrices[66];

		Animation Animation;
		std::vector<GameObject*> Children;
	};
}