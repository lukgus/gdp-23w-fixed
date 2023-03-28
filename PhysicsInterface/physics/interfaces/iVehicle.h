#pragma once

#include "Math.h"

namespace physics
{
	class iVehicle
	{
	public:
		virtual ~iVehicle() {}

		virtual void AddWheel(const Vector3& connectionPoint, const Vector3& wheelDirection,
			const Vector3& wheelAxle, float suspensionRestLength, float wheelRadius,
			bool isFrontWheel) = 0;

		virtual void ApplyEngineForce(float force, int wheelIndex) = 0;
		virtual void SetSteering(float value, int wheelIndex) = 0;
		virtual void SetBrake(float value, int wheelIndex) = 0;

		virtual void GetChassisTransform(Matrix4x4& matrix) const = 0;
		virtual void GetWheelTransform(Matrix4x4& matrix, int wheelIndex) const = 0;

	protected:
		iVehicle() {}

	private:
		iVehicle(const iVehicle&) { }
		iVehicle& operator=(const iVehicle&) {
			return *this;
		}
	};
}
