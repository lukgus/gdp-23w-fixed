#pragma once

#include "GL.h"

#include <map>
#include <string>
#include <vector>

namespace gdp
{

	class Model {
	public:
		Model();
		Model(const char* filepath);
		Model(const char* filepath, bool withBones);
		Model(const std::vector<glm::vec3>& Vertices, const std::vector<int>& triangles);
		~Model();

		GLuint Vbo;
		GLuint VertexBufferId;
		GLuint IndexBufferId;

		std::vector<glm::vec3> Vertices;
		std::vector<int> triangles;	// 1,2,3

		unsigned int NumTriangles;
	};
}