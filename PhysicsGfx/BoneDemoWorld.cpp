#include "BoneDemoWorld.h"
#include "Animation.h"

#include <map>
#include <vector>

namespace gdp
{
	glm::mat4 CreateModelMatrix(const glm::mat4& parentModelMatrix, const glm::vec3& translate, const glm::quat& rotate)
	{
		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.f), translate);
		glm::mat4 RotationMatrix = glm::mat4_cast(rotate);
		return parentModelMatrix * TranslationMatrix * RotationMatrix;
	}

	BoneDemoWorld::BoneDemoWorld()
	{

	}

	BoneDemoWorld::~BoneDemoWorld()
	{

	}

	void BoneDemoWorld::Initialize()
	{
		LoadAssets();

		m_BoneObject = GDP_CreateGameObject();
		m_BoneObject->Position = glm::vec3(0.f);
		m_BoneObject->HasBones = true;
		m_BoneObject->Renderer.MeshId = m_ConeModelId;
		m_BoneObject->Renderer.MaterialId = m_WoodMaterialId;
		m_BoneObject->BoneModelMatrices[0] = glm::mat4(1.f);	// identity matrix


		m_Bone1Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		m_Bone2Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		m_Bone3Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	void BoneDemoWorld::Destroy()
	{
	}

	void KeyPress(char key, bool state)
	{

	}

	void BoneDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0.f, 200.f, 500.f));
		SetCameraFacingDirection(glm::vec3(0.f, 0.f, -1.f));

		const float rotateSpeed = 5.f;
		const float moveSpeed = 10.f;

		if (GDP_IsKeyHeldDown('w'))
		{
			m_Bone1Position.y += dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('s'))
		{
			m_Bone1Position.y -= dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('a'))
		{
			m_Bone1Position.x -= dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('d'))
		{
			m_Bone1Position.x += dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('q'))
		{
			m_Bone1Rotation = glm::rotate(m_Bone1Rotation, dt * rotateSpeed, glm::vec3(0, 0, 1));
		}
		if (GDP_IsKeyHeldDown('e'))
		{
			m_Bone1Rotation = glm::rotate(m_Bone1Rotation, dt * -rotateSpeed, glm::vec3(0, 0, 1));
		}

		if (GDP_IsKeyHeldDown('t'))
		{
			m_Bone2Position.y += dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('g'))
		{
			m_Bone2Position.y -= dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('f'))
		{
			m_Bone2Position.x -= dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('h'))
		{
			m_Bone2Position.x += dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('r'))
		{
			m_Bone2Rotation = glm::rotate(m_Bone2Rotation, dt * rotateSpeed, glm::vec3(0, 0, 1));
		}
		if (GDP_IsKeyHeldDown('y'))
		{
			m_Bone2Rotation = glm::rotate(m_Bone2Rotation, dt * -rotateSpeed, glm::vec3(0, 0, 1));
		}

		if (GDP_IsKeyHeldDown('i'))
		{
			m_Bone3Position.y += dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('k'))
		{
			m_Bone3Position.y -= dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('j'))
		{
			m_Bone3Position.x -= dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('l'))
		{
			m_Bone3Position.x += dt * moveSpeed;
		}
		if (GDP_IsKeyHeldDown('u'))
		{
			m_Bone3Rotation = glm::rotate(m_Bone3Rotation, dt * rotateSpeed, glm::vec3(0, 0, 1));
		}
		if (GDP_IsKeyHeldDown('o'))
		{
			m_Bone3Rotation = glm::rotate(m_Bone3Rotation, dt * -rotateSpeed, glm::vec3(0, 0, 1));
		}

		if (GDP_IsKeyHeldDown('z'))
		{
			// Rotate the current rotation around a vector
			// We want to rotate around the Y axis.
			m_BoneObject->Rotation = glm::rotate(m_BoneObject->Rotation, dt * rotateSpeed, glm::vec3(0, 1, 0));
		}
		if (GDP_IsKeyHeldDown('x'))
		{
			m_BoneObject->Rotation = glm::rotate(m_BoneObject->Rotation, dt * -rotateSpeed, glm::vec3(0, 1, 0));
		}

		// Model Matrix = Rotation, translation & scale
		//m_BoneObject->BoneModelMatrices[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f));
		//m_BoneObject->BoneModelMatrices[1] = glm::translate(m_BoneObject->BoneModelMatrices[0], m_Bone1Position);
		//m_BoneObject->BoneModelMatrices[2] = glm::translate(m_BoneObject->BoneModelMatrices[1], m_Bone2Position);
		//m_BoneObject->BoneModelMatrices[3] = glm::translate(m_BoneObject->BoneModelMatrices[2], m_Bone3Position);

		m_BoneObject->BoneModelMatrices[0] = CreateModelMatrix(glm::mat4(1.f), m_Bone1Position, m_Bone1Rotation);
		m_BoneObject->BoneModelMatrices[1] = CreateModelMatrix(m_BoneObject->BoneModelMatrices[0], m_Bone2Position, m_Bone2Rotation);
		m_BoneObject->BoneModelMatrices[2] = CreateModelMatrix(m_BoneObject->BoneModelMatrices[1], m_Bone3Position, m_Bone3Rotation);
		m_BoneObject->BoneModelMatrices[3] = m_BoneObject->BoneModelMatrices[2];

		m_BoneObject->BoneRotationMatrices[0] = glm::mat4(1.f) * glm::mat4_cast(m_Bone1Rotation);
		m_BoneObject->BoneRotationMatrices[1] = m_BoneObject->BoneRotationMatrices[0] * glm::mat4_cast(m_Bone2Rotation);
		m_BoneObject->BoneRotationMatrices[2] = m_BoneObject->BoneRotationMatrices[1] * glm::mat4_cast(m_Bone3Rotation);
		m_BoneObject->BoneRotationMatrices[3] = m_BoneObject->BoneRotationMatrices[2];	// Identity matrix; No rotation
		// Rotation matrix only includes the rotation

		//glm::mat4 RotationMatrix = glm::mat4_cast(go->Rotation);
		//glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), go->Scale);
		//glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
	}

	void BoneDemoWorld::LoadAssets()
	{
		GDP_LoadModel(m_ConeModelId, "assets/models/Neutrala.fbx", true);

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_WoodMaterialId, m_WoodTextureId, glm::vec3(1.0f));
	}

	void BoneDemoWorld::CreateAnimations()
	{
		AnimationData slerpRotationAnimation;
		slerpRotationAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 2.f, 0.f), 0.f));
		slerpRotationAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f));
		slerpRotationAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1, 0, 0, 0), 0.0f, true));
		slerpRotationAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0, 0, 0, 1), 5.0f, true));
		slerpRotationAnimation.Duration = 5.0f;
		GDP_LoadAnimation("SLERPAnimation", slerpRotationAnimation);
	}
}