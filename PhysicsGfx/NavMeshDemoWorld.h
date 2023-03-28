#pragma once

#include "World.h"
#include "Engine.h"

#include <queue>
#include <vector>
#include <glm/vec3.hpp>
using namespace std;

#define GENERATE_VISUAL_DEBUG_NAVMESH

namespace gdp
{
	class NavMeshDemoWorld : public World
	{
	private:
		class NavMeshNode
		{
		public:
			NavMeshNode() {}
			NavMeshNode(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c);
			~NavMeshNode() {}

			void AddNeighbour(NavMeshNode* neighbour);

			glm::vec3 CenterPoint;

			// Do we want to store Index values or vertices
			// This can depend on your Mesh exporter, asset pipeline process,
			// and your mesh importer settings
			glm::vec3 Vertices[3];
			
			// Think of a way to find neighbours.
			// How do we define neighbours?
			std::vector<NavMeshNode*> Neighbours;
		};

		class NavMesh
		{
		public:
			NavMesh() {}
			~NavMesh() {}

			void SetNeighbours();
			void AddNavMeshNode(NavMeshNode* node);
		private:
			bool IsValidNeighbour(int idxA, int idxB);

			std::vector<NavMeshNode*> m_Nodes;
		};

	public:
		NavMeshDemoWorld();
		~NavMeshDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Update(float dt) override;
		virtual void Render() override;

	private:
		void LoadAssets();
		void CreateNavMesh();

		NavMesh* m_NavMesh;

		unsigned int m_ModelForNavMeshId;
		unsigned int m_WoodTextureId;
		unsigned int m_WoodMaterialId;

		float m_CameraMoveSpeed;
		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraDirection;

		GameObject* m_Torus;

#ifdef GENERATE_VISUAL_DEBUG_NAVMESH
		GameObject* m_DebugNavMesh;
		unsigned int m_VisualDebugNavMeshModelId;
		unsigned int m_VisualDebugNavMeshMateriallId;
		unsigned int m_NavMeshMaterialId;
#endif
	};
}