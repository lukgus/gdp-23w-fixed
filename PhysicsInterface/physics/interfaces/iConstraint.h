#pragma once

#include "ConstraintType.h"

namespace physics
{
	class iConstraint
	{
	public:
		virtual ~iConstraint() {}

		ConstraintType GetConstraintType() const {
			return m_ConstraintType;
		}

	protected:
		iConstraint(ConstraintType constraintType)
			: m_ConstraintType(constraintType)
		{ }

	private:
		ConstraintType m_ConstraintType;

		iConstraint(const iConstraint&) = delete;
		iConstraint& operator=(const iConstraint&) {}
	};
}
