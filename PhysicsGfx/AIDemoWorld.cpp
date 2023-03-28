#include "AIDemoWorld.h"

//#define DISPLAY_DEBUG_SPHERES 

namespace gdp
{
	// Christer Ericson's Realtime Collision Detection
	// Page 128-129
	void ClosestPtPointSegment(glm::vec3 c, glm::vec3 a, glm::vec3 b, float& t, glm::vec3& d)
	{
		glm::vec3 ab = b - a;
		// Project c onto ab, computing parameterized position d(t) = a * t*(b - a)
		t = glm::dot(c - a, ab) / glm::dot(ab, ab);

		// If outside segment clamp t (and therfore) d to the closest endpoint
		if (t < 0.f) t = 0.f;
		if (t > 1.f) t = 1.f;

		// Compute projected position from the clamped t
		d = a + t * ab;
	}

	AIDemoWorld::AIDemoWorld()
	{

	}

	AIDemoWorld::~AIDemoWorld()
	{

	}

	void AIDemoWorld::Initialize()
	{
		LoadAssets();

		GameObject* agent;
		agent = CreateAgent(glm::vec3( 1, 5, 0));
		m_Player = agent;
		m_playerFaceDirection = glm::vec3(0.f, 1.f, 0.f);

		agent = CreateAgent(glm::vec3(1, -10, 0));
		m_Enemies.push_back(agent);

		agent = CreateAgent(glm::vec3(-1, -7, 0));
		m_Enemies.push_back(agent);

		agent = CreateAgent(glm::vec3(-4, 4, 0));
		m_Enemies.push_back(agent);

		agent = CreateAgent(glm::vec3(4, 4, 0));
		m_Enemies.push_back(agent);

#ifdef DISPLAY_DEBUG_SPHERES
		m_NearDebugSphere = CreateAgent(glm::vec3(0.f));
		m_NearDebugSphere->Scale *= 0.25f;

		m_FarDebugSphere = CreateAgent(glm::vec3(0.f));
		m_FarDebugSphere->Scale *= 0.25f;

		m_SeekSteeringDebugSphere = CreateAgent(glm::vec3(0.f));
		m_SeekSteeringDebugSphere->Scale *= 0.25f;
		m_SeekSteeringDebugSphere->Renderer.MaterialId = m_SeekMaterialId;

		m_AvoidanceSteeringDebugSphere = CreateAgent(glm::vec3(0.f));
		m_AvoidanceSteeringDebugSphere->Scale *= 0.25f;
		m_AvoidanceSteeringDebugSphere->Renderer.MaterialId = m_FleeMaterialId;

		m_TotalSteeringDebugSphere = CreateAgent(glm::vec3(0.f));
		m_TotalSteeringDebugSphere->Scale *= 0.25f;
		m_TotalSteeringDebugSphere->Renderer.MaterialId = m_ObstacleMaterialId;

		m_ClosestDebugSphere = CreateAgent(glm::vec3(0.f));
		m_ClosestDebugSphere->Scale *= 0.1f;
		m_ClosestDebugSphere->Renderer.MaterialId = m_ObstacleMaterialId;
#endif
		//agent = CreateAgent(glm::vec3( 5, 0, 0));
		//m_Enemies.push_back(agent);

		//agent = CreateAgent(glm::vec3( 0, 5, 0));
		//m_Enemies.push_back(agent);

		//agent = CreateAgent(glm::vec3( 0, -5, 0));
		//m_Enemies.push_back(agent);

		GameObject* obstacle;
		obstacle = CreateObstacle(glm::vec3(0.f), glm::vec3(2.f));
		m_Obstacles.push_back(obstacle);

		obstacle = CreateObstacle(glm::vec3(5.f, 0.f, 0.f), glm::vec3(1.f));
		m_Obstacles.push_back(obstacle);

		obstacle = CreateObstacle(glm::vec3(0.f, 5.f, 0.f), glm::vec3(1.f));
		m_Obstacles.push_back(obstacle);
	}

	void AIDemoWorld::Destroy()
	{

	}

	void AIDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0.f, 0.f, 20.f));
		SetCameraFacingDirection(glm::vec3(0.f, 0.f, -1.f));

		glm::vec3 playerVelocity(0.f);
		if (GDP_IsKeyHeldDown('w'))
		{
			playerVelocity.y += 1.f;
		}
		if (GDP_IsKeyHeldDown('a'))
		{
			playerVelocity.x -= 1.f;
		}
		if (GDP_IsKeyHeldDown('s'))
		{
			playerVelocity.y -= 1.f;
		}
		if (GDP_IsKeyHeldDown('d'))
		{
			playerVelocity.x += 1.f;
		}


		if (glm::length(playerVelocity) != 0.f)
		{
			m_playerFaceDirection = playerVelocity;
			playerVelocity = glm::normalize(playerVelocity);
			m_Player->Velocity = playerVelocity;
			m_Player->Position += m_Player->Velocity * dt * 3.0f;
		}


		// Quick implementation
		// TODO: Move to it's own class
		// Seek behaviour


		const float AheadDistance = 4.f;
		for (int i = 0; i < m_Enemies.size(); i++)
		{
			// Seek or Flee the player
			GameObject* target = m_Player;
			GameObject* agent = m_Enemies[i];

			glm::vec3 targetPosition = target->Position;

			// Calculate a look ahead value for Pursue & Evade behaviours
			if (glm::length(target->Velocity) != 0.f)
			{
				//glm::vec3 lookAhead = glm::normalize(target->Velocity) * AheadDistance;
				//targetPosition += lookAhead;
			}

			glm::vec3 towardsAgent = glm::normalize(agent->Position - targetPosition);
			glm::vec3 towardsTarget = -towardsAgent;

			float dot = glm::dot(towardsAgent, m_playerFaceDirection);
			///if (dot <= 0.f)
			//{
				// Seek or Pursue
				agent->Steering += towardsTarget;
#ifdef DISPLAY_DEBUG_SPHERES
				m_SeekSteeringDebugSphere->Position = agent->Position + agent->Steering;
#endif
				agent->Renderer.MaterialId = m_SeekMaterialId;
			//}
			//else
			//{
			//	// Flee or Evade
			//	agent->Steering = -towardsTarget;
			//	agent->Renderer.MaterialId = m_FleeMaterialId;
			//}
		}

		// Obstacle Avoidance
		const float nearDistance = 0.f;
		const float farDistance = 3.f;

		for (int agentIdx = 0; agentIdx < m_Enemies.size(); agentIdx++)
		{
			GameObject* agent = m_Enemies[agentIdx];

			if (glm::length(agent->Velocity) == 0.f)
				continue;

			glm::vec3 velNormal = glm::normalize(agent->Velocity);
			//glm::vec3 steerNormal = glm::normalize(agent->Steering);
			glm::vec3 nearPoint = agent->Position + velNormal * nearDistance;
			glm::vec3 farPoint = agent->Position + velNormal * farDistance;

#ifdef DISPLAY_DEBUG_SPHERES
			m_NearDebugSphere->Position = nearPoint;
			m_FarDebugSphere->Position = farPoint;
#endif
			for (int obstacleIdx = 0; obstacleIdx < m_Obstacles.size(); obstacleIdx++)
			{
				GameObject* obstacle = m_Obstacles[obstacleIdx];

				float t;
				glm::vec3 closestPoint;
				ClosestPtPointSegment(obstacle->Position, farPoint, nearPoint, t, closestPoint);
#ifdef DISPLAY_DEBUG_SPHERES
				m_ClosestDebugSphere->Position = closestPoint;
				m_ClosestDebugSphere->Position.z = 2.5f;
#endif
				float sumOfRadii = obstacle->Scale.x + agent->Scale.x;

				// CHeck if we are colliding with the obstacle
				if (glm::distance(obstacle->Position, agent->Position) < sumOfRadii)
				{
					glm::vec3 moveBackPosition = glm::normalize(agent->Position - obstacle->Position) * sumOfRadii;
					agent->Position = obstacle->Position + moveBackPosition;
				}

				// Check if we are close enough to the obstacle to start
				// avoiding the obstacle
				if (glm::distance(closestPoint, obstacle->Position) > sumOfRadii)
					continue;

				//float distance = glm::distance(farPoint, obstacle->Position);
				//if (distance > obstacle->Scale.x)
				//	continue;

				// Assuming we are only using 1 relative point.
				// (We will use both points after)
				glm::vec3 steerDirection = glm::normalize(farPoint - obstacle->Position);

				//agent->Steering *= (1 - t);
				agent->Steering += steerDirection;
#ifdef DISPLAY_DEBUG_SPHERES
				m_SeekSteeringDebugSphere->Position = agent->Position + steerDirection;
				m_TotalSteeringDebugSphere->Position = agent->Position + agent->Steering;
#endif
				//agent->Steering *= t;
				//agent->Steering = glm::normalize(steerDirection);
			}
		}

		const float max_speed = 1.f;
		for (int i = 0; i < m_Enemies.size(); i++)
		{
			GameObject* agent = m_Enemies[i];
			if (glm::length(agent->Steering) != 0)
				agent->Velocity = glm::normalize(agent->Velocity + agent->Steering) * max_speed;
			else
				agent->Velocity = glm::vec3(0.f);
			agent->Steering = glm::vec3(0.f);
			agent->Position += agent->Velocity * dt;
		}
	}

	void AIDemoWorld::LoadAssets()
	{
		// Models
		GDP_LoadModel(m_SphereModelId, "assets/models/uvsphere.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_BallMaterialId, m_WoodTextureId, glm::vec3(1.0f));
		GDP_CreateMaterial(m_SeekMaterialId, m_WoodTextureId, glm::vec3(1.0f, 0.5f, 0.5f));
		GDP_CreateMaterial(m_FleeMaterialId, m_WoodTextureId, glm::vec3(0.5f, 0.5f, 1.0f));
		GDP_CreateMaterial(m_ObstacleMaterialId, m_WoodTextureId, glm::vec3(0.15f));
	}

	GameObject* AIDemoWorld::CreateAgent(const glm::vec3& position)
	{
		GameObject* ball = GDP_CreateGameObject();
		ball->Position = position;
		ball->Scale = glm::vec3(1.f);
		ball->Renderer.ShaderId = 1;
		ball->Renderer.MeshId = m_SphereModelId;
		ball->Renderer.MaterialId = m_BallMaterialId;
		return ball;
	}

	GameObject* AIDemoWorld::CreateObstacle(const glm::vec3& position, const glm::vec3& size)
	{
		GameObject* obstacle = GDP_CreateGameObject();
		obstacle->Position = position;
		obstacle->Scale = size;
		obstacle->Renderer.ShaderId = 1;
		obstacle->Renderer.MeshId = m_SphereModelId;
		obstacle->Renderer.MaterialId = m_BallMaterialId;
		return obstacle;
	}
}