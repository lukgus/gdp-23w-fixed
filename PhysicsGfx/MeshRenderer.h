#pragma once

namespace gdp
{
	struct MeshRenderer {
		bool RenderFrontAndBack = false;
		unsigned int MeshId;
		unsigned int ShaderId;
		unsigned int MaterialId;
	};
}