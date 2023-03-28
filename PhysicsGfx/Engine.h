#pragma once

#include "GameObject.h"
#include <vector>
#include "Model.h"
#include "physics.h"
#include "Model.h"

namespace gdp
{
    struct DebugRendererObject
    {
        unsigned int MaterialId;
        unsigned int MeshId;
    };

    void GDP_Initialize();
    void GDP_Destroy();
    void GDP_UpdateCallback(void (*Callback)(float dt));
    void GDP_RenderCallback(void (*Callback)(void));
    void GDP_Run();
    void GDP_CreateWindow(const char* title, const int width, const int height);

    // Physics
    //bool GDP_CollisionTest(GameObject* a, GameObject* b);
    physics::iPhysicsWorld* GDP_GetPhysicsWorld();
    physics::iRigidBody* GDP_CreateRigidBody(const physics::RigidBodyDesc& desc, physics::iShape* shape);
    physics::iSoftBody* GDP_CreateSoftBody(const physics::SoftBodyDesc& desc);
    physics::iVehicle* GDP_CreateVehicle(physics::iRigidBody* chassis);
    physics::iCharacterController* GDP_CreateCharacterController(
        physics::iConvexShape* shape, float stepHeight, const glm::vec3& up);

    // Assimp Loading
    void GDP_LoadFBXFile(unsigned int& id, std::string& animationName, const std::string& filename);
    GameObject* GDP_CreateAnimatedCharacter(
        const char* filename, const std::vector<std::string>& animations);

    // Animation
    void GDP_LoadAnimation(const char* name, AnimationData animation);

    // Assets
    void GDP_LoadTexture(unsigned int& id, const char* filepath);
    void GDP_LoadModel(unsigned int& id, const char* filepath, bool withBones = false);


    void GDP_CreateModel(unsigned int& id, const std::vector<glm::vec3>& Vertices, const std::vector<int>& triangles);
    void GDP_GetModelData(unsigned int id, std::vector<glm::vec3>& Vertices, std::vector<int>& triangles,
        unsigned int& numTriangles, unsigned int& vbo);
    void GDP_CreateModel(unsigned int& id, const std::vector<glm::vec3>& vertices, const std::vector<int>& triangles);
    Model* GDP_GetModel(unsigned int id);
    void GDP_CreateMaterial(unsigned int& id, unsigned int textureId, const glm::vec3& color);
    void GDP_CreateShaderProgramFromFiles(unsigned int& id, unsigned int& shaderProgramId,
        const char* vertexShader, const char* fragmentShader);

    // Game World
    GameObject* GDP_CreateGameObject(bool addToWorld = true);
    void GDP_SetCameraTarget(GameObject* go);
        
    // Camera
    void SetCameraPosition(const glm::vec3& position);
    void SetCameraFacingDirection(const glm::vec3& direction);
    void GetCameraPosition(glm::vec3& position);
    void GetCameraFacingDirection(glm::vec3& direction);

    // User Input
    bool GDP_IsKeyHeldDown(unsigned char key);
    bool GDP_IsKeyPressed(unsigned char key);
    bool GDP_IsKeyReleased(unsigned char key);
    void GDP_GetMousePosition(int& x, int& y);
    bool GDP_GetMouseButtonState(int button, int& state);

    // Debugging
    void SetDebugModelInfo(unsigned int meshId, unsigned int materialId);
}