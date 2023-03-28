#pragma once

#include "Math.h"

namespace physics
{
	class iCharacterController
	{
	public:
		virtual ~iCharacterController() {}

		virtual void SetWalkDirection(const Vector3& walkDirection) = 0;
		virtual void SetVelocityForTimeInterval(const Vector3& velocity, float timeInterval) = 0;
		virtual void Warp(const Vector3& origin) = 0;
		virtual bool CanJump() const = 0;
		virtual void Jump(const Vector3& dir = Vector3(0, 0, 0)) = 0;
		virtual bool OnGround() const = 0;
	protected:
		iCharacterController() {}

	private:
		iCharacterController(const iCharacterController&) { }
		iCharacterController& operator=(const iCharacterController&) {
			return *this;
		}
	};
}
