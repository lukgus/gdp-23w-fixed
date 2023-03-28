#pragma once

#include "World.h"
#include "Engine.h"

#include <queue>
#include <vector>
#include <glm/vec3.hpp>
using namespace std;

namespace gdp
{
	struct PathNode
	{
		PathNode(char name)
			: name(name)
			, parent(nullptr)
		{
		}

		glm::vec3 point;
		PathNode* parent;
		int id;
		char name;
		float distanceFromStart;
		vector<PathNode*> neighbours;

		bool operator() (const PathNode* l, const PathNode* r) const
		{ 
			return l->distanceFromStart > r->distanceFromStart;
		}

		PathNode() : name(' '){}
	};

	struct Graph
	{
		//PathNode* AddNode();
		//void SetNeighbours(PathNode* a, PathNode* b);
		void SetNeighbours(int a, int b);

		vector<PathNode*> nodes;
	};

	class AIPathFindingDemoWorld : public World
	{
	public:
		AIPathFindingDemoWorld();
		~AIPathFindingDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Update(float dt) override;
		virtual void Render() override;

	private:
		void LoadAssets();
		void CreateGraph();
		void CreatePathNode(const glm::vec3& position, char name);

		bool BreadthFirstSearch(Graph* graph, PathNode* start, PathNode* end);
		bool DepthFirstSearch(Graph* graph, PathNode* start, PathNode* end);
		bool DijkstraSearch(Graph* graph, PathNode* start, PathNode* end);
		bool AStarSearch(Graph* graph, PathNode* start, PathNode* end);

		void RenderConnection(PathNode* a, PathNode* b);
		void DrawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color);

		unsigned int m_SphereModelId;
		unsigned int m_WoodTextureId;

		unsigned int m_OpenMaterialId;
		unsigned int m_ClosedMaterialId;
		unsigned int m_UnvisitedMaterialId;
		unsigned int m_FoundPathMaterialId;

		vector<PathNode*> m_OpenSet;
		vector<PathNode*> m_ClosedSet;
		PathNode* m_StartNode;
		PathNode* m_EndNode;

		// Create a Graph and nodes
		Graph m_Graph;
		typedef struct sFloat4 {
			float x, y, z, w;
		} sFloat4;

		typedef struct sVertex_p4c4 {
			sFloat4 Pos;
			sFloat4 Color;
		} sVertex_p4c4;

		typedef struct sVertex_p4 {
			sFloat4 Pos;
		} sVertex_p4;

		GLuint m_VertexBufferId;
		GLuint m_IndexBufferId;
		GLuint m_VBO;
		GLuint m_ShaderId;

		std::vector<GameObject*> m_PathNodeGameObjects;
	};
}