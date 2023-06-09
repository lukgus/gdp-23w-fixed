#include "Character.h"

#include <glm/gtx/easing.hpp>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "vertex_types.h"

#include "GL.h"
#include "CheckGLError.h"

// #define PRINT_DEBUG_INFO

namespace gdp
{
	void Character::CastToGLM(const aiMatrix4x4& in, glm::mat4& out)
	{
		out = glm::transpose(glm::make_mat4(&in.a1));

		//out = glm::mat4(in.a1, in.b1, in.c1, in.d1,
		//	in.a2, in.b2, in.c2, in.d2,
		//	in.a3, in.b3, in.c3, in.d3,
		//	in.a4, in.b4, in.c4, in.d4);
	}

	void Character::CastToGLM(const aiQuaternion& in, glm::quat& out)
	{
		out.w = in.w;
		out.x = in.x;
		out.y = in.y;
		out.z = in.z;
	}

	void Character::CastToGLM(const aiVector3D& in, glm::vec3& out)
	{
		out.x = in.x;
		out.y = in.y;
		out.z = in.z;
	}

	AssimpScene::AssimpScene(const char* filename, unsigned int flags)
	{
		m_Scene = m_Importer.ReadFile(filename, flags);

		Animations = m_Scene->mAnimations;
		Cameras = m_Scene->mCameras;
		Lights = m_Scene->mLights;
		Materials = m_Scene->mMaterials;
		Meshes = m_Scene->mMeshes;
		Textures = m_Scene->mTextures;

		RootNode = m_Scene->mRootNode;
	}

	AssimpScene::~AssimpScene()	{ }

	Character::Character()
		: m_IsPlaying(true)
		, m_AnimationSpeed(1.0)
		, m_CurrentTimeInSeconds(0.0)
		, m_CurrentAnimation(0)
		, m_NumAnimationsLoaded(0)
		, m_UpdateMode(Kinematic)
	{
	}

	Character::~Character()
	{
	}

	void Character::LoadCharacterFromAssimp(const char* filename)
	{
		printf("Character::LoadCharacterFromAssimp: %s\n", filename);

		unsigned int flags = 
			aiProcess_Triangulate 
			| aiProcess_LimitBoneWeights
			| aiProcess_JoinIdenticalVertices;
		m_Scene = new AssimpScene(filename, flags);

		aiMatrix4x4 g = m_Scene->RootNode->mTransformation;
		aiMatrix4x4 inverse = g.Inverse();


		// Node hierarchy for rendering
		m_RootNode = CreateNodeHierarchy(m_Scene->RootNode);
		CastToGLM(inverse, m_GlobalInverseTransform);

		if (m_Scene->HasMeshes())
		{
			// Load all the meshes
			unsigned int numMeshes = m_Scene->NumMeshes();
			printf("-Loading %d meshes!\n", numMeshes);
			for (int i = 0; i < m_Scene->NumMeshes(); i++)
			{
				aiMesh* mesh = m_Scene->Meshes[i];


				std::string meshName(mesh->mName.C_Str());
				printf("\n---------------------\n");
				printf("Mesh: %s\n", meshName.c_str());
				printf("  Vertices: %d\n", mesh->mNumVertices);
				printf("  Faces: %d\n", mesh->mNumFaces);
				printf("  Bones: %d\n", mesh->mNumBones);
				printf("  Vertex Start %d\n", mesh->mFaces[0].mIndices[0]);

			}
			printf("\n\n");

			{
				aiMesh* mesh = m_Scene->Meshes[0];
				if (!LoadAssimpMesh(mesh))
				{
					printf("Failed to load mesh! \n");
				}
			}
		}

		printf("Done loading FBX file!\n");
	}

	void Character::LoadAnimationFromAssimp(const char* filename)
	{
		const aiScene* scene = m_AnimationImporter.ReadFile(filename, 0);

		unsigned int numAnimations = scene->mNumAnimations;
		printf("-Loading %d animations!\n", numAnimations);
		aiAnimation* animation = scene->mAnimations[0];
		LoadAssimpAnimation(animation);
	}

	Model* Character::GetModel(int index)
	{
		assert(index >= 0);
		assert(index < m_Models.size());
		return m_Models[index];
	}

	void Character::LoadAssimpBones(const aiMesh* assimpMesh)
	{
		// Record Vertex Weights
		int totalWeights = 0;
		m_BoneVertexData.resize(assimpMesh->mNumVertices);
		int boneCount = 0;

		int numBones = assimpMesh->mNumBones;
		for (int i = 0; i < numBones; i++)
		{
			aiBone* bone = assimpMesh->mBones[i];

			int boneIdx = 0;
			std::string boneName(bone->mName.data);

			printf("%d: %s\n", i, boneName.c_str());

			std::map<std::string, int>::iterator it = m_BoneNameToIdMap.find(boneName);
			if (it == m_BoneNameToIdMap.end())
			{
				boneIdx = boneCount;
				boneCount++;
				BoneInfo bi;
				bi.name = boneName;
				m_BoneInfoVec.push_back(bi);

				CastToGLM(bone->mOffsetMatrix, m_BoneInfoVec[boneIdx].boneOffset);
				m_BoneNameToIdMap[boneName] = boneIdx;
			}
			else
			{
				boneIdx = it->second;
			}

			for (int weightIdx = 0; weightIdx < bone->mNumWeights; weightIdx++)
			{
				float weight = bone->mWeights[weightIdx].mWeight;
				int vertexId = bone->mWeights[weightIdx].mVertexId;
				m_BoneVertexData[vertexId].AddBoneInfo(boneIdx, weight);
			}
		}
	}

