#include "ConeTwistConstraint.h"

namespace physics
{
	ConeTwistConstraint::ConeTwistConstraint(
		iRigidBody* rbA, iRigidBody* rbB, glm::mat4 transformA, glm::mat4 transformB)
		: iConstraint(ConstraintType::ConeTwist)
		, m_RigidBodyA(rbA)
		, m_RigidBodyB(rbB)
		, m_TransformA(transformA)
		, m_TransformB(transformB)
	{

	}

	ConeTwistConstraint::ConeTwistConstraint(iRigidBody* rbA, glm::mat4 transformA)
		: ConeTwistConstraint(rbA, nullptr, transformA, glm::mat4())
	{ }

	ConeTwistConstraint::~ConeTwistConstraint()
	{
	}

	ConeTwistConstraint* ConeTwistConstraint::Cast(iConstraint* constraint)
	{
		return dynamic_cast<ConeTwistConstraint*>(constraint);
	}

}