#include "NavMeshDemoWorld.h"

#include <assert.h>
#include <queue>
//#define DISPLAY_DEBUG_SPHERES 
#include "vertex_types.h"
#include "CheckGLError.h"

namespace gdp
{

	NavMeshDemoWorld::NavMeshNode::NavMeshNode(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c)
	{
		Vertices[0] = a;
		Vertices[1] = b;
		Vertices[2] = c;
		CenterPoint = (a + b + c) / 3.f;
	}

	void NavMeshDemoWorld::NavMeshNode::AddNeighbour(NavMeshNode* neighbour)
	{
		Neighbours.push_back(neighbour);
	}

	void NavMeshDemoWorld::NavMesh::SetNeighbours()
	{
		for (int nodeAIdx = 0; nodeAIdx < m_Nodes.size() - 1; nodeAIdx++)
		{
			for (int nodeBIdx = nodeAIdx + 1; nodeBIdx < m_Nodes.size(); nodeBIdx++)
			{
				if (IsValidNeighbour(nodeAIdx, nodeBIdx))
				{
					m_Nodes[nodeAIdx]->AddNeighbour(m_Nodes[nodeBIdx]);
					m_Nodes[nodeBIdx]->AddNeighbour(m_Nodes[nodeAIdx]);
				}
			}
		}
	}

	void NavMeshDemoWorld::NavMesh::AddNavMeshNode(NavMeshNode* node)
	{
		m_Nodes.push_back(node);
	}

	bool NavMeshDemoWorld::NavMesh::IsValidNeighbour(int idxA, int idxB)
	{
		int similarCount = 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (m_Nodes[idxA]->Vertices[i] == m_Nodes[idxB]->Vertices[j])
					similarCount++;
			}
		}

		if (similarCount == 2)
			return true;

		if (similarCount > 2)
		{
			printf("NavMeshDemoWorld::NavMesh::IsValidNeighbour: A duplicate triangle has been added to the NavMesh\n");
		}

		return false;
	}

	NavMeshDemoWorld::NavMeshDemoWorld()
	{
		m_NavMesh = new NavMesh();

		m_CameraPosition = glm::vec3(0, 62, 300);
		m_CameraDirection = glm::vec3(0, -.2, -0.8);
		m_CameraMoveSpeed = 70.f;
	}

	NavMeshDemoWorld::~NavMeshDemoWorld()
	{

	}

	void NavMeshDemoWorld::Initialize()
	{
		LoadAssets();
		CreateNavMesh();
	}

	void NavMeshDemoWorld::Destroy()
	{

	}

	void NavMeshDemoWorld::Update(float dt)
	{
		// Bind a key to show/hide the original model
		if (GDP_IsKeyHeldDown('w'))
		{
			m_CameraPosition.z -= m_CameraMoveSpeed * dt;
		}
		if (GDP_IsKeyHeldDown('s'))
		{
			m_CameraPosition.z += m_CameraMoveSpeed * dt;
		}
		if (GDP_IsKeyHeldDown('d'))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * dt;
		}
		if (GDP_IsKeyHeldDown('a'))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		}
		if (GDP_IsKeyHeldDown('q'))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * dt;
		}
		if (GDP_IsKeyHeldDown('e'))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;
		}

		SetCameraPosition(m_CameraPosition);
		SetCameraFacingDirection(m_CameraDirection);

#ifdef GENERATE_VISUAL_DEBUG_NAVMESH
		// Bind a key to show/hide the generated nav mesh 
		if (GDP_IsKeyPressed('b'))
		{
			m_Torus->Enabled = !m_Torus->Enabled;
		}
		if (GDP_IsKeyPressed('v'))
		{
			m_DebugNavMesh->Enabled = !m_DebugNavMesh->Enabled;
		}