	void Character::UpdateTransforms(std::vector<glm::mat4>& transforms, std::vector<glm::mat4>& globals, float dt)
	{
		if (m_IsPlaying && m_AnimationSpeed != 0.0f)
		{
			m_CurrentTimeInSeconds += dt * m_AnimationSpeed;
			m_CurrentTimeInSeconds = fmod(
				m_CurrentTimeInSeconds, m_DurationInSeconds[m_CurrentAnimation]);

			int keyFrameTime = (int)((m_CurrentTimeInSeconds / m_DurationInSeconds[m_CurrentAnimation]) * 
				m_DurationInTicks[m_CurrentAnimation]);

			glm::mat4 identity(1.f);

			if (m_TransitionTime > 0.0f)
			{
				m_TransitionTime -= dt;
				if (m_TransitionTime < 0.0f)
				{
					m_TransitionTime = 0.0f;
				}
			}

			printf("--------------------\n");
			printf("Time: %.4f %d/%d\n", m_CurrentTimeInSeconds, keyFrameTime, (int)m_DurationInTicks);

			if (m_UpdateMode == Kinematic)
			{
				UpdateBoneHierarchy(m_RootNode, identity, keyFrameTime);
				transforms.resize(m_BoneInfoVec.size());
				globals.resize(m_BoneInfoVec.size());
				int numTransforms = m_BoneInfoVec.size();
				for (int i = 0; i < numTransforms; i++)
				{
					transforms[i] = m_BoneInfoVec[i].finalTransformation;
					globals[i] = m_BoneInfoVec[i].globalTransformation;
				}
			}
			else
			{
				// Should be able to do this once during load:
				// Calculate Transform data from NodeHierarchy 

				// Update from Ragdoll info
				int numTransforms = m_BoneInfoVec.size();
				glm::mat4 bodyPartTransform;
				for (int i = 0; i < numTransforms; i++)
				{
					m_Ragdoll->GetBodyPartTransform(i, bodyPartTransform);
					transforms[i] = bodyPartTransform; // * NodeTransform
				}
			}
		}
	}

	AnimNode* Character::CreateNodeHierarchy(aiNode* assimpNode, int depth)
	{
		AnimNode* newNode = new AnimNode();
		newNode->name = std::string(assimpNode->mName.data);
		CastToGLM(assimpNode->mTransformation, newNode->transformation);

		for (int i = 0; i < depth; i++)
			printf(" ");
		printf("%s (%d)\n", newNode->name.c_str(), assimpNode->mNumChildren);


		//glm::vec3 globalScale, globalTranslation, ignore3;
		//glm::vec4 ignore4;
	    //glm::quat globalOrientation;

		//bool success = glm::decompose(newNode->transformation, globalScale, globalOrientation, globalTranslation, ignore3, ignore4);

		//printf("  NodeTransformation:\n");
		//printf("    Position: (%.4f, %.4f, %.4f)\n", globalTranslation.x, globalTranslation.y, globalTranslation.z);
		//printf("    Scale: (%.4f, %.4f, %.4f)\n", globalScale.x, globalScale.y, globalScale.z);
		//printf("    Rotation: (%.4f, %.4f, %.4f, %.4f)\n", globalOrientation.x, globalOrientation.y, globalOrientation.z, globalOrientation.w);
		//printf("\n");

		for (int i = 0; i < assimpNode->mNumChildren; i++)
		{
			AnimNode* childNode = CreateNodeHierarchy(assimpNode->mChildren[i], depth + 1);
			newNode->AddChild(childNode);
		}

		return newNode;
	}

	bool Character::LoadAssimpMesh(const aiMesh* assimpMesh)
	{
		if (assimpMesh == nullptr)
			return false;

		//Bones
		LoadAssimpBones(assimpMesh);

		std::string meshName(assimpMesh->mName.data);
		printf("%s\n", meshName.c_str());

		// Data
		unsigned int numFaces = assimpMesh->mNumFaces;
		unsigned int numIndicesInIndexArray = numFaces * 3;
		sVertex_p4t4n4b4w4* pTempVertArray = new sVertex_p4t4n4b4w4[numIndicesInIndexArray * 2];
		GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];

