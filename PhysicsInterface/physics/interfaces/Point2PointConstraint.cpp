#include "Point2PointConstraint.h"

namespace physics
{
	Point2PointConstraint::Point2PointConstraint(iRigidBody* rbA, iRigidBody* rbB, Vector3 pointA, Vector3 pointB)
		: iConstraint(ConstraintType::Point2Point)
		, m_RigidBodyA(rbA)
		, m_RigidBodyB(rbB)
		, m_PointA(pointA)
		, m_PointB(pointB)
	{

	}

	Point2PointConstraint::Point2PointConstraint(iRigidBody* rbA, Vector3 pointA)
		: Point2PointConstraint(rbA, nullptr, pointA, Vector3())
	{ }

	Point2PointConstraint::~Point2PointConstraint()
	{
	}

	Point2PointConstraint* Point2PointConstraint::Cast(iConstraint* constraint)
	{
		return dynamic_cast<Point2PointConstraint*>(constraint);
	}

}