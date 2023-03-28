#include "Vehicle.h"
#include "Conversion.h"

#include <physics/interfaces/iRigidBody.h>

namespace physics
{
	Vehicle::Vehicle(btRigidBody* btChassis, btVehicleRaycaster* vehicleRaycaster)
	{
		// Come back to replace this later
		btRaycastVehicle::btVehicleTuning tuning;
		m_Vehicle = new btRaycastVehicle(tuning, btChassis, vehicleRaycaster);

		btChassis->setActivationState(DISABLE_DEACTIVATION);
	}

	Vehicle::~Vehicle()
	{

	}

	void Vehicle::AddWheel(const Vector3& connectionPoint, const Vector3& wheelDirection,
		const Vector3& wheelAxle, float suspensionRestLength, float wheelRadius,
		bool isFrontWheel)
	{
		btVector3 btConnectionPoint;
		btVector3 btWheelDirection;
		btVector3 btWheelAxle;

		CastBulletVector3(connectionPoint, &btConnectionPoint);
		CastBulletVector3(wheelDirection, &btWheelDirection);
		CastBulletVector3(wheelAxle, &btWheelAxle);

		btRaycastVehicle::btVehicleTuning tuning;

		m_Vehicle->addWheel(btConnectionPoint, btWheelDirection, btWheelAxle,
			suspensionRestLength, wheelRadius, tuning, isFrontWheel);
	}

	void Vehicle::ApplyEngineForce(float force, int wheelIndex)
	{
		m_Vehicle->applyEngineForce(force, wheelIndex);
	}

	void Vehicle::SetSteering(float value, int wheelIndex)
	{
		m_Vehicle->setSteeringValue(value, wheelIndex);
	}

	void Vehicle::SetBrake(float value, int wheelIndex)
	{
		m_Vehicle->setBrake(value, wheelIndex);
	}

	void Vehicle::GetChassisTransform(Matrix4x4& matrix) const
	{
		const btTransform& chassisTransform = m_Vehicle->getChassisWorldTransform();
		chassisTransform.getOpenGLMatrix(&(matrix[0][0]));
	}

	void Vehicle::GetWheelTransform(Matrix4x4& matrix, int wheelIndex) const
	{
		const btTransform& wheelTransform = m_Vehicle->getWheelTransformWS(wheelIndex);
		wheelTransform.getOpenGLMatrix(&(matrix[0][0]));
	}
}