		int vertArrayIndex = 0;
		for (int faceIndex = 0; faceIndex < numFaces; faceIndex++)
		{
			const aiFace& face = assimpMesh->mFaces[faceIndex];

			unsigned int numIndices = face.mNumIndices;
			for (int indicesIndex = 0; indicesIndex < numIndices; indicesIndex++)
			{
				unsigned int vertexIndex = face.mIndices[indicesIndex];

				const aiVector3D& vertex = assimpMesh->mVertices[vertexIndex];

				pTempVertArray[vertArrayIndex].Pos.x = vertex.x;
				pTempVertArray[vertArrayIndex].Pos.y = vertex.y;
				pTempVertArray[vertArrayIndex].Pos.z = vertex.z;
				pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

				if (assimpMesh->HasNormals())
				{
					const aiVector3D& normal = assimpMesh->mNormals[vertexIndex];
					pTempVertArray[vertArrayIndex].Normal.x = normal.x;
					pTempVertArray[vertArrayIndex].Normal.y = normal.y;
					pTempVertArray[vertArrayIndex].Normal.z = normal.z;
					pTempVertArray[vertArrayIndex].Normal.w = 0.f;
				}
				else
				{
					pTempVertArray[vertArrayIndex].Normal.x = 1.f;
					pTempVertArray[vertArrayIndex].Normal.y = 0.f;
					pTempVertArray[vertArrayIndex].Normal.z = 0.f;
					pTempVertArray[vertArrayIndex].Normal.w = 0.f;
				}

				//if (assimpMesh->HasTextureCoords(0))
				//{
					//const aiVector3D& uvCoord = assimpMesh->mTextureCoords[0][vertexIndex];
					pTempVertArray[vertArrayIndex].TexUVx2.x = 0;
					pTempVertArray[vertArrayIndex].TexUVx2.y = 0;
					pTempVertArray[vertArrayIndex].TexUVx2.z = 0;
					pTempVertArray[vertArrayIndex].TexUVx2.w = 0;
				//}

				// Use a BoneInformation Map to get bone info and store the values here

				// Copy instead of reference, try..
				BoneVertexData bvd = m_BoneVertexData[vertexIndex];
				pTempVertArray[vertArrayIndex].BoneIds.x = bvd.ids[0];
				pTempVertArray[vertArrayIndex].BoneIds.y = bvd.ids[1];
				pTempVertArray[vertArrayIndex].BoneIds.z = bvd.ids[2];
				pTempVertArray[vertArrayIndex].BoneIds.w = bvd.ids[3];

				pTempVertArray[vertArrayIndex].BoneWeights.x = bvd.weights[0];
				pTempVertArray[vertArrayIndex].BoneWeights.y = bvd.weights[1];
				pTempVertArray[vertArrayIndex].BoneWeights.z = bvd.weights[2];
				pTempVertArray[vertArrayIndex].BoneWeights.w = bvd.weights[3];

				pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

				vertArrayIndex++;
			}
		}

		Model* model = new Model();
		//int testNumTriangles = triangles.size();
		model->NumTriangles = numFaces;
		glGenVertexArrays(1, &model->Vbo);
		glBindVertexArray(model->Vbo);
		CheckGLError();

		glGenBuffers(1, &model->VertexBufferId);
		glGenBuffers(1, &model->IndexBufferId);
		CheckGLError();

		glBindBuffer(GL_ARRAY_BUFFER, model->VertexBufferId);
		CheckGLError();

		unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4t4n4b4w4);
		glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);
		CheckGLError();

		unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4b4w4);
		unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4b4w4, Pos);
		unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4b4w4, TexUVx2);
		unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4b4w4, Normal);
		unsigned int byteOffsetToBoneWeights = offsetof(sVertex_p4t4n4b4w4, BoneWeights);
		unsigned int byteOffsetToBoneIds = offsetof(sVertex_p4t4n4b4w4, BoneIds);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		CheckGLError();

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneWeights);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneIds);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IndexBufferId);

		unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

		glBindVertexArray(0);

		delete[] pTempVertArray;
		delete[] pIndexArrayLocal;

		//printf("  - Done Generating Buffer data");

		//printf("  - Finished Loading model \"%s\" with %d vertices, %d triangles, id is: %d\n", mesh->mName.C_Str(), mesh->mNumVertices, model->NumTriangles, model->Vbo);
		m_MeshToIdMap[meshName] = m_Models.size();
		m_Models.push_back(model);

		return true;
	}

	void Character::LoadAssimpAnimation(const aiAnimation* animation)
	{
		if (animation == nullptr)
			return;

		if (m_NumAnimationsLoaded >= 10)
			return;

		unsigned int numChannels = animation->mNumChannels;

		// Hacking in which animation channel to add
		std::vector<AnimationData*>& channels = m_Channels[m_NumAnimationsLoaded];

		m_DurationInTicks[m_NumAnimationsLoaded] = animation->mDuration;
		m_TicksPerSecond[m_NumAnimationsLoaded] = animation->mTicksPerSecond;
		m_DurationInSeconds[m_NumAnimationsLoaded] = 
			m_DurationInTicks[m_NumAnimationsLoaded] / m_TicksPerSecond[m_NumAnimationsLoaded];

		m_NumAnimationsLoaded++;

		channels.resize(numChannels);
		for (int i = 0; i < numChannels; i++)
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			std::string name(nodeAnim->mNodeName.data);
			printf("%s\n", name.c_str());

			m_BoneNameToAnimationMap[name] = i;

			unsigned int numPositionKeys = nodeAnim->mNumPositionKeys;
			unsigned int numRotationKeys = nodeAnim->mNumRotationKeys;
			unsigned int numScalingKeys = nodeAnim->mNumScalingKeys;

			AnimationData* animData = new AnimationData();
			channels[i] = animData;
			animData->Name = name;

			animData->PositionKeyFrames.resize(numPositionKeys);
			animData->RotationKeyFrames.resize(numRotationKeys);
			animData->ScaleKeyFrames.resize(numScalingKeys);

			for (int keyIdx = 0; keyIdx < numPositionKeys; keyIdx++)
			{
				const aiVectorKey& posKey = nodeAnim->mPositionKeys[keyIdx];
				animData->PositionKeyFrames[keyIdx].time = posKey.mTime;
				animData->PositionKeyFrames[keyIdx].value.x = posKey.mValue.x;
				animData->PositionKeyFrames[keyIdx].value.y = posKey.mValue.y;
				animData->PositionKeyFrames[keyIdx].value.z = posKey.mValue.z;
			}

			for (int keyIdx = 0; keyIdx < numRotationKeys; keyIdx++)
			{
				const aiQuatKey& rotKey = nodeAnim->mRotationKeys[keyIdx];
				animData->RotationKeyFrames[keyIdx].time = rotKey.mTime;
				animData->RotationKeyFrames[keyIdx].value.x = rotKey.mValue.x;
				animData->RotationKeyFrames[keyIdx].value.y = rotKey.mValue.y;
				animData->RotationKeyFrames[keyIdx].value.z = rotKey.mValue.z;
				animData->RotationKeyFrames[keyIdx].value.w = rotKey.mValue.w;
			}

			for (int keyIdx = 0; keyIdx < numScalingKeys; keyIdx++)
			{
				const aiVectorKey& scaleKey = nodeAnim->mScalingKeys[keyIdx];
				animData->ScaleKeyFrames[keyIdx].time = scaleKey.mTime;
				animData->ScaleKeyFrames[keyIdx].value.x = scaleKey.mValue.x;
				animData->ScaleKeyFrames[keyIdx].value.y = scaleKey.mValue.y;
				animData->ScaleKeyFrames[keyIdx].value.z = scaleKey.mValue.z;
			}
		}
	}
//
//	BoneNode* CreateNodeHierarchy(aiNode* node, int depth = 0)
//	{
//		BoneNode* newNode = new BoneNode();
//		newNode->name = node->mName.C_Str();
//		CastToGLM(node->mTransformation, newNode->transformation);
//
//#ifdef PRINT_DEBUG_INFO
//		for (int i = 0; i < depth; i++)
//			printf(" ");
//		printf("%s (%d)\n", newNode->name.c_str(), node->mNumChildren);
//#endif
//
//
//		glm::vec3 globalScale, globalTranslation, ignore3;
//		glm::vec4 ignore4;
//		glm::quat globalOrientation;
//
//		bool success = glm::decompose(newNode->transformation, globalScale, globalOrientation, globalTranslation, ignore3, ignore4);
//
//
//#ifdef PRINT_DEBUG_INFO
//		printf("  NodeTransformation:\n");
//		printf("    Position: (%.4f, %.4f, %.4f)\n", globalTranslation.x, globalTranslation.y, globalTranslation.z);
//		printf("    Scale: (%.4f, %.4f, %.4f)\n", globalScale.x, globalScale.y, globalScale.z);
//		printf("    Rotation: (%.4f, %.4f, %.4f, %.4f)\n", globalOrientation.x, globalOrientation.y, globalOrientation.z, globalOrientation.w);
//		printf("\n");
//#endif
//
//		//const aiMatrix4x4& m = node->mTransformation;
//		//printf("%.2f %.2f %.2f %.2f, %.2f %.2f %.2f %.2f, %.2f %.2f %.2f %.2f, %.2f %.2f %.2f %.2f\n",
//		//    m.a1, m.b1, m.c1, m.d1, m.a2, m.b2, m.c2, m.d2, m.a3, m.b3, m.c3, m.d3, m.a4, m.b4, m.c4, m.d4);
//
//		std::vector<aiNode*> children;
//
//		for (int i = 0; i < node->mNumChildren; i++)
//		{
//			children.push_back(node->mChildren[i]);
//
//			BoneNode* childNode = CreateNodeHierarchy(node->mChildren[i], depth + 1);
//			newNode->AddChild(childNode);
//		}
//
//		return newNode;
//	}

	AnimationData* Character::FindAnimationData(const std::string& nodeName, int animation)
	{
		std::map<std::string, int>::iterator animIt = m_BoneNameToAnimationMap.find(nodeName);

		//printf("%s\n", nodeName.c_str());
		for (int i = 0; i < m_Channels[animation].size(); i++)
		{
			if (nodeName == m_Channels[animation][i]->Name)
			{
				return m_Channels[animation][i];
			}
		}

		if (animIt != m_BoneNameToAnimationMap.end())
		{
			int breakhereplz = 0;
		}

		return nullptr;
	}


	//aiNodeAnim* Character::FindAssimpNodeAnim(const std::string& nodeName)
	//{
	//	aiAnimation* animation = m_Scene->Animations[0];
	//	int numChannels = animation->mNumChannels;

	//	for (int i = 0; i < numChannels; i++)
	//	{
	//		std::string name(animation->mChannels[i]->mNodeName.data);
	//		if (nodeName == name)
	//			return animation->mChannels[i];
	//	}
	//	return nullptr;
	//}

	//aiNode* Character::FindAssimpNode(aiNode* node, const std::string& name)
	//{
	//	std::string assimpName(node->mName.data);
	//	if (assimpName == name)
	//	{
	//		return node;
	//	}

	//	unsigned int numChildren = node->mNumChildren;
	//	for (unsigned int i = 0; i < numChildren; i++)
	//	{
	//		aiNode* child = FindAssimpNode(node->mChildren[i], name);
	//		if (child != nullptr)
	//			return child;
	//	}

	//	return nullptr;
	//}

	//aiNode* GetNodeFromName(aiNode* node, const std::string& name)
	//{
	//	if (std::string(node->mName.C_Str()) == name)
	//		return node;

	//	for (int i = 0; i < node->mNumChildren; i++)
	//	{
	//		return GetNodeFromName(node->mChildren[i], name);
	//	}

	//	return nullptr;
	//}

	void Character::UpdateBoneHierarchy(AnimNode* node, const glm::mat4& parentTransformationMatrix, float keyFrameTime)
	{
		if (node == nullptr)
			return;

		std::string nodeName(node->name);

		glm::mat4 transformationMatrix = node->transformation;

		AnimationData* animNode = FindAnimationData(nodeName, m_CurrentAnimation);
		AnimationData* animNode2 = FindAnimationData(nodeName, m_PreviousAnimation);
		if (animNode != nullptr)
		{
			// Calculate the position of this node.
			glm::vec3 position = GetAnimationPosition(*animNode, keyFrameTime);
			glm::vec3 scale = GetAnimationScale(*animNode, keyFrameTime);
			glm::quat rotation = GetAnimationRotation(*animNode, keyFrameTime);

			if (animNode2 != nullptr && m_TransitionTime > 0.0f)
			{
				glm::vec3 position2 = GetAnimationPosition(*animNode2, keyFrameTime);
				glm::vec3 scale2 = GetAnimationScale(*animNode2, keyFrameTime);
				glm::quat rotation2 = GetAnimationRotation(*animNode2, keyFrameTime);

				float currRatio = 1.0f - m_TransitionTime;
				float prevRatio = m_TransitionTime;

				position = position * currRatio + position2 * prevRatio;
				scale = scale * currRatio + scale2 * prevRatio;
				rotation = glm::slerp(rotation2, rotation, currRatio);
			}

			// Calculate our transformation matrix
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
			glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

			transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;

#ifdef PRINT_DEBUG_INFO
			printf("  Local Transformation:\n");
			printf("    Position: (%.4f, %.4f, %.4f)\n", position.x, position.y, position.z);
			printf("    Scale: (%.4f, %.4f, %.4f)\n", scale.x, scale.y, scale.z);
			printf("    Rotation: (%.4f, %.4f, %.4f, %.4f)\n", rotation.x, rotation.y, rotation.z, rotation.w);
#endif
		}
		glm::mat4 globalTransformation = parentTransformationMatrix * transformationMatrix;

		if (m_BoneNameToIdMap.find(nodeName) != m_BoneNameToIdMap.end())
		{
			//printf("%d : %s\n", boneIt->second, nodeName.c_str());
			int boneIdx = m_BoneNameToIdMap[nodeName];
			m_BoneInfoVec[boneIdx].finalTransformation = m_GlobalInverseTransform * globalTransformation * m_BoneInfoVec[boneIdx].boneOffset;
			m_BoneInfoVec[boneIdx].globalTransformation = globalTransformation;
		}

#ifdef PRINT_DEBUG_INFO
		glm::vec3 globalScale, globalTranslation, ignore3;
		glm::vec4 ignore4;
		glm::quat globalOrientation;

		bool success = glm::decompose(globalTransformation, globalScale, globalOrientation, globalTranslation, ignore3, ignore4);

		printf("  Global Transformation:\n");
		printf("    Position: (%.4f, %.4f, %.4f)\n", globalTranslation.x, globalTranslation.y, globalTranslation.z);
		printf("    Scale: (%.4f, %.4f, %.4f)\n", globalScale.x, globalScale.y, globalScale.z);
		printf("    Rotation: (%.4f, %.4f, %.4f, %.4f)\n", globalOrientation.x, globalOrientation.y, globalOrientation.z, globalOrientation.w);
		printf("\n");
#endif

		for (int i = 0; i < node->children.size(); i++)
		{
			UpdateBoneHierarchy(node->children[i], globalTransformation, keyFrameTime);
		}
	}
//
//	void Character::UpdateBoneHierarchy(aiNode* node, const glm::mat4& parentTransformationMatrix, float keyFrameTime)
//	{
//		if (node == nullptr)
//			return;
//
//		std::string nodeName(node->mName.data);
//		glm::mat4 transformationMatrix;
//		CastToGLM(node->mTransformation, transformationMatrix);
//		aiNodeAnim* assimpNodeAnim = FindAssimpNodeAnim(nodeName);
//		//printf(" %s\n", nodeName.c_str());
//
//		if (assimpNodeAnim)
//		{
//			// Calculate the position of this node.
//			glm::vec3 position;
//			glm::vec3 scale;
//			glm::quat rotation;
//			position = GetAssimpAnimationPosition(assimpNodeAnim, keyFrameTime);
//			scale = GetAssimpAnimationScale(assimpNodeAnim, keyFrameTime);
//			rotation = GetAssimpAnimationRotation(assimpNodeAnim, keyFrameTime);
//
//			// Calculate our transformation matrix
//			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
//			glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
//			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
//
//			transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
//
//#ifdef PRINT_DEBUG_INFO
//			printf("  Local Transformation:\n");
//			printf("    Position: (%.4f, %.4f, %.4f)\n", position.x, position.y, position.z);
//			printf("    Scale: (%.4f, %.4f, %.4f)\n", scale.x, scale.y, scale.z);
//			printf("    Rotation: (%.4f, %.4f, %.4f, %.4f)\n", rotation.x, rotation.y, rotation.z, rotation.w);
//#endif
//		}
//
//		glm::mat4 globalTransformation = parentTransformationMatrix * transformationMatrix;
//
//		// Store the transformation matrix in the Animation component info.
//		//Check if this is a bone, and get the bone offset
//
//		aiMesh* mesh = m_Scene->Meshes[0];
//		int numBones = mesh->mNumBones;
//		aiBone* bone = nullptr;
//		for (int i = 0; i < numBones; i++)
//		{
//			std::string boneName(mesh->mBones[i]->mName.data);
//			if (boneName == nodeName)
//			{
//				bone = mesh->mBones[i];
//				break;
//			}
//		}
//
//
//		if (m_BoneNameToIdMap.find(nodeName) != m_BoneNameToIdMap.end())
//		{
//			//printf("%d : %s\n", boneIt->second, nodeName.c_str());
//			glm::mat4 offset(1.0f);
//			if (bone != nullptr)
//				CastToGLM(bone->mOffsetMatrix, offset);
//			int boneIdx = m_BoneNameToIdMap[nodeName];
//			m_BoneInfoVec[boneIdx].finalTransformation = m_GlobalInverseTransform * globalTransformation * m_BoneInfoVec[boneIdx].boneOffset;
//			m_BoneInfoVec[boneIdx].globalTransformation = globalTransformation;
//		}
//
//		glm::vec3 globalScale, globalTranslation, ignore3;
//		glm::vec4 ignore4;
//		glm::quat globalOrientation;
//
//		bool success = glm::decompose(globalTransformation, globalScale, globalOrientation, globalTranslation, ignore3, ignore4);
//
//#ifdef PRINT_DEBUG_INFO
//		printf("  Global Transformation:\n");
//		printf("    Position: (%.4f, %.4f, %.4f)\n", globalTranslation.x, globalTranslation.y, globalTranslation.z);
//		printf("    Scale: (%.4f, %.4f, %.4f)\n", globalScale.x, globalScale.y, globalScale.z);
//		printf("    Rotation: (%.4f, %.4f, %.4f, %.4f)\n", globalOrientation.x, globalOrientation.y, globalOrientation.z, globalOrientation.w);
//		printf("\n");
//#endif
//
//		for (int i = 0; i < node->mNumChildren; i++)
//		{
//			UpdateBoneHierarchy(node->mChildren[i], globalTransformation, keyFrameTime);
//		}
//	}

	int Character::FindPositionKeyFrameIndex(const AnimationData& animation, float keyFrameTime)
	{
		for (int i = 0; i < animation.PositionKeyFrames.size() - 1; i++)
		{
			if (animation.PositionKeyFrames[i + 1].time > keyFrameTime)
				return i;
		}

		return 0;
	}

	int Character::FindScaleKeyFrameIndex(const AnimationData& animation, float keyFrameTime)
	{
		for (int i = 0; i < animation.ScaleKeyFrames.size() - 1; i++)
		{
			if (animation.ScaleKeyFrames[i + 1].time > keyFrameTime)
				return i;
		}

		return 0;
	}

	int Character::FindRotationKeyFrameIndex(const AnimationData& animation, float keyFrameTime)
	{
		for (int i = 0; i < animation.RotationKeyFrames.size() - 1; i++)
		{
			if (animation.RotationKeyFrames[i + 1].time > keyFrameTime)
				return i;
		}

		return 0;
	}

	glm::vec3 Character::GetAnimationPosition(const AnimationData& animation, float keyFrameTime)
	{
		// Assert animation.PositionKeyFrames.size() > 0

		if (animation.PositionKeyFrames.size() == 1)
			return animation.PositionKeyFrames[0].value;

		int positionKeyFrameIndex = FindPositionKeyFrameIndex(animation, keyFrameTime);
		int nextPositionKeyFrameIndex = positionKeyFrameIndex + 1;
		PositionKeyFrame positionKeyFrame = animation.PositionKeyFrames[positionKeyFrameIndex];
		PositionKeyFrame nextPositionKeyFrame = animation.PositionKeyFrames[nextPositionKeyFrameIndex];
		float difference = nextPositionKeyFrame.time - positionKeyFrame.time;
		float ratio = (keyFrameTime - positionKeyFrame.time) / difference;

		if (ratio < 0.0f) ratio = 0.0f;
		if (ratio > 1.0f) ratio = 1.0f;

		//glm::vec3 result = glm::mix(positionKeyFrame.value, nextPositionKeyFrame.value, ratio);
		glm::vec3 result = (nextPositionKeyFrame.value - positionKeyFrame.value) * ratio + positionKeyFrame.value;

#ifdef PRINT_DEBUG_INFO
		//printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
		//	positionKeyFrame.value.x, positionKeyFrame.value.y,
		//	nextPositionKeyFrame.value.x, nextPositionKeyFrame.value.y,
		//	result.x, result.y);
#endif

		return result;
	}

	glm::vec3 Character::GetAnimationScale(const AnimationData& animation, float keyFrameTime)
	{
		// Assert animation.ScaleKeyFrames.size() > 0

		if (animation.ScaleKeyFrames.size() == 1)
			return animation.ScaleKeyFrames[0].value;

		int scaleKeyFrameIndex = FindScaleKeyFrameIndex(animation, keyFrameTime);
		int nextScaleKeyFrameIndex = scaleKeyFrameIndex + 1;
		ScaleKeyFrame scaleKeyFrame = animation.ScaleKeyFrames[scaleKeyFrameIndex];
		ScaleKeyFrame nextScaleKeyFrame = animation.ScaleKeyFrames[nextScaleKeyFrameIndex];
		float difference = nextScaleKeyFrame.time - scaleKeyFrame.time;
		float ratio = (keyFrameTime - scaleKeyFrame.time) / difference;

		if (ratio < 0.0f) ratio = 0.0f;
		if (ratio > 1.0f) ratio = 1.0f;

		//glm::vec3 result = glm::mix(scaleKeyFrame.value, nextScaleKeyFrame.value, ratio);
		glm::vec3 result = (nextScaleKeyFrame.value - scaleKeyFrame.value) * ratio + scaleKeyFrame.value;

#ifdef PRINT_DEBUG_INFO
		//printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
			//scaleKeyFrame.value.x, scaleKeyFrame.value.y,
			//nextScaleKeyFrame.value.x, nextScaleKeyFrame.value.y,
			//result.x, result.y);
#endif

		return result;
	}

	glm::quat Character::GetAnimationRotation(const AnimationData& animation, float keyFrameTime)
	{
		if (animation.RotationKeyFrames.size() == 1)
			return animation.RotationKeyFrames[0].value;

		int rotationKeyFrameIndex = FindRotationKeyFrameIndex(animation, keyFrameTime);
		int nextRotationKeyFrameIndex = rotationKeyFrameIndex + 1;
		RotationKeyFrame rotationKeyFrame = animation.RotationKeyFrames[rotationKeyFrameIndex];
		RotationKeyFrame nextRotationKeyFrame = animation.RotationKeyFrames[nextRotationKeyFrameIndex];
		float difference = nextRotationKeyFrame.time - rotationKeyFrame.time;
		float ratio = (keyFrameTime - rotationKeyFrame.time) / difference;

		if (ratio < 0.0f) ratio = 0.0f;
		if (ratio > 1.0f) ratio = 1.0f;

		glm::quat result;
		result = glm::slerp(rotationKeyFrame.value, nextRotationKeyFrame.value, ratio);
		result = glm::normalize(result);

#ifdef PRINT_DEBUG_INFO
		//printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
			//scaleKeyFrame.value.x, scaleKeyFrame.value.y,
			//nextScaleKeyFrame.value.x, nextScaleKeyFrame.value.y,
			//result.x, result.y);
#endif

		return result;
	}
