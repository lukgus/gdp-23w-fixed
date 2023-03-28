#include "AIPathFindingDemoWorld.h"

#include <assert.h>
#include <queue>
//#define DISPLAY_DEBUG_SPHERES 
#include "vertex_types.h"
#include "CheckGLError.h"

namespace gdp
{

	void PrintSet(const std::string& name, const std::vector<PathNode*>& set)
	{
		printf("%s { ", name.c_str());

		for (int i = 0; i < set.size(); i++)
		{
			printf("%c ", set[i]->name);
		}

		printf("}\n");
	}

	std::vector<PathNode*>::iterator GetPathNodeWithShortestDistanceFromStart(std::vector<PathNode*>& set)
	{
		std::vector<PathNode*>::iterator findIt;
		std::vector<PathNode*>::iterator shortestIt;
		float shortestDistance = FLT_MAX;
		for (findIt = set.begin(); findIt != set.end(); ++findIt)
		{
			if ((*findIt)->distanceFromStart < shortestDistance)
			{
				shortestDistance = (*findIt)->distanceFromStart;
				shortestIt = findIt;
			}
		}

		return shortestIt;
	}

	void Graph::SetNeighbours(int a, int b)
	{
		assert(a >= 0);
		assert(b >= 0);
		assert(a < nodes.size());
		assert(b < nodes.size());

		nodes[a]->neighbours.push_back(nodes[b]);
		nodes[b]->neighbours.push_back(nodes[a]);
	}

	AIPathFindingDemoWorld::AIPathFindingDemoWorld()
	{

	}

	AIPathFindingDemoWorld::~AIPathFindingDemoWorld()
	{

	}

	void AIPathFindingDemoWorld::Initialize()
	{
		unsigned int numVerticesInVertArray = 2;
		unsigned int numIndicesInIndexArray = 2;
		sVertex_p4c4* pTempVertArray = new sVertex_p4c4[numIndicesInIndexArray];
		GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray];

		pTempVertArray[0].Pos.x = 10.f;
		pTempVertArray[0].Pos.y = 0.f;
		pTempVertArray[0].Pos.z = 0.f;
		pTempVertArray[0].Pos.w = 1.f;
		pTempVertArray[0].Color.x = 0.f;
		pTempVertArray[0].Color.y = 1.f;
		pTempVertArray[0].Color.z = 0.f;
		pTempVertArray[0].Color.w = 0.f;

		pTempVertArray[1].Pos.x = 0.f;
		pTempVertArray[1].Pos.y = 0.f;
		pTempVertArray[1].Pos.z = 0.f;
		pTempVertArray[1].Pos.w = 1.f;
		pTempVertArray[1].Color.x = 0.f;
		pTempVertArray[1].Color.y = 1.f;
		pTempVertArray[1].Color.z = 0.f;
		pTempVertArray[1].Color.w = 0.f;

		glGenVertexArrays(1, &m_VBO);
		glBindVertexArray(m_VBO);
		CheckGLError();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		CheckGLError();

		glGenBuffers(1, &m_VertexBufferId);
		glGenBuffers(1, &m_IndexBufferId);
		CheckGLError();

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		CheckGLError();

		unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4c4);
		glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_DYNAMIC_DRAW);
		CheckGLError();

		unsigned int bytesInOneVertex = sizeof(sVertex_p4c4);
		unsigned int byteOffsetToPosition = offsetof(sVertex_p4c4, Pos);
		unsigned int byteOffsetToColor = offsetof(sVertex_p4c4, Color);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToColor);
		CheckGLError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
		CheckGLError();

		unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_DYNAMIC_DRAW);
		CheckGLError();

		glBindVertexArray(0);
		CheckGLError();

		delete[] pTempVertArray;
		delete[] pIndexArrayLocal;

		//priority_queue<PathNode*, std::vector<PathNode*>, PathNode> test;

		//PathNode* a = new PathNode('A');
		//PathNode* b = new PathNode('B');
		//PathNode* c = new PathNode('C');
		//PathNode* d = new PathNode('D');
		//PathNode* e = new PathNode('E');

		//a->distanceFromStart = 3;
		//b->distanceFromStart = 2;
		//c->distanceFromStart = 12;
		//d->distanceFromStart = 1;
		//e->distanceFromStart = 10;

		//test.push(a);
		//test.push(b);
		//test.push(c);
		//test.push(d);
		//test.push(e);

		//printf("\n");

		//PathNode* top;
		//top = test.top();
		//test.pop();
		//printf("%c : %.2f\n", top->name, top->distanceFromStart);
		//top = test.top();
		//test.pop();
		//printf("%c : %.2f\n", top->name, top->distanceFromStart);
		//top = test.top();
		//test.pop();
		//printf("%c : %.2f\n", top->name, top->distanceFromStart);
		//top = test.top();
		//test.pop();
		//printf("%c : %.2f\n", top->name, top->distanceFromStart);
		//top = test.top();
		//test.pop();
		//printf("%c : %.2f\n", top->name, top->distanceFromStart);

		LoadAssets();
		CreateGraph();

		//bool pathFound = DepthFirstSearch(&m_Graph, m_Graph.nodes[19], m_Graph.nodes[20]);
		//if (pathFound)
		//{
		//	printf("Found a path between T and U!\n");

		//	PathNode* path = m_Graph.nodes[20];
		//	while (path != nullptr)
		//	{
		//		printf("%c ", path->name);
		//		path = path->parent;
		//	}
		//	printf("\n");
		//}
		//else
		//{
		//	printf("Did not find a path between T and U!\n");
		//}

		m_StartNode = m_Graph.nodes[19];
		m_EndNode = m_Graph.nodes[20];
		m_OpenSet.push_back(m_StartNode);
		printf("Added %c to the open set!\n", m_StartNode->name);
		m_PathNodeGameObjects[m_StartNode->id]->Renderer.MaterialId = m_OpenMaterialId;
	}

	void AIPathFindingDemoWorld::Destroy()
	{

	}

	void AIPathFindingDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0.f, 0.f, 20.f));
		SetCameraFacingDirection(glm::vec3(0.f, 0.f, -1.f));

		if (GDP_IsKeyPressed('7') && !m_OpenSet.empty())
		{
			PrintSet("open:   ", m_OpenSet);
			PrintSet("closed: ", m_ClosedSet);

			vector<PathNode*>::iterator itX = GetPathNodeWithShortestDistanceFromStart(m_OpenSet);
			PathNode* X = *itX;
			m_OpenSet.erase(itX);

			printf("Removed %c from the open set!\n", X->name);

			if (X == m_EndNode)
				return;

			m_ClosedSet.push_back(X);
			m_PathNodeGameObjects[X->id]->Renderer.MaterialId = m_ClosedMaterialId;
			printf("Added %c to the closed set!\n", X->name);

			printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
			for (int i = 0; i < X->neighbours.size(); i++)
			{
				PathNode* n = X->neighbours[i];
				printf("Found neighbour %c!\n", n->name);
				if (std::find(m_ClosedSet.begin(), m_ClosedSet.end(), n) != m_ClosedSet.end())
				{
					continue;
				}
				else if (std::find(m_OpenSet.begin(), m_OpenSet.end(), n) != m_OpenSet.end())
				{
					// Distance from n to it's parent + distance from parent to start
					float distanceFromStartToN = glm::distance(n->point, X->point) + X->distanceFromStart;
					if (distanceFromStartToN < n->distanceFromStart)
					{
						n->parent = X;
						n->distanceFromStart = distanceFromStartToN;
					}
				}
				else
				{
					m_OpenSet.insert(m_OpenSet.begin(), n);
					m_PathNodeGameObjects[n->id]->Renderer.MaterialId = m_OpenMaterialId;
					n->parent = X;
					printf("Added %c to the open set!\n", n->name);
				}
			}
		}
	}

	void AIPathFindingDemoWorld::DrawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color)
	{
		unsigned int bytesInOneVertex = sizeof(sVertex_p4c4);
		unsigned int byteOffsetToPosition = offsetof(sVertex_p4c4, Pos);
		unsigned int byteOffsetToColor = offsetof(sVertex_p4c4, Color);

		// Depending on how your model is loaded and stored
		// You may not have a list of all of your triangles in order
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		CheckGLError();

		glm::vec4 orgin(0.f, 0.f, 0.f, 1.f);
		glm::vec4 left(.5f, .5f, 0.f, 1.f);

		GLsizeiptr size = GLsizeiptr(sizeof(float));

		// FROM Point
		glBufferSubData(GL_ARRAY_BUFFER,
			/* offset */	GLintptr(0),
			/* size */		size * 3,
			/* data */		&a);
		CheckGLError();

		// FROM Color
		glBufferSubData(GL_ARRAY_BUFFER,
			/* offset */	GLintptr(byteOffsetToColor),
			/* size */		size * 3,
			/* data */		&color);
		CheckGLError();

		glBufferSubData(GL_ARRAY_BUFFER,
			/* offset */	GLintptr(bytesInOneVertex),
			/* size */		size * 3,
			/* data */		&b);
		CheckGLError();

		// FROM Color
		glBufferSubData(GL_ARRAY_BUFFER,
			/* offset */	GLintptr(bytesInOneVertex + byteOffsetToColor),
			/* size */		size * 3,
			/* data */		&color);
		CheckGLError();

		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindVertexArray(m_VBO);
		CheckGLError();
		glDrawArrays(GL_LINES, 0, 2);
		//glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (GLvoid*)0);
		CheckGLError();
	}

	void AIPathFindingDemoWorld::RenderConnection(PathNode* a, PathNode* b)
	{
		DrawLine(a->point,b->point, glm::vec3(1.f));
	}

	void AIPathFindingDemoWorld::Render()
	{
		for (int i = 0; i < m_Graph.nodes.size(); i++)
		{
			if (m_Graph.nodes[i]->parent != nullptr)
			{
				RenderConnection(m_Graph.nodes[i], m_Graph.nodes[i]->parent);
			}
		}
	}

	void AIPathFindingDemoWorld::LoadAssets()
	{
		// Models
		GDP_LoadModel(m_SphereModelId, "assets/models/uvsphere.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_OpenMaterialId, m_WoodTextureId, glm::vec3(0.0f, 1.f, 0.f));
		GDP_CreateMaterial(m_ClosedMaterialId, m_WoodTextureId, glm::vec3(1.f, 0.f, 0.f));
		GDP_CreateMaterial(m_UnvisitedMaterialId, m_WoodTextureId, glm::vec3(1.f, 1.f, 1.f));
		GDP_CreateMaterial(m_FoundPathMaterialId, m_WoodTextureId, glm::vec3(0.f, 0.f, 1.f));
	}

	void AIPathFindingDemoWorld::CreateGraph()
	{
		CreatePathNode(glm::vec3(0.f, 5.f, 0.f), 'A');
		CreatePathNode(glm::vec3(-4.f, 3.f, 0.f), 'B');
		CreatePathNode(glm::vec3(0.f, 3.f, 0.f), 'C');
		CreatePathNode(glm::vec3(4.f, 3.f, 0.f), 'D');
		CreatePathNode(glm::vec3(-6.f, 1.f, 0.f), 'E');
		CreatePathNode(glm::vec3(-3.f, 1.f, 0.f), 'F');
		CreatePathNode(glm::vec3(-1.f, 1.f, 0.f), 'G');
		CreatePathNode(glm::vec3(1.f, 1.f, 0.f), 'H');
		CreatePathNode(glm::vec3(3.f, 1.f, 0.f), 'I');
		CreatePathNode(glm::vec3(6.f, 1.f, 0.f), 'J');
		CreatePathNode(glm::vec3(-7.f, -1.f, 0.f), 'K');
		CreatePathNode(glm::vec3(-5.f, -1.f, 0.f), 'L');
		CreatePathNode(glm::vec3(-3.f, -1.f, 0.f), 'M');
		CreatePathNode(glm::vec3(-1.f, -1.f, 0.f), 'N');
		CreatePathNode(glm::vec3(1.f, -1.f, 0.f), 'O');
		CreatePathNode(glm::vec3(3.f, -1.f, 0.f), 'P');
		CreatePathNode(glm::vec3(5.f, -1.f, 0.f), 'Q');
		CreatePathNode(glm::vec3(7.f, -1.f, 0.f), 'R');
		CreatePathNode(glm::vec3(-7.f, -3.f, 0.f), 'S');
		CreatePathNode(glm::vec3(-5.f, -3.f, 0.f), 'T');
		CreatePathNode(glm::vec3(3.f, -3.f, 0.f), 'U');

		m_Graph.SetNeighbours(0, 1);
		m_Graph.SetNeighbours(0, 2);
		m_Graph.SetNeighbours(0, 3);
		m_Graph.SetNeighbours(1, 4);
		m_Graph.SetNeighbours(1, 5);
		m_Graph.SetNeighbours(2, 6);
		m_Graph.SetNeighbours(2, 7);
		m_Graph.SetNeighbours(3, 8);
		m_Graph.SetNeighbours(3, 9);
		m_Graph.SetNeighbours(4, 10);
		m_Graph.SetNeighbours(4, 11);
		m_Graph.SetNeighbours(5, 11);
		m_Graph.SetNeighbours(5, 12);
		m_Graph.SetNeighbours(6, 13);
		m_Graph.SetNeighbours(7, 14);
		m_Graph.SetNeighbours(7, 15);
		m_Graph.SetNeighbours(8, 15);
		m_Graph.SetNeighbours(8, 16);
		m_Graph.SetNeighbours(9, 17);
		m_Graph.SetNeighbours(10, 18);
		m_Graph.SetNeighbours(11, 19);
		m_Graph.SetNeighbours(15, 20);
	}

	void AIPathFindingDemoWorld::CreatePathNode(const glm::vec3& position, char name)
	{
		PathNode* newPathNode = new PathNode(name);
		newPathNode->point = position;
		newPathNode->id = m_Graph.nodes.size();
		m_Graph.nodes.push_back(newPathNode);

		GameObject* ball = GDP_CreateGameObject();
		ball->Position = position;
		ball->Scale = glm::vec3(0.5f);
		ball->Renderer.ShaderId = 1;
		ball->Renderer.MeshId = m_SphereModelId;
		ball->Renderer.MaterialId = m_UnvisitedMaterialId;
		m_PathNodeGameObjects.push_back(ball);
	}
	/*
	*	Depth-First Search
	*	open : set
	*	closed : set
	*	insert start into the open set
	*	while open is not empty
	*		remove the first cell from open; store as X
	*		if X is a goal then return SUCCESS
	*		else
	*			generate children of X
	*			put X in the closed set
	*			discard children of X if already on open or closed
	*			put remaining children on beginning of open set
	*	return FAIL
	*/
	bool AIPathFindingDemoWorld::DepthFirstSearch(Graph* graph, PathNode* start, PathNode* end)
	{
		vector<PathNode*> open;
		vector<PathNode*> closed;
		open.push_back(start);
		printf("Added %c to the open set!\n", start->name);
		while (!open.empty())
		{
			PrintSet("open:   ", open);
			PrintSet("closed: ", closed);

			PathNode* X = open.back();
			open.pop_back();
			printf("Removed %c from the open set!\n", X->name);

			if (X == end)
				return true;

			closed.push_back(X);
			printf("Added %c to the closed set!\n", X->name);

			printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
			for (int i = 0; i < X->neighbours.size(); i++)
			{
				PathNode* n = X->neighbours[i];
				printf("Found neighbour %c!\n", n->name);
				if (std::find(open.begin(), open.end(), X->neighbours[i]) == open.end()
					&& std::find(closed.begin(), closed.end(), X->neighbours[i]) == closed.end())
				{
					open.push_back(n);
					n->parent = X;
					printf("Added %c to the open set!\n", n->name);
				}
			}
		}

		return false;
	}

	/*
	*	Breadth-First Search
	*	open : set
	*	closed : set
	*	insert start into the open set
	*	while open is not empty
	*		remove the first cell from open; store as X
	*		if X is a goal then return SUCCESS
	*		else
	*			generate children of X
	*			put X in the closed set
	*			discard the children of X if already in the open or closed set
	*			put remaining children at the end of the open set
	*	return FAIL
	*/
	bool AIPathFindingDemoWorld::BreadthFirstSearch(Graph* graph, PathNode* start, PathNode* end)
	{
		vector<PathNode*> open;
		vector<PathNode*> closed;
		open.push_back(start);
		printf("Added %c to the open set!\n", start->name);
		while (!open.empty())
		{
			PrintSet("open:   ", open);
			PrintSet("closed: ", closed);

			PathNode* X = open.back();
			open.pop_back();
			printf("Removed %c from the open set!\n", X->name);

			if (X == end)
				return true;

			closed.push_back(X);
			printf("Added %c to the closed set!\n", X->name);

			printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
			for (int i = 0; i < X->neighbours.size(); i++)
			{
				PathNode* n = X->neighbours[i];
				printf("Found neighbour %c!\n", n->name);
				if (std::find(open.begin(), open.end(), X->neighbours[i]) == open.end()
					&& std::find(closed.begin(), closed.end(), X->neighbours[i]) == closed.end())
				{
					open.insert(open.begin(), n);
					n->parent = X;
					printf("Added %c to the open set!\n", n->name);
				}
			}
		}

		return false;
	}

	/*
	* Dijkstra's Algorithm
	* open : set
	* closed : set
	* insert start into the open set
	* while open is not empty
	*	find node with shortest distance to our start node; store as X
	*	if x is goal then return SUCCESS
	*	else
	*		generate children of X
	*		put X in the closed set
	*		Add children not in open or closed set to the open set
	*		Calculate the distance to each new child from the start node
	*		If a child is a faster path from X than its current parent
	*			then set the parent to X
	* return FAIL
	*	
	*/

	bool AIPathFindingDemoWorld::DijkstraSearch(Graph* graph, PathNode* start, PathNode* end)
	{
		//priority_queue<PathNode*, std::vector<PathNode*>, PathNode> open;
		vector<PathNode*> open;
		vector<PathNode*> closed;

		open.push_back(start);
		printf("Added %c to the open set!\n", start->name);
		while (!open.empty())
		{
			PrintSet("open:   ", open);
			PrintSet("closed: ", closed);

			vector<PathNode*>::iterator itX = GetPathNodeWithShortestDistanceFromStart(open);
			PathNode* X = *itX;
			open.erase(itX);

			printf("Removed %c from the open set!\n", X->name);

			if (X == end)
				return true;

			closed.push_back(X);
			printf("Added %c to the closed set!\n", X->name);

			printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
			for (int i = 0; i < X->neighbours.size(); i++)
			{
				PathNode* n = X->neighbours[i];
				printf("Found neighbour %c!\n", n->name);
				if (std::find(closed.begin(), closed.end(), n) == closed.end())
				{
					continue;
				}
				else if (std::find(open.begin(), open.end(), n) != open.end())
				{
					// Distance from n to it's parent + distance from parent to start
					float distanceFromStartToN = glm::distance(n->point, X->point) + X->distanceFromStart;
					if (distanceFromStartToN < n->distanceFromStart)
					{
						n->parent = X;
						n->distanceFromStart = distanceFromStartToN;
					}
				}
				else
				{
					open.insert(open.begin(), n);
					n->parent = X;
					printf("Added %c to the open set!\n", n->name);
				}
			}
		}
	}

	/*
	* Your Project!
	* I will provide the algorithm
	* 
	*/
	bool AIPathFindingDemoWorld::AStarSearch(Graph* graph, PathNode* start, PathNode* end)
	{
		return false;
	}
}