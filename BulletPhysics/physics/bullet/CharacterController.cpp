#include "CharacterController.h"
#include "Conversion.h"

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace physics
{
	CharacterController::CharacterController(
		btPairCachingGhostObject* btGhostObject, 
		btConvexShape* shape, 
		btScalar stepHeight, 
		const btVector3& up)
		: m_GhostObject(btGhostObject)
	{
		m_CharacterController = new btKinematicCharacterController(
			m_GhostObject, shape, stepHeight, up);
		m_CharacterController->setUseGhostSweepTest(false);

		m_CharacterController->setMaxPenetrationDepth(0);
	}

	CharacterController::~CharacterController()
	{
	}

	void CharacterController::SetWalkDirection(const Vector3& walkDirection)
	{
		btVector3 btWalkDirection;
		CastBulletVector3(walkDirection, &btWalkDirection);
		m_CharacterController->setWalkDirection(btWalkDirection);
	}

	void CharacterController::SetVelocityForTimeInterval(const Vector3& velocity, float timeInterval)
	{
		btVector3 btVelocity;
		CastBulletVector3(velocity, &btVelocity);
		m_CharacterController->setVelocityForTimeInterval(btVelocity, btScalar(timeInterval));
	}

	void CharacterController::Warp(const Vector3& origin)
	{
		btVector3 btOrigin;
		CastBulletVector3(origin, &btOrigin);
		m_CharacterController->warp(btOrigin);
	}

	bool CharacterController::CanJump() const
	{
		return m_CharacterController->canJump();
	}

	void CharacterController::Jump(const Vector3& dir/*=Vector3(0, 0, 0)*/)
	{
		btVector3 btDir;
		CastBulletVector3(dir, &btDir);
		m_CharacterController->jump(btDir);
	}

	bool CharacterController::OnGround() const
	{
		return m_CharacterController->onGround();
	}

}