//
//	int Character::FindAssimpPositionKeyFrameIndex(const aiNodeAnim* animation, float keyFrameTime)
//	{
//		for (int i = 0; i < animation->mNumPositionKeys - 1; i++)
//		{
//			if (animation->mPositionKeys[i + 1].mTime > keyFrameTime)
//				return i;
//		}
//
//		return 0;
//	}
//
//	int Character::FindAssimpScaleKeyFrameIndex(const aiNodeAnim* animation, float keyFrameTime)
//	{
//		for (int i = 0; i < animation->mNumScalingKeys - 1; i++)
//		{
//			if (animation->mScalingKeys[i + 1].mTime > keyFrameTime)
//				return i;
//		}
//
//		return 0;
//	}
//
//	int Character::FindAssimpRotationKeyFrameIndex(const aiNodeAnim* animation, float keyFrameTime)
//	{
//		for (int i = 0; i < animation->mNumRotationKeys - 1; i++)
//		{
//			if (animation->mRotationKeys[i + 1].mTime > keyFrameTime)
//				return i;
//		}
//
//		return 0;
//	}
//
//	glm::vec3 Character::GetAssimpAnimationPosition(const aiNodeAnim* animation, float keyFrameTime)
//	{
//		// Assert animation.PositionKeyFrames.size() > 0
//
//		if (animation->mNumPositionKeys == 1)
//		{
//			glm::vec3 glmPosition;
//			CastToGLM(animation->mPositionKeys[0].mValue, glmPosition);
//			return glmPosition;
//		}
//
//		int positionKeyFrameIndex = FindAssimpPositionKeyFrameIndex(animation, keyFrameTime);
//		int nextPositionKeyFrameIndex = positionKeyFrameIndex + 1;
//		aiVectorKey positionKeyFrame = animation->mPositionKeys[positionKeyFrameIndex];
//		aiVectorKey nextPositionKeyFrame = animation->mPositionKeys[nextPositionKeyFrameIndex];
//		float difference = nextPositionKeyFrame.mTime - positionKeyFrame.mTime;
//		float ratio = (keyFrameTime - positionKeyFrame.mTime) / difference;
//
//		if (ratio < 0.0f) ratio = 0.0f;
//		if (ratio > 1.0f) ratio = 1.0f;
//
//		glm::vec3 start, end;
//		CastToGLM(positionKeyFrame.mValue, start);
//		CastToGLM(nextPositionKeyFrame.mValue, end);
//
//		//glm::vec3 result = glm::mix(positionKeyFrame.value, nextPositionKeyFrame.value, ratio);
//		glm::vec3 result = (end - start) * ratio + start;
//
//#ifdef PRINT_DEBUG_INFO
//		//printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
//		//	positionKeyFrame.value.x, positionKeyFrame.value.y,
//		//	nextPositionKeyFrame.value.x, nextPositionKeyFrame.value.y,
//		//	result.x, result.y);
//#endif
//
//		return result;
//	}
//
//	glm::vec3 Character::GetAssimpAnimationScale(const aiNodeAnim* animation, float keyFrameTime)
//	{
//		// Assert animation.ScaleKeyFrames.size() > 0
//
//		if (animation->mNumScalingKeys == 1)
//		{
//			glm::vec3 glmScale;
//			CastToGLM(animation->mScalingKeys[0].mValue, glmScale);
//			return glmScale;
//		}
//
//		int scaleKeyFrameIndex = FindAssimpScaleKeyFrameIndex(animation, keyFrameTime);
//		int nextScaleKeyFrameIndex = scaleKeyFrameIndex + 1;
//		aiVectorKey scaleKeyFrame = animation->mScalingKeys[scaleKeyFrameIndex];
//		aiVectorKey nextScaleKeyFrame = animation->mScalingKeys[nextScaleKeyFrameIndex];
//		float difference = nextScaleKeyFrame.mTime - scaleKeyFrame.mTime;
//		float ratio = (keyFrameTime - scaleKeyFrame.mTime) / difference;
//
//		if (ratio < 0.0f) ratio = 0.0f;
//		if (ratio > 1.0f) ratio = 1.0f;
//
//		glm::vec3 start, end;
//		CastToGLM(scaleKeyFrame.mValue, start);
//		CastToGLM(nextScaleKeyFrame.mValue, end);
//
//		//glm::vec3 result = glm::mix(positionKeyFrame.value, nextPositionKeyFrame.value, ratio);
//		glm::vec3 result = (end - start) * ratio + start;
//#ifdef PRINT_DEBUG_INFO
//		//printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
//			//scaleKeyFrame.value.x, scaleKeyFrame.value.y,
//			//nextScaleKeyFrame.value.x, nextScaleKeyFrame.value.y,
//			//result.x, result.y);
//#endif
//
//		return result;
//	}
//
//	glm::quat Character::GetAssimpAnimationRotation(const aiNodeAnim* animation, float keyFrameTime)
//	{
//		if (animation->mNumRotationKeys == 1)
//		{
//			glm::quat glmQuat;
//			CastToGLM(animation->mRotationKeys[0].mValue, glmQuat);
//			return glmQuat;
//		}
//
//		int rotationKeyFrameIndex = FindAssimpRotationKeyFrameIndex(animation, keyFrameTime);
//		int nextRotationKeyFrameIndex = rotationKeyFrameIndex + 1;
//		aiQuatKey rotationKeyFrame = animation->mRotationKeys[rotationKeyFrameIndex];
//		aiQuatKey nextRotationKeyFrame = animation->mRotationKeys[nextRotationKeyFrameIndex];
//		float difference = nextRotationKeyFrame.mTime - rotationKeyFrame.mTime;
//		float ratio = (keyFrameTime - rotationKeyFrame.mTime) / difference;
//
//		if (ratio < 0.0f) ratio = 0.0f;
//		if (ratio > 1.0f) ratio = 1.0f;
//
//		glm::quat result;
//		glm::quat start;
//		glm::quat end;
//		CastToGLM(rotationKeyFrame.mValue, start);
//		CastToGLM(nextRotationKeyFrame.mValue, end);
//
//		result = glm::slerp(start, end, ratio);
//		result = glm::normalize(result);
//
//#ifdef PRINT_DEBUG_INFO
//		//printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
//			//scaleKeyFrame.value.x, scaleKeyFrame.value.y,
//			//nextScaleKeyFrame.value.x, nextScaleKeyFrame.value.y,
//			//result.x, result.y);
//#endif
//
//		return result;
//	}
}