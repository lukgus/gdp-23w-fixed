#include "Ragdoll.h"

#include <physics/interfaces/CapsuleShape.h>
#include <physics/interfaces/ConeTwistConstraint.h>

#include "Engine.h"
using namespace gdp;

Ragdoll::Ragdoll(float mass, float scale)
	: m_Mass(mass)
	, m_Scale(scale)
{
	m_Bodies.resize(NUM_BODY_PARTS);
	m_Shapes.resize(NUM_BODY_PARTS);
	m_Joints.resize(NUM_JOINTS);

	Generate();
}

Ragdoll::~Ragdoll()
{

}

void Ragdoll::Generate()
{
	GenerateBodyParts();
	ConnectJoints();

	for (int i = 0; i < NUM_BODY_PARTS; i++)
	{
		GDP_GetPhysicsWorld()->AddBody(m_Bodies[i]);
	}

	for (int i = 0; i < NUM_JOINTS; i++)
	{
		GDP_GetPhysicsWorld()->AddConstraint(m_Joints[i]);
	}
}

void Ragdoll::GenerateBodyParts()
{
	// Create all the shapes
	//							CreateCapsule(Radius, Height)
	m_Shapes[BodyPart_Head] =			CreateCapsule(0.14f * m_Scale, 0.14f * m_Scale);
	m_Shapes[BodyPart_Spine] =			CreateCapsule(0.2f * m_Scale, 0.3f * m_Scale);
	m_Shapes[BodyPart_Pelvis] =			CreateCapsule(0.2f * m_Scale, 0.2f * m_Scale);
	m_Shapes[BodyPart_UpperLeftArm] =	CreateCapsule(0.15f * m_Scale, 0.44f * m_Scale);
	m_Shapes[BodyPart_LowerLeftArm] =	CreateCapsule(0.12f * m_Scale, 0.44f * m_Scale);
	m_Shapes[BodyPart_UpperRightArm] =	CreateCapsule(0.15f * m_Scale, 0.44f * m_Scale);
	m_Shapes[BodyPart_LowerRightArm] =	CreateCapsule(0.12f * m_Scale, 0.44f * m_Scale);
	m_Shapes[BodyPart_UpperLeftLeg] =	CreateCapsule(0.20f * m_Scale, 0.50f * m_Scale);
	m_Shapes[BodyPart_LowerLeftLeg] =	CreateCapsule(0.15f * m_Scale, 0.50f * m_Scale);
	m_Shapes[BodyPart_UpperRightLeg] =	CreateCapsule(0.20f * m_Scale, 0.50f * m_Scale);
	m_Shapes[BodyPart_LowerRightLeg] =	CreateCapsule(0.15f * m_Scale, 0.50f * m_Scale);

	// Create all the bodies
	m_Bodies[BodyPart_Head] =			CreateBodyPart(m_Shapes[BodyPart_Head], Vector3(0, 3, 0), 0.14f * m_Scale);
	m_Bodies[BodyPart_Spine] =			CreateBodyPart(m_Shapes[BodyPart_Spine], Vector3(0, 2, 0), 1.f * m_Scale);
	m_Bodies[BodyPart_Pelvis] =			CreateBodyPart(m_Shapes[BodyPart_Pelvis], Vector3(0, 1, 0), 1.f * m_Scale);
	m_Bodies[BodyPart_UpperLeftArm] =	CreateBodyPart(m_Shapes[BodyPart_UpperLeftArm], Vector3(-1, 2,0), 0.4f * m_Scale);
	m_Bodies[BodyPart_LowerLeftArm] =	CreateBodyPart(m_Shapes[BodyPart_LowerLeftArm], Vector3(-1, 1, 0), 0.2f * m_Scale);
	m_Bodies[BodyPart_UpperRightArm] =	CreateBodyPart(m_Shapes[BodyPart_UpperRightArm], Vector3(1, 2, 0), 0.4f * m_Scale);
	m_Bodies[BodyPart_LowerRightArm] =	CreateBodyPart(m_Shapes[BodyPart_LowerRightArm], Vector3(1, 1, 0), 0.2f * m_Scale);
	m_Bodies[BodyPart_UpperLeftLeg] =	CreateBodyPart(m_Shapes[BodyPart_UpperLeftLeg], Vector3(-1, 1, 0), 0.6f * m_Scale);
	m_Bodies[BodyPart_LowerLeftLeg] =	CreateBodyPart(m_Shapes[BodyPart_LowerLeftLeg], Vector3(-1, 0, 0), 0.3f * m_Scale);
	m_Bodies[BodyPart_UpperRightLeg] =	CreateBodyPart(m_Shapes[BodyPart_UpperRightLeg], Vector3(1, 1, 0), 0.6f * m_Scale);
	m_Bodies[BodyPart_LowerRightLeg] =	CreateBodyPart(m_Shapes[BodyPart_LowerRightLeg], Vector3(1, 0, 0), 0.3f * m_Scale);

}

void Ragdoll::ConnectJoints()
{
	CreateJoint(
		Joint_Neck,
		BodyPart_Head, 
		BodyPart_Spine, 
		Vector3(0.f, -0.24f, 0.f) * m_Scale,
		Vector3(0.f, 0.38f, 0.f) * m_Scale
	);

	CreateJoint(
		Joint_Pelvis,
		BodyPart_Spine, 
		BodyPart_Pelvis,
		Vector3(0.f, -0.35, 0.f) * m_Scale,
		Vector3(0.f, 0.33f, 0.f) * m_Scale
	);

	CreateJoint(
		Joint_LeftHip,
		BodyPart_Pelvis, 
		BodyPart_UpperLeftLeg,
		Vector3(-0.2f, -0.33f, 0.f) * m_Scale,
		Vector3(0.0f, 0.48f, 0.f) * m_Scale
	);

	CreateJoint(
		Joint_LeftKnee,
		BodyPart_UpperLeftLeg, 
		BodyPart_LowerLeftLeg,
		Vector3(0.0f, -0.48f, 0.f) * m_Scale,
		Vector3(0.0f, 0.43f, 0.f) * m_Scale
	);

	CreateJoint(
		Joint_RightHip,
		BodyPart_Pelvis, 
		BodyPart_UpperRightLeg,
		Vector3(0.2f, -0.33f, 0.f) * m_Scale,
		Vector3(0.0f, 0.48f, 0.f) * m_Scale
	);

	CreateJoint(
		Joint_RightKnee,
		BodyPart_UpperRightLeg, 
		BodyPart_LowerRightLeg,
		Vector3(0.0f, -0.48f, 0.f) * m_Scale,
		Vector3(0.0f, 0.43f, 0.f) * m_Scale
	);

	CreateJoint(
		Joint_LeftShoulder,
		BodyPart_Spine,
		BodyPart_UpperLeftArm,
		Vector3(-0.28f, 0.28f, 0) * m_Scale,
		Vector3(0, 0.4, 0) * m_Scale
	);

	CreateJoint(
		Joint_LeftElbow,
		BodyPart_UpperLeftArm, 
		BodyPart_LowerLeftArm,
		Vector3(0, -0.4f, 0) * m_Scale,
		Vector3(0, 0.37, 0) * m_Scale
	);

	CreateJoint(
		Joint_RightShoulder,
		BodyPart_Spine, 
		BodyPart_UpperRightArm,
		Vector3(0.28f, 0.28f, 0) * m_Scale,
		Vector3(0, 0.4, 0) * m_Scale
	);

	CreateJoint(
		Joint_RightElbow,
		BodyPart_UpperRightArm, 
		BodyPart_LowerRightArm,
		Vector3(0, -0.4f, 0) * m_Scale,
		Vector3(0, 0.37, 0) * m_Scale
	);

}

iRigidBody* Ragdoll::CreateBodyPart(iShape* capsule, const Vector3& initialOffset, float mass)
{
	RigidBodyDesc desc;
	desc.mass = mass < 0 ? 0 : mass;
	desc.position = initialOffset;
	//if (mass < 0)
	//{
	//	desc.isStatic = true;
	//	desc.position.y = 4.0f;
	//}
	return GDP_CreateRigidBody(desc, capsule);
}

iShape* Ragdoll::CreateCapsule(float radius, float height)
{
	return new CapsuleShape(radius, height);
}

void Ragdoll::CreateJoint(Joint joint, BodyPart bodyPartA, BodyPart bodyPartB, Vector3 offsetA, Vector3 offsetB)
{
	glm::mat4 transformA = glm::translate(glm::mat4(1.f), offsetA);
	glm::mat4 transformB = glm::translate(glm::mat4(1.f), offsetB);

	m_Joints[joint] = new ConeTwistConstraint(m_Bodies[bodyPartA], m_Bodies[bodyPartB], transformA, transformB);
}