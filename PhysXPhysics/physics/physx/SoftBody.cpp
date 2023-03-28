#include "SoftBody.h"

#include <algorithm>

#include <physics\interfaces\iShape.h>

#include <cfloat>

namespace physics
{
	SoftBody* SoftBody::Cast(iCollisionBody* body)
	{
		return dynamic_cast<SoftBody*>(body);
	}

	SoftBody::SoftBody(const SoftBodyDesc& desc)
	{
	}

	SoftBody::~SoftBody(void)
	{
	}

	unsigned int SoftBody::GetNumNodes()
	{
		return 0;
	}

	void SoftBody::GetNodePosition(unsigned int nodeIndex, glm::vec3& nodePositionOut)
	{
		nodePositionOut = glm::vec3(0.f);
	}

};