#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <physics/interfaces/iRigidBody.h>
#include <physics/interfaces/RigidBodyDesc.h>
#include <physics/interfaces/iShape.h>


namespace physics
{
	class CollisionHandler;
	class RigidBody : public iRigidBody
	{
	public:
		RigidBody(const RigidBodyDesc& desc, iShape* shape);
		~RigidBody();

		static RigidBody* Cast(iCollisionBody* body);

		iShape* GetShape(void);

		bool IsStatic(void) const;

		//virtual void GetWorldTransform(glm::mat4& transformOut) override;

		virtual void GetPosition(glm::vec3& positionOut) override;
		virtual void SetPosition(const glm::vec3& positionIn) override;

		virtual void GetRotation(glm::quat& orientationOut) override;
		virtual void SetRotation(const glm::quat& orientationIn) override;

		virtual void ApplyForce(const glm::vec3& force) override;
		virtual void ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint) override;

		virtual void ApplyImpulse(const glm::vec3& impulse) override;
		virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint) override;

		virtual void ApplyTorque(const glm::vec3& torque) override;
		virtual void ApplyTorqueImpulse(const glm::vec3& torqueImpulse) override;

	protected:

	private:
		RigidBody(const RigidBody& other) { ; }
		RigidBody& operator=(const RigidBody& other) { return *this; }

	};
};