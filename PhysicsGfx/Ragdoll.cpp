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

	// Hacking in the Bone Indexes for this example.
	m_BoneIndexToBodyPartIndex[0] = BodyPart_LowerRightLeg;
	m_BoneIndexToBodyPartIndex[1] = BodyPart_LowerRightLeg;
	m_BoneIndexToBodyPartIndex[2] = BodyPart_LowerLeftLeg;
	m_BoneIndexToBodyPartIndex[3] = BodyPart_UpperRightLeg;
	m_BoneIndexToBodyPartIndex[4] = BodyPart_UpperLeftLeg;
	m_BoneIndexToBodyPartIndex[5] = BodyPart_LowerLeftLeg;
	m_BoneIndexToBodyPartIndex[6] = BodyPart_LowerRightLeg;
	m_BoneIndexToBodyPartIndex[7] = BodyPart_LowerRightLeg;
	m_BoneIndexToBodyPartIndex[8] = BodyPart_LowerLeftLeg;
	m_BoneIndexToBodyPartIndex[9] = BodyPart_Pelvis;
	m_BoneIndexToBodyPartIndex[10] = BodyPart_LowerLeftLeg;
	m_BoneIndexToBodyPartIndex[11] = BodyPart_UpperRightArm;
	m_BoneIndexToBodyPartIndex[12] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[13] = BodyPart_Head;
	m_BoneIndexToBodyPartIndex[14] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[15] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[16] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[17] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[18] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[19] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[20] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[21] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[22] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[23] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[24] = BodyPart_LowerRightArm;
	m_BoneIndexToBodyPartIndex[25] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[26] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[27] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[28] = BodyPart_UpperLeftArm;
	m_BoneIndexToBodyPartIndex[29] = BodyPart_UpperLeftArm;
	m_BoneIndexToBodyPartIndex[30] = BodyPart_Head;
	m_BoneIndexToBodyPartIndex[31] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[32] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[33] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[34] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[35] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[36] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[37] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[38] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[39] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[40] = BodyPart_LowerLeftArm;
	m_BoneIndexToBodyPartIndex[41] = BodyPart_Head;
	m_BoneIndexToBodyPartIndex[42] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[43] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[44] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[45] = BodyPart_Spine;
	m_BoneIndexToBodyPartIndex[46] = BodyPart_Spine;





	//m_BoneIndexToBodyPartIndex[13] = BodyPart_Head;			// RigHead
	//m_BoneIndexToBodyPartIndex[25] = BodyPart_Spine;		// RigSpine1
	//m_BoneIndexToBodyPartIndex[9] = BodyPart_Pelvis;		// RigPelvis
	//m_BoneIndexToBodyPartIndex[28] = BodyPart_UpperLeftArm;	// RigLArmUpperArm
	//m_BoneIndexToBodyPartIndex[31] = BodyPart_LowerLeftArm;	// RigLArmForearm
	//m_BoneIndexToBodyPartIndex[1] = BodyPart_UpperRightArm;	// RigRArmUpper
	//m_BoneIndexToBodyPartIndex[14] = BodyPart_LowerRightArm;// RigRArmForearm
	//m_BoneIndexToBodyPartIndex[4] = BodyPart_UpperLeftLeg;	// RigLLeg1
	//m_BoneIndexToBodyPartIndex[5] = BodyPart_LowerLeftLeg;	// RigLLeg2
	//m_BoneIndexToBodyPartIndex[3] = BodyPart_UpperRightLeg;	// RigRLeg1
	//m_BoneIndexToBodyPartIndex[0] = BodyPart_LowerRightLeg;	// RigRLeg2
}

Ragdoll::~Ragdoll()
{

}

void Ragdoll::GetBodyPartTransform(int boneIndex, glm::mat4& out)
{
	map<int,int>::iterator it = m_BoneIndexToBodyPartIndex.find(boneIndex);
	if (it == m_BoneIndexToBodyPartIndex.end())
		return;

	m_Bodies[it->second]->GetTransform(out);
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
	m_Bodies[BodyPart_Head] =			CreateBodyPart(m_Shapes[BodyPart_Head], Vector3(0, 3, 0), -0.14f * m_Scale);
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