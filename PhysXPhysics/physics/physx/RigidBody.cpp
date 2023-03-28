#include "RigidBody.h"

#include "Conversion.h"

namespace physics
{
	RigidBody* RigidBody::Cast(iCollisionBody* body)
	{
		return dynamic_cast<RigidBody*>(body);
	}

	RigidBody::RigidBody(const RigidBodyDesc& desc, iShape* shape)
	{
	}

	RigidBody::~RigidBody(void)
	{
	}

	bool RigidBody::IsStatic(void) const
	{
		return false;
	}

	iShape* RigidBody::GetShape(void)
	{
		return nullptr;
	}

	//void RigidBody::GetWorldTransform(glm::mat4& transformOut)
	//{
	//}

	void RigidBody::GetPosition(glm::vec3& positionOut)
	{
	}

	void RigidBody::SetPosition(const glm::vec3& positionIn)
	{
		// Can't.
	}

	void RigidBody::GetRotation(glm::quat& orientationOut)
	{
	}

	void RigidBody::SetRotation(const glm::quat& orientationIn)
	{
		// Can't
	}

	void RigidBody::ApplyForce(const glm::vec3& force)
	{
	}

	void RigidBody::ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint)
	{
	}

	void RigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
	}

	void RigidBody::ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint)
	{
	}

	void RigidBody::ApplyTorque(const glm::vec3& torque)
	{
	}

	void RigidBody::ApplyTorqueImpulse(const glm::vec3& torqueImpulse)
	{
	}

};