#pragma once

#include <vector>

#include <physics/interfaces/iRigidBody.h>
#include <physics/interfaces/iConstraint.h>
#include <physics/interfaces/iShape.h>

using namespace std;
using namespace physics;

class Ragdoll
{
private:
	enum BodyPart
	{
		BodyPart_Head,
		BodyPart_Spine,
		BodyPart_Pelvis,
		BodyPart_UpperLeftArm,
		BodyPart_LowerLeftArm,
		BodyPart_UpperRightArm,
		BodyPart_LowerRightArm,
		BodyPart_UpperLeftLeg,
		BodyPart_LowerLeftLeg,
		BodyPart_UpperRightLeg,
		BodyPart_LowerRightLeg,
		NUM_BODY_PARTS
	};

	enum Joint
	{
		Joint_Neck,
		Joint_Pelvis,
		Joint_LeftHip,
		Joint_LeftKnee,
		Joint_RightHip,
		Joint_RightKnee,
		Joint_LeftShoulder,
		Joint_LeftElbow,
		Joint_RightShoulder,
		Joint_RightElbow,
		NUM_JOINTS
	};

	typedef struct BodyPartInfo
	{
		float width;
		float height;
		float mass;
		Vector3 offset;		/* pivot at center/middle */
	} BodyPartInfo;

public:
	Ragdoll(float mass, float scale);
	~Ragdoll();

private:
	void Generate();
	void GenerateBodyParts();
	void ConnectJoints();

	iRigidBody* CreateBodyPart(iShape* capsule, const Vector3& initialOffset, float mass);
	iShape* CreateCapsule(float radius, float height);
	void CreateJoint(Joint joint, BodyPart bodyPartA, BodyPart bodyPartB, Vector3 offsetA, Vector3 offsetB);

private:
	vector<iShape*> m_Shapes;
	vector<iConstraint*> m_Joints;
	vector<iRigidBody*> m_Bodies;
	vector<BodyPartInfo> m_BodyPartInfo;

	float m_Mass;
	float m_Scale;
};