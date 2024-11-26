#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <filesystem>

#include "stb_image.h"

// Logic
#include "renderer/math/math.h"

// Debug Renderer
#include "renderer/debug/DebugRenderer.h"

// Renderer
#include "renderer/types/Shader.h"
#include "renderer/Model.h"
#include "renderer/Mesh.h"
#include "renderer/Window.h"
#include "renderer/Camera.h"

// Game
#include "game/Scene.h"
#include "game/Player.h"
#include "game/entities/lights/PointLight.h"
#include "game/entities/lights/SpotLight.h"

#include "systems/SystemManager.h"
#include "systems/ResourceManager.h"

// settings
const unsigned int SCR_WIDTH = 1600;//2560
const unsigned int SCR_HEIGHT = 1440; //1440
#define MAX_POINTLIGHT 2
#define MAX_SPOTLIGHT 2


static void UpdateLights(const Scene& scene, const std::shared_ptr<Shader>& mainShader)
{
    // Point lights
    const auto& pointLights = scene.GetLights("PointLight");
    for (size_t i = 0; i < pointLights.size() && i < MAX_POINTLIGHT; ++i)
    {
        auto pointLight = std::static_pointer_cast<PointLight>(pointLights[i]);
        std::string baseUniform = "pointLights[" + std::to_string(i) + "].";
        mainShader->setUniform1i("material.diffuse", 0); // SlotID 0 for diffuse Texture2D
        mainShader->setUniform1f("material.shininess", 128.0f);
        mainShader->setUniform3f((baseUniform + "position").c_str(), pointLight->GetPosition());
        mainShader->setUniform1f((baseUniform + "constant").c_str(), pointLight->GetConstant());
        mainShader->setUniform1f((baseUniform + "linear").c_str(), pointLight->GetLinear());
        mainShader->setUniform1f((baseUniform + "quadratic").c_str(), pointLight->GetQuadratic());
        mainShader->setUniform3f((baseUniform + "ambient").c_str(), pointLight->GetVec3Color() * 0.1f);
        mainShader->setUniform3f((baseUniform + "diffuse").c_str(), pointLight->GetVec3Color() * 0.3f);
        mainShader->setUniform3f((baseUniform + "specular").c_str(), pointLight->GetVec3Color() * 0.3f);
    }

    // Spot lights
    const auto& spotLights = scene.GetLights("SpotLight");
    for (size_t i = 0; i < spotLights.size() && i < MAX_SPOTLIGHT; ++i)
    {
        auto spotLight = std::static_pointer_cast<SpotLight>(spotLights[i]);
        std::string baseUniform = "spotLights[" + std::to_string(i) + "].";
        mainShader->setUniform1i("material.diffuse", 0);// SlotID 0 for diffuse Texture2D
        mainShader->setUniform1f("material.shininess", 220.0f);
        mainShader->setUniform3f((baseUniform + "position").c_str(), spotLight->GetPosition());
        mainShader->setUniform3f((baseUniform + "direction").c_str(), spotLight->GetDirection());
        mainShader->setUniform1f((baseUniform + "cutOff").c_str(), spotLight->GetCutOff());
        mainShader->setUniform1f((baseUniform + "outerCutOff").c_str(), spotLight->GetOuterCutOff());
        mainShader->setUniform1f((baseUniform + "constant").c_str(), spotLight->GetConstant());
        mainShader->setUniform1f((baseUniform + "linear").c_str(), spotLight->GetLinear());
        mainShader->setUniform1f((baseUniform + "quadratic").c_str(), spotLight->GetQuadratic());
        mainShader->setUniform3f((baseUniform + "ambient").c_str(), spotLight->GetVec3Color() * 0.2f);
        mainShader->setUniform3f((baseUniform + "diffuse").c_str(), spotLight->GetVec3Color() * 0.8f);
        mainShader->setUniform3f((baseUniform + "specular").c_str(), spotLight->GetVec3Color() * 0.8f);
    }
}

std::vector<DebugVertex> GenerateDebugCube(const Vec3& origin, const Color& color)
{
    std::vector<DebugVertex> cubeVertices;

    // Cube vertices relative to origin
    Vec3 vertices[8] = {
        origin + Vec3(0.0f, 0.0f, 0.0f), // 0
        origin + Vec3(0.5f, 0.0f, 0.0f), // 1
        origin + Vec3(0.5f, 0.5f, 0.0f), // 2
        origin + Vec3(0.0f, 0.5f, 0.0f), // 3
        origin + Vec3(0.0f, 0.0f, 0.5f), // 4
        origin + Vec3(0.5f, 0.0f, 0.5f), // 5
        origin + Vec3(0.5f, 0.5f, 0.5f), // 6
        origin + Vec3(0.0f, 0.5f, 0.5f)  // 7
    };

    // Define edges of the cube (pairs of vertex indices)
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
    };

    // Create DebugVertex pairs for each edge
    for (int i = 0; i < 12; ++i)
    {
        cubeVertices.push_back({ vertices[edges[i][0]], color });
        cubeVertices.push_back({ vertices[edges[i][1]], color });
    }

    return cubeVertices;
}

int main()
{
    Window wnd("DEBUG ENGINE", SCR_WIDTH, SCR_WIDTH);
    wnd.backgroundColor = Vec4(0.1, 0.1f, 0.1f, 1.0f);
    //std::cout << "Current Working Directory: " << std::filesystem::current_path() << "\n" << std::endl;

    Vec3 LightPositions[] =
    {
        Vec3(0.f, 2.0f, 0.f),
        Vec3(1.0f, 0.4f, 0.2f)
    };

    Scene scene;

    // Initialize SystemManager
    SystemManager& systemManager = SystemManager::GetInstance();

    // Register core systems
    auto* timeManager = systemManager.RegisterSystem<TimeManager>();
    auto* inputManager = systemManager.RegisterSystem<InputManager>();
    auto* resourceManager = systemManager.RegisterSystem<ResourceManager>();

    // Initialize resource types / Internally it create factories for each type
    resourceManager->RegisterResourceType<Texture2D>();
    resourceManager->RegisterResourceType<Model>();
    resourceManager->RegisterResourceType<Shader>();

    inputManager->SetWindow(wnd.getGLFWwindow());

    // SHADERS ===================
    std::cout << "[ LOADING SHADERS ]" << std::endl;
    auto lightShader = resourceManager->LoadResource<Shader>("res/shaders/gl_worldlights");
    auto lightSourceShader = resourceManager->LoadResource<Shader>("res/shaders/gl_lightsource");
    //==========================

    // MODELS ===================

    // TODO : Handle Multiple Instances
    auto cube1m = resourceManager->LoadResource<Model>("res/tests/cube1m/cube1m.obj");
    cube1m->SetBufferUsage(BufferUsage::Static);
    cube1m->SetPosition(Vec3(0.f, 0.0f, 0.f));

    auto floor = resourceManager->LoadResource<Model>("res/tests/plane/plane.obj");
    floor->SetBufferUsage(BufferUsage::Static);
    floor->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

    auto lightSource = resourceManager->LoadResource<Model>("res/tests/sSphere/sSphere.obj");
    lightSource->SetBufferUsage(BufferUsage::Static);
    lightSource->SetPosition(Vec3(0.0f, 0.4f, 0.0f));

    //==========================

    // CAMERA SETUP ============
    Camera camera(Vec3(0.0f, 1.0f, 0.f));

    float fov = 50.0f;  // Field of view in degrees
    float aspectRatio = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
    float nearPlane = 0.05f;  // Near clipping plane
    float farPlane = 100.0f; // Far clipping plane

    camera.SetProjection(fov, aspectRatio, nearPlane, farPlane);

    // Camera Controller
    CameraController* cameraController = new CameraController(&camera, Vec3(0.f, 0.f, 0.f));

    // DEBUG RENDERER ============
    DebugRenderer::Initialize();
    DebugRenderer::SetCamera(&camera);

    //==========================

     // Checks loaded resources
    resourceManager->PrintResources();

    // Enable cursor capture
    //glfwSetInputMode(wnd.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Color r = Color::Red;
    Color w = Color::White;

    Vec3 red = r.ToVec3();
    Vec3 white = w.ToVec3();

    // Create light objects
    auto light1 = std::make_shared<PointLight>(LightPositions[0], red);  // Point light
    auto light2 = std::make_shared<SpotLight>(LightPositions[1], white);  // Spotlight

    scene.AddLight("PointLight", light1);
    scene.AddLight("SpotLight", light2);

    // DEBUG CUBE OF LINES ============
    const Vec3 cubeOrigin(0.0f, 0.0f, 0.0f); // Cube starting position

    std::vector<DebugVertex> cubeLines = GenerateDebugCube(cubeOrigin, Color::Orange);
    std::vector<DebugVertex> cubeLines2 = GenerateDebugCube(Vec3(2.0f, 0.0f, 1.0f), Color::Blue);

    for (size_t i = 0; i < cubeLines.size(); i += 2)
    {
        DebugRenderer::AddLine(new Line(cubeLines[i], cubeLines[i + 1], Color::Orange));
        DebugRenderer::AddLine(new Line(cubeLines2[i], cubeLines2[i + 1], Color::Blue));
    }


    //==========================

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(wnd.getGLFWwindow()))
    {
        // Check for ESC key press to close the window
        if (inputManager->IsKeyPressed(OWL_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(wnd.getGLFWwindow(), true);
        }

        cameraController->Update(timeManager->GetDeltaTime());
        systemManager.UpdateSystems(timeManager->GetDeltaTime());

        wnd.clear();
        DebugRenderer::Render();

        light2->SetSpotlightDirection(camera.GetFront());
        light2->SetPosition(camera.GetTransform()->GetPosition());

        // Render the pointlight source
        lightSourceShader->setUniform3f("sourceColor", red);
        lightSource->Render(lightSourceShader, camera.GetViewUniform());

        // Update Lights shader uniforms
        UpdateLights(scene, lightShader);

        floor->Render(lightShader, camera.GetViewUniform());
        cube1m->Render(lightShader, camera.GetViewUniform());
        //room->Render(lightShader, camera.GetViewUniform());

        wnd.update();
    }

    DebugRenderer::ShutDown();
    wnd.closed();
    return 0;
}