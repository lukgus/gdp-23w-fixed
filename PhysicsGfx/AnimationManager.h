#pragma once

#include <string>
#include <map>

#include "Animation.h"
#include "GameObject.h"


namespace gdp
{
	class AnimationManager
	{
	public:
		AnimationManager();
		~AnimationManager();

		bool LoadAnimation(const std::string& name, AnimationData animation);
		bool LoadCharacterAnimation(const std::string& name, CharacterAnimationData animation);
		void Update(const std::vector<GameObject*>& gameObjects, float dt);

	private:
		void UpdateBoneHierarchy(BoneNode* node, CharacterAnimationData& data, const glm::mat4& parentTransformationMatrix, float keyFrameTime);
		void SetGameObjectBoneModelMatrices(GameObject* gameObject, BoneNode* node, const CharacterAnimationData& data);
		AnimationData* FindAnimationData(const std::string& nodeName, const CharacterAnimationData& data);

		int FindPositionKeyFrameIndex(const AnimationData& animation, float keyFrameTime);
		int FindScaleKeyFrameIndex(const AnimationData& animation, float keyFrameTime);
		int FindRotationKeyFrameIndex(const AnimationData& animation, float keyFrameTime);

		glm::vec3 GetAnimationPosition(const AnimationData& animation, float keyFrameTime);
		glm::vec3 GetAnimationScale(const AnimationData& animation, float keyFrameTime);
		glm::quat GetAnimationRotation(const AnimationData& animation, float keyFrameTime);

		// BallBounceAnimation will be stored here
		std::map<std::string, AnimationData> m_Animations;
		std::map<std::string, CharacterAnimationData> m_CharacterAnimations;
		glm::mat4 m_GlobalInverseTransform;
	};
}
