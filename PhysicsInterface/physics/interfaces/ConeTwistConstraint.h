#pragma once

#include "iConstraint.h"
#include "iRigidBody.h"
#include "Math.h"

namespace physics
{
	class ConeTwistConstraint : public iConstraint
	{
	public:
		ConeTwistConstraint(iRigidBody* rbA, iRigidBody* rbB, 
			glm::mat4 transformA, glm::mat4 transformB);
		ConeTwistConstraint(iRigidBody* rbA, glm::mat4 transformA);

		virtual ~ConeTwistConstraint();

		static ConeTwistConstraint* Cast(iConstraint* constraint);

		iRigidBody* GetRididBodyA() const { return m_RigidBodyA; }
		iRigidBody* GetRididBodyB() const { return m_RigidBodyB; }
		const glm::mat4& GetTransformA() const { return m_TransformA; }
		const glm::mat4& GetTransformB() const { return m_TransformB; }

	protected:
		ConeTwistConstraint() = delete;

	private:
		iRigidBody* m_RigidBodyA;
		iRigidBody* m_RigidBodyB;
		glm::mat4 m_TransformA;
		glm::mat4 m_TransformB;

		ConeTwistConstraint(const ConeTwistConstraint&) : iConstraint(ConstraintType::ConeTwist) {}
		ConeTwistConstraint& operator=(const ConeTwistConstraint&) {
			return *this;
		}

	};

}