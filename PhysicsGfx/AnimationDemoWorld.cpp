#include "AnimationDemoWorld.h"
#include "Animation.h"

namespace gdp
{
	AnimationDemoWorld::AnimationDemoWorld()
	{

	}

	AnimationDemoWorld::~AnimationDemoWorld()
	{

	}

	void AnimationDemoWorld::Initialize()
	{
		LoadAssets();
		LoadAnimations();

		m_PlayerBall = CreateBall(glm::vec3(0.f), 1.f);

		GameObject* childBall = GDP_CreateGameObject();
		childBall->Animation.AnimationTime = 0.f;
		childBall->Animation.IsLooping = true;
		childBall->Animation.IsPlaying = true;
		childBall->Animation.AnimationType = "SLERPAnimation";
		childBall->Animation.Speed = 1.f;
		childBall->Position = glm::vec3(0.f, 2.f, 0.f);
		childBall->Renderer.ShaderId = 1;
		childBall->Renderer.MeshId = m_SphereModelId;
		childBall->Renderer.MaterialId = m_BallMaterialId;
		childBall->HasParent = true;
		m_PlayerBall->Children.push_back(childBall);


		GameObject* child2Ball = GDP_CreateGameObject();
		child2Ball->Animation.AnimationTime = 0.f;
		child2Ball->Animation.IsLooping = true;
		child2Ball->Animation.IsPlaying = true;
		child2Ball->Animation.AnimationType = "SLERPAnimation";
		child2Ball->Animation.Speed = 1.f;
		child2Ball->Position = glm::vec3(0.f, 2.f, 0.f);
		child2Ball->Renderer.ShaderId = 1;
		child2Ball->Renderer.MeshId = m_SphereModelId;
		child2Ball->Renderer.MaterialId = m_BallMaterialId;
		child2Ball->HasParent = true;
		childBall->Children.push_back(child2Ball);


		GameObject* child3Ball = GDP_CreateGameObject();
		child3Ball->Position = glm::vec3(0.f, 2.f, 0.f);
		child3Ball->Renderer.ShaderId = 1;
		child3Ball->Renderer.MeshId = m_SphereModelId;
		child3Ball->Renderer.MaterialId = m_BallMaterialId;
		child3Ball->HasParent = true;
		child2Ball->Children.push_back(child3Ball);



		GameObject* secondBall = CreateBall(glm::vec3(5, 0, 0.f), 1.f);


		GameObject* bchildBall = GDP_CreateGameObject();
		bchildBall->Animation.AnimationTime = 0.f;
		bchildBall->Animation.IsLooping = true;
		bchildBall->Animation.IsPlaying = true;
		bchildBall->Animation.AnimationType = "MIXAnimation";
		bchildBall->Animation.Speed = 1.f;
		bchildBall->Position = glm::vec3(0.f, 2.f, 0.f);
		bchildBall->Renderer.ShaderId = 1;
		bchildBall->Renderer.MeshId = m_SphereModelId;
		bchildBall->Renderer.MaterialId = m_BallMaterialId;
		bchildBall->HasParent = true;
		secondBall->Children.push_back(bchildBall);


		GameObject* bchild2Ball = GDP_CreateGameObject();
		bchild2Ball->Animation.AnimationTime = 0.f;
		bchild2Ball->Animation.IsLooping = true;
		bchild2Ball->Animation.IsPlaying = true;
		bchild2Ball->Animation.AnimationType = "MIXAnimation";
		bchild2Ball->Animation.Speed = 1.f;
		bchild2Ball->Position = glm::vec3(0.f, 2.f, 0.f);
		bchild2Ball->Renderer.ShaderId = 1;
		bchild2Ball->Renderer.MeshId = m_SphereModelId;
		bchild2Ball->Renderer.MaterialId = m_BallMaterialId;
		bchild2Ball->HasParent = true;
		bchildBall->Children.push_back(bchild2Ball);


		GameObject* bchild3Ball = GDP_CreateGameObject();
		bchild3Ball->Position = glm::vec3(0.f, 2.f, 0.f);
		bchild3Ball->Renderer.ShaderId = 1;
		bchild3Ball->Renderer.MeshId = m_SphereModelId;
		bchild3Ball->Renderer.MaterialId = m_BallMaterialId;
		bchild3Ball->HasParent = true;
		bchild2Ball->Children.push_back(bchild3Ball);
	}

	void AnimationDemoWorld::Destroy()
	{
		if (m_PlayerBall != nullptr)
			delete m_PlayerBall;
	}

	void AnimationDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0.f, 5.f, 30.f));
		SetCameraFacingDirection(glm::vec3(0.f, 0.f, -1.f));

		if (GDP_IsKeyPressed('1'))
		{
			m_PlayerBall->Animation.AnimationType = "EaseInAnimation";
			m_PlayerBall->Animation.AnimationTime = 0.f;
		}
		if (GDP_IsKeyPressed('2'))
		{
			m_PlayerBall->Animation.AnimationType = "EaseOutAnimation";
			m_PlayerBall->Animation.AnimationTime = 0.f;
		}
		if (GDP_IsKeyPressed('3'))
		{
			m_PlayerBall->Animation.AnimationType = "EaseInOutAnimation";
			m_PlayerBall->Animation.AnimationTime = 0.f;
		}
		if (GDP_IsKeyPressed('4'))
		{
			m_PlayerBall->Animation.Speed -= 0.1f;
		}
		if (GDP_IsKeyPressed('5'))
		{
			m_PlayerBall->Animation.Speed += 0.1f;
		}
	}

	void AnimationDemoWorld::LoadAssets()
	{
		// Models
		GDP_LoadModel(m_SphereModelId, "assets/models/uvsphere.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_BallMaterialId, m_WoodTextureId, glm::vec3(1.0f));
	}

	void AnimationDemoWorld::LoadAnimations()
	{
		AnimationData slerpRotationAnimation;
		slerpRotationAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 2.f, 0.f), 0.f));
		slerpRotationAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f));
		slerpRotationAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1, 0, 0, 0), 0.0f, true));
		slerpRotationAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0, 0, 0, 1), 5.0f, true));
		slerpRotationAnimation.Duration = 5.0f;
		GDP_LoadAnimation("SLERPAnimation", slerpRotationAnimation);

		AnimationData mixRotationAnimation;
		mixRotationAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 2.f, 0.f), 0.f));
		mixRotationAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.f), 0.f));
		mixRotationAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1, 0, 0, 0), 0.0f, false));
		mixRotationAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0, 0, 0, 1), 5.0f, false));
		mixRotationAnimation.Duration = 5.0f;
		GDP_LoadAnimation("MIXAnimation", mixRotationAnimation);

		AnimationData easeInAnimation;
		easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 0, 0), 0.0f, EaseIn));
		easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5, 0, 0), 0.25f, EaseIn));
		easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5, 10, 0), 0.5f, EaseIn));
		easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 10, 0), 0.75f, EaseIn));
		easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 0, 0), 1.0f));
		easeInAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.0f), 0.0f));
		easeInAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1, 0, 0, 0), 0.0f, true));
		easeInAnimation.Duration = 5.0f;
		GDP_LoadAnimation("EaseInAnimation", easeInAnimation);

		AnimationData easeOutAnimation;
		easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 0, 0), 0.0f, EaseOut));
		easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5, 0, 0), 0.25f, EaseOut));
		easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5, 10, 0), 0.5f, EaseOut));
		easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 10, 0), 0.75f, EaseOut));
		easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 0, 0), 1.0f));
		easeOutAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.0f), 0.0f));
		easeOutAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1, 0, 0, 0), 0.0f, true));
		easeOutAnimation.Duration = 5.0f;
		GDP_LoadAnimation("EaseOutAnimation", easeOutAnimation);

		AnimationData easeInOutAnimation;
		easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 0, 0), 0.0f, EaseInOut));
		easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5, 0, 0), 0.25f, EaseInOut));
		easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5, 10, 0), 0.5f, EaseInOut));
		easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 10, 0), 0.75f, EaseInOut));
		easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5, 0, 0), 1.0f));
		easeInOutAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(1.0f), 0.0f));
		easeInOutAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1, 0, 0, 0), 0.0f, true));
		easeInOutAnimation.Duration = 5.0f;
		GDP_LoadAnimation("EaseInOutAnimation", easeInOutAnimation);
	}

	GameObject* AnimationDemoWorld::CreateBall(const glm::vec3& position, float size)
	{
		GameObject* ball = GDP_CreateGameObject();
		ball->Position = position;
		ball->Renderer.ShaderId = 1;
		ball->Renderer.MeshId = m_SphereModelId;
		ball->Renderer.MaterialId = m_BallMaterialId;
		return ball;
	}
}