#endif
	}

	void NavMeshDemoWorld::Render()
	{
	}

	void NavMeshDemoWorld::LoadAssets()
	{
		// Models
		//GDP_LoadModel(m_ModelForNavMeshId, "assets/models/cube.fbx");
		//GDP_LoadModel(m_ModelForNavMeshId, "assets/models/torus.fbx");
		GDP_LoadModel(m_ModelForNavMeshId, "assets/models/helmsdeep_working.ply");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_WoodMaterialId, m_WoodTextureId, glm::vec3(1.0f, 1.f, 1.f));

#ifdef GENERATE_VISUAL_DEBUG_NAVMESH
		GDP_CreateMaterial(m_NavMeshMaterialId, m_WoodTextureId, glm::vec3(0.0f, 0.f, 1.f));
#endif
		m_Torus = GDP_CreateGameObject();
		m_Torus->Position = glm::vec3(0.f);
		m_Torus->Scale = glm::vec3(1.f);
		m_Torus->Renderer.ShaderId = 1;
		m_Torus->Renderer.MeshId = m_ModelForNavMeshId;
		m_Torus->Renderer.MaterialId = m_WoodMaterialId;

	}

	void NavMeshDemoWorld::CreateNavMesh()
	{

#ifdef GENERATE_VISUAL_DEBUG_NAVMESH
		std::vector<glm::vec3> debugVertices;
		std::vector<int> debugIndices;
#endif

		std::vector<glm::vec3> Vertices;
		std::vector<int> triangles;
		unsigned int numTriangles;
		unsigned int vbo;
		GDP_GetModelData(m_ModelForNavMeshId, Vertices, triangles, numTriangles, vbo);


		// Comment in chat or use your microphone to answer or guess
		const glm::vec3 upVector = glm::vec3(0, 1, 0);
		float validSlope = 0.3f;
		// Up Vector!

		// Each triangle will be a node
		// Each triangle is 3 entries (+=3)
		for (int index = 0; index < triangles.size(); index+=3)
		{
			const glm::vec3 v1 = Vertices[triangles[index]];
			const glm::vec3 v2 = Vertices[triangles[index+1]];
			const glm::vec3 v3 = Vertices[triangles[index+2]];

			const glm::vec3 A = glm::normalize(v1 - v2);
			const glm::vec3 B = glm::normalize(v1 - v3);

			glm::vec3 c = glm::cross(A, B);
			float slope = glm::abs(glm::dot(c, upVector));

			if (slope >= validSlope)
			{
				// This is a valid, walkable, traversable face.
				// We can use this face as a node in our 
				// navigational mesh.

				// Add our Node to our NavMesh.
				NavMeshNode* navMeshNode = new NavMeshNode(v1, v2, v3);

				m_NavMesh->AddNavMeshNode(navMeshNode);


#ifdef GENERATE_VISUAL_DEBUG_NAVMESH
				int debugIndex = debugVertices.size();

				debugIndices.push_back(debugIndex);
				debugIndices.push_back(debugIndex + 1);
				debugIndices.push_back(debugIndex + 2);

				debugVertices.push_back(v1);
				debugVertices.push_back(v2);
				debugVertices.push_back(v3);

				printf("%d %d %d >", debugIndex, debugIndex + 1, debugIndex + 2);
				printf("(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)\n", 
					v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
#endif
			}
		}

		// Set neighbours so we can utilize our NavMesh for path finding
		m_NavMesh->SetNeighbours();

		// Create a visual mesh to render
#ifdef GENERATE_VISUAL_DEBUG_NAVMESH
		GDP_CreateModel(m_VisualDebugNavMeshModelId, debugVertices, debugIndices);
		m_DebugNavMesh = GDP_CreateGameObject();
		m_DebugNavMesh->Position = glm::vec3(0.f);
		m_DebugNavMesh->Scale = glm::vec3(1.f);
		m_DebugNavMesh->Renderer.RenderFrontAndBack = true;
		m_DebugNavMesh->Renderer.ShaderId = 1;
		m_DebugNavMesh->Renderer.MeshId = m_VisualDebugNavMeshModelId;
		m_DebugNavMesh->Renderer.MaterialId = m_NavMeshMaterialId;
#endif
	}
}