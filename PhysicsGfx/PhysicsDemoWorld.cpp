#include "PhysicsDemoWorld.h"

#include "GL.h"
#include "Model.h"
#include "vertex_types.h"
#include "physics.h"

#include <physics/interfaces/SphereShape.h>
#include <physics/interfaces/PlaneShape.h>

namespace gdp
{
	PhysicsDemoWorld::PhysicsDemoWorld()
	{

	}

	PhysicsDemoWorld::~PhysicsDemoWorld()
	{

	}

	void PhysicsDemoWorld::Initialize()
	{
		GDP_GetPhysicsWorld()->SetGravity(glm::vec3(0, -9.81, 0));

		LoadAssets();
		CreateGround();
		//CreateCloth();

		CreateBall(glm::vec3(-5, 10, -5), 0.1f);
		CreateBall(glm::vec3(-5, 4, 5), 5.f);
		CreateBall(glm::vec3(5, 6, 5), 10.f);
		CreateBall(glm::vec3(5, 3, -5), 20.f);

		CreatePlayerBall(glm::vec3(10, 3, 10));

	}

	void PhysicsDemoWorld::Destroy()
	{

	}

	void PhysicsDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0, 12, 30));
		SetCameraFacingDirection(glm::vec3(0, -.2, -0.8));

		//UpdateDebugSpheres();
		//UpdateSoftBodyRendering();

		//if (GDP_IsKeyPressed('9'))
		//{
		//	for (int i = 0; i < m_DebugSphereList.size(); i++)
		//	{
		//		m_DebugSphereList[i]->Enabled = false;
		//	}
		//}
		//if (GDP_IsKeyPressed('0'))
		//{
		//	for (int i = 0; i < m_DebugSphereList.size(); i++)
		//	{
		//		m_DebugSphereList[i]->Enabled = true;
		//	}
		//}

		physics::iRigidBody* rigidBody = dynamic_cast<physics::iRigidBody*>(m_ControllableBall->PhysicsBody);
		if (rigidBody != nullptr)
		{
			float force = 5.f;
			glm::vec3 direction(0.f);


			if (GDP_IsKeyHeldDown('w'))
			{
				direction.z += -1;
			}
			if (GDP_IsKeyHeldDown('a'))
			{
				direction.x += -1;
			}
			if (GDP_IsKeyHeldDown('s'))
			{
				direction.z += 1;
			}
			if (GDP_IsKeyHeldDown('d'))
			{
				direction.x += 1;
			}
			if (GDP_IsKeyPressed(' '))
			{
				rigidBody->ApplyImpulse(glm::vec3(0.f, 3.5f, 0.f));
			}

			//rigidBody->ApplyTorque(direction * force);
			rigidBody->ApplyForce(direction * force);
			//rigidBody->ApplyForceAtPoint(direction * force, glm::vec3(0.f, 1.f, 0.f));

			// This should be updated right after physics update step.
			rigidBody->GetPosition(m_ControllableBall->Position);
			rigidBody->GetRotation(m_ControllableBall->Rotation);
		}

		for (int i = 0; i < m_BallList.size(); i++)
		{
			GameObject* ball = m_BallList[i];
			physics::iRigidBody* rigidBody = dynamic_cast<physics::iRigidBody*>(ball->PhysicsBody);
			if (ball->PhysicsBody != nullptr)
			{
				glm::vec3 position;

				rigidBody->GetPosition(position);
				rigidBody->GetRotation(ball->Rotation);

				ball->Position = position;

				if (position.x < -16.f)
				{
					rigidBody->ApplyForce(glm::vec3(1.f, 0.f, 0.f));
				}

				if (position.x > 16.f)
				{
					rigidBody->ApplyForce(glm::vec3(-1.f, 0.f, 0.f));
				}

				if (position.z < -16.f)
				{
					rigidBody->ApplyForce(glm::vec3(0.f, 0.f, 1.f));
				}

				if (position.z > 16.f)
				{
					rigidBody->ApplyForce(glm::vec3(0.f, 0.f, -1.f));
				}
			}
		}
	}

	void PhysicsDemoWorld::UpdateDebugSpheres()
	{
		physics::iSoftBody* cloth = dynamic_cast<physics::iSoftBody*>(m_ClothGameObject->PhysicsBody);
		for (int i = 0; i < cloth->GetNumNodes(); i++)
		{
			cloth->GetNodePosition(i, m_DebugSphereList[i]->Position);
		}
	}

	void PhysicsDemoWorld::UpdateSoftBodyRendering()
	{
		physics::iSoftBody* softBody = dynamic_cast<physics::iSoftBody*>(m_ClothGameObject->PhysicsBody);
		unsigned int numNodes = softBody->GetNumNodes();

		std::vector<glm::vec3> Vertices;
		std::vector<int> triangles;
		unsigned int numTriangles;
		unsigned int vbo;
		GDP_GetModelData(m_ClothModelId, Vertices, triangles, numTriangles, vbo);


		unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4);
		unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4, Pos);
		unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4, Normal);

		// Loop through each of the SoftBodyNodes and update the buffer data
		GLsizeiptr size = GLsizeiptr(sizeof(float));

		glm::vec3 nodeA, nodeB, nodeC;
		std::vector<glm::vec3> normals;
		normals.resize(numNodes, glm::vec3(0.f));
		for (int i = 0; i < numTriangles; i++)
		{
			int index1 = triangles[i];
			int index2 = triangles[i + 1];
			int index3 = triangles[i + 2];

			softBody->GetNodePosition(index1, nodeA);
			softBody->GetNodePosition(index2, nodeB);
			softBody->GetNodePosition(index3, nodeC);

			glm::vec3 CA = nodeC - nodeA;
			glm::vec3 CB = nodeC - nodeB;
			glm::vec3 normal = glm::normalize(glm::cross(CA, CB));

			normals[index1] += normal;
			normals[index2] += normal;
			normals[index3] += normal;
		}

		for (int i = 0; i < normals.size(); i++)
		{
			normals[i] = glm::normalize(normals[i]);
		}

		// Depending on how your model is loaded and stored
		// You may not have a list of all of your triangles in order
		glBindBuffer(GL_ARRAY_BUFFER, 5);
		glm::vec3 position, normal;
		int index = 0;
		for (int triIndex = 0; triIndex < triangles.size(); triIndex++)
		{
			softBody->GetNodePosition(triangles[triIndex], position);
			normal = normals[triangles[triIndex]];

			index = bytesInOneVertex * triIndex;

			//for (int i = 0; i < 3; i++)
			//{
				int boneOffset = index;

				// Position
				glBufferSubData(GL_ARRAY_BUFFER,
				/* offset */	GLintptr(byteOffsetToPosition + boneOffset),
				/* size */		size * 3,
				/* data */		&position);

				// Normal
				glBufferSubData(GL_ARRAY_BUFFER,
				/* offset */	GLintptr(byteOffsetToNormal + boneOffset),
				/* size */		size * 3,
				/* data */		&normal);
			//}
		}
	}

	void PhysicsDemoWorld::LoadAssets()
	{
		// Models
		GDP_LoadModel(m_PlaneModelId, "assets/models/plane.fbx");
		GDP_LoadModel(m_SphereModelId, "assets/models/uvsphere.fbx");
		GDP_LoadModel(m_ClothModelId, "assets/models/cloth5x5.obj");

		// Textures
		GDP_LoadTexture(m_ClothTextureId, "assets/textures/camo.bmp");
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");
		GDP_LoadTexture(m_BlankTextureId, "assets/textures/MetalPipeWallRusty_opacity.png");

		// Create Materials
		GDP_CreateMaterial(m_ClothMaterialId, m_ClothTextureId, glm::vec3(1.0f));
		GDP_CreateMaterial(m_GroundMaterialId, m_WoodTextureId, glm::vec3(1.0f));
		GDP_CreateMaterial(m_RedBallMaterialId, m_WoodTextureId, glm::vec3(1.0f, 0.8f, 0.8f));
		GDP_CreateMaterial(m_BlueBallMaterialId, m_WoodTextureId, glm::vec3(0.8f, 0.8f, 1.0f));
	}

	void PhysicsDemoWorld::CreateCloth()
	{
		m_ClothGameObject = GDP_CreateGameObject();
		m_ClothGameObject->Position = glm::vec3(0, 0, 0);
		m_ClothGameObject->Renderer.ShaderId = 1;
		m_ClothGameObject->Renderer.MeshId = m_ClothModelId;
		m_ClothGameObject->Renderer.MaterialId = m_ClothMaterialId;
		
		physics::SoftBodyDesc desc;
		unsigned int numTriangles;
		unsigned int vbo;
		GDP_GetModelData(m_ClothModelId, desc.NodePositions, desc.TriangulatedIndices, numTriangles, vbo);
		for (int i = 0; i < desc.NodePositions.size(); i++)
		{
			desc.NodePositions[i] += glm::vec3(0.f, 2.5f, 0.f);
		}
		physics::iSoftBody* softBody = GDP_CreateSoftBody(desc);
		m_ClothGameObject->PhysicsBody = softBody;
		GDP_GetPhysicsWorld()->AddBody(m_ClothGameObject->PhysicsBody);
		

		for (int i = 0; i < desc.NodePositions.size(); i++)
		{
			GameObject* debugSphere = GDP_CreateGameObject();
			debugSphere->Position = glm::vec3(0, 0, 0);
			debugSphere->Scale = glm::vec3(0.1f);
			debugSphere->Renderer.ShaderId = 1;
			debugSphere->Renderer.MeshId = m_SphereModelId;
			debugSphere->Renderer.MaterialId = m_GroundMaterialId;
			m_DebugSphereList.push_back(debugSphere);
		}
	}

	void PhysicsDemoWorld::CreateGround()
	{
		GameObject* ground = GDP_CreateGameObject();
		ground->Position = glm::vec3(0, 0, 0);
		ground->Scale = glm::vec3(16, 16, 1);
		ground->Rotation = glm::angleAxis(glm::radians(-90.f), glm::vec3(1, 0, 0));
		ground->Renderer.ShaderId = 1;
		ground->Renderer.MeshId = m_PlaneModelId;
		ground->Renderer.MaterialId = m_GroundMaterialId;

		physics::iShape* planeShape = new physics::PlaneShape(0.0f, glm::vec3(0.f, 1.f, 0.f));
		physics::RigidBodyDesc desc;
		desc.isStatic = true;
		desc.mass = 0;
		desc.position = glm::vec3(0.f);
		desc.linearVelocity = glm::vec3(0.f);
		ground->PhysicsBody = GDP_CreateRigidBody(desc, planeShape);
		GDP_GetPhysicsWorld()->AddBody(ground->PhysicsBody);
	}

	void PhysicsDemoWorld::CreateBall(const glm::vec3& position, float size)
	{
		GameObject* ball = GDP_CreateGameObject();
		ball->Position = position;
		ball->Scale = glm::vec3(1.f);
		ball->Renderer.ShaderId = 1;
		ball->Renderer.MeshId = m_SphereModelId;
		ball->Renderer.MaterialId = m_BlueBallMaterialId;

		physics::iShape* ballShape = new physics::SphereShape(1.0f);
		physics::RigidBodyDesc desc;
		desc.isStatic = false;
		desc.mass = size;
		desc.position = position;
		desc.linearVelocity = glm::vec3(0.f);
		ball->PhysicsBody = GDP_CreateRigidBody(desc, ballShape);

		m_BallList.push_back(ball);
		GDP_GetPhysicsWorld()->AddBody(ball->PhysicsBody);
	}

	void PhysicsDemoWorld::CreatePlayerBall(const glm::vec3& position)
	{
		m_ControllableBall = GDP_CreateGameObject();
		m_ControllableBall->Position = position;
		m_ControllableBall->Renderer.ShaderId = 1;
		m_ControllableBall->Renderer.MeshId = m_SphereModelId;
		m_ControllableBall->Renderer.MaterialId = m_BlueBallMaterialId;

		physics::iShape* ballShape = new physics::SphereShape(1.0f);
		physics::RigidBodyDesc desc;
		desc.isStatic = false;
		desc.mass = 1.f;
		desc.position = position;
		desc.linearVelocity = glm::vec3(0.f);
		m_ControllableBall->PhysicsBody = GDP_CreateRigidBody(desc, ballShape);

		GDP_GetPhysicsWorld()->AddBody(m_ControllableBall->PhysicsBody);
	}
}