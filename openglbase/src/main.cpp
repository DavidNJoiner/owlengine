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
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 1080;

static void UpdateLights(const Scene& scene, Camera& camera)
{
    // Iterate through lights in the scene
    const auto& pointLights = scene.GetLights("PointLight");

    for (const auto& light : pointLights)
    {
        auto pointLight = std::static_pointer_cast<PointLight>(light);
        std::shared_ptr<Shader> shader = pointLight->GetShader();
        shader->setUniform1i("material.diffuse", 0);
        shader->setUniform1i("material.specular", 1);
        shader->setUniform1f("material.shininess", 78.0f);
        shader->setUniform3f("pointLights.position", pointLight->GetPosition());
        shader->setUniform3f("pointLights.ambient", pointLight->GetColor() * 0.1f);
        shader->setUniform3f("pointLights.diffuse", pointLight->GetColor() * 0.5f);
        shader->setUniform3f("pointLights.specular", pointLight->GetColor() * 0.3f);
        shader->setUniform1f("pointLights.constant", pointLight->GetConstant());
        shader->setUniform1f("pointLights.linear", pointLight->GetLinear());
        shader->setUniform1f("pointLights.quadratic", pointLight->GetQuadratic());
    }

    // Iterate through lights in the scene
    const auto& spotLights = scene.GetLights("SpotLight");

    for (const auto& light : spotLights)
    {
        auto spotLight = std::static_pointer_cast<SpotLight>(light);
        std::shared_ptr<Shader> shader = spotLight->GetShader();
        shader->setUniform1i("material.diffuse", 0);
        shader->setUniform1i("material.specular", 1.2);
        shader->setUniform1f("material.shininess", 128.0f);
        shader->setUniform3f("spotLight.position", spotLight->GetPosition());
        shader->setUniform3f("spotLight.direction", spotLight->GetDirection());
        shader->setUniform1f("spotLight.cutOff", spotLight->GetCutOff());
        shader->setUniform1f("spotLight.outerCutOff", spotLight->GetOuterCutOff());
        shader->setUniform1f("spotLight.constant", spotLight->GetConstant());
        shader->setUniform1f("spotLight.linear", spotLight->GetLinear());
        shader->setUniform1f("spotLight.quadratic", spotLight->GetQuadratic());
        shader->setUniform3f("spotLight.ambient", spotLight->GetColor() * 0.2f);
        shader->setUniform3f("spotLight.diffuse", spotLight->GetColor() * 0.2f);
        shader->setUniform3f("spotLight.specular", spotLight->GetColor() * 0.4f);
    }
}

std::vector<DebugVertex> GenerateDebugCube(const Vec3& origin, const Color& color)
{
    std::vector<DebugVertex> cubeVertices;

    // Cube vertices relative to origin
    Vec3 vertices[8] = {
        origin + Vec3(0.0f, 0.0f, 0.0f), // 0
        origin + Vec3(1.0f, 0.0f, 0.0f), // 1
        origin + Vec3(1.0f, 1.0f, 0.0f), // 2
        origin + Vec3(0.0f, 1.0f, 0.0f), // 3
        origin + Vec3(0.0f, 0.0f, 1.0f), // 4
        origin + Vec3(1.0f, 0.0f, 1.0f), // 5
        origin + Vec3(1.0f, 1.0f, 1.0f), // 6
        origin + Vec3(0.0f, 1.0f, 1.0f)  // 7
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
    Window wnd("Graphics GL", SCR_WIDTH, SCR_WIDTH);
    wnd.backgroundColor = Vec4(0.4, 0.4f, 0.4f, 1.0f);
    //std::cout << "Current Working Directory: " << std::filesystem::current_path() << "\n" << std::endl;

    Vec3 LightPositions[] =
    {
        Vec3(0.f, 0.3f, 0.f),
        Vec3(0.f, 2.0f, 0.5f)
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
    //auto basic = resourceManager->LoadResource<Shader>("res/shaders/standard");
    auto spotLightShader = resourceManager->LoadResource<Shader>("res/shaders/spotlight");
    auto pointLightShader = resourceManager->LoadResource<Shader>("res/shaders/pointlight");
    auto lightSourceShader = resourceManager->LoadResource<Shader>("res/shaders/lightsource");
    //==========================

    // MODELS ===================
    //auto Room312 = resourceManager->LoadResource<Model>("res/tests/room312/rr91.obj");
    //Room312->SetPosition(Vec3(0.0f, 0.0f, -10.f));

    //auto stairs = resourceManager->LoadResource<Model>("res/tests/stairs/stairs.obj");
    //stairs->SetBufferUsage(BufferUsage::Static);
    //stairs->SetPosition(Vec3(0.f, 0.0f, 0.f));

    // TODO : Handle Multiple Instances
    auto cube1m = resourceManager->LoadResource<Model>("res/tests/cube1m/cube1m.obj");
    cube1m->SetBufferUsage(BufferUsage::Static);
    cube1m->SetPosition(Vec3(0.f, 0.0f, 0.f));
    auto cube1m2 = resourceManager->LoadResource<Model>("res/tests/cube1m/cube1m.obj");
    cube1m2->SetBufferUsage(BufferUsage::Static);
    cube1m2->SetPosition(Vec3(0.f, 0.0f, 0.1f));

    auto floor = resourceManager->LoadResource<Model>("res/tests/plane/plane.obj");
    floor->SetBufferUsage(BufferUsage::Static);
    floor->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

    auto lightSphere = resourceManager->LoadResource<Model>("res/tests/sSphere/sSphere.obj");
    lightSphere->SetBufferUsage(BufferUsage::Static);
    lightSphere->SetPosition(Vec3(0.0f, 0.4f, 0.0f));

    //==========================

    // Checks loaded resources
    resourceManager->PrintResources();

    // CAMERA SETUP ============
    Camera camera(Vec3(0.0f, 0.25f, 0.f));

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

    // Enable cursor capture
    glfwSetInputMode(wnd.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Light properties
    const Vec3 lightColor = Vec3(0.9686f, 0.9216f, 0.7922f); // Color of the light
    const Vec3 lightColor2 = Vec3(0.9686f, 0.9216f, 0.7922f); // Color of the light
    const Vec3 lightDirection = Vec3(0.0f, -1.0f, 0.f); // Direction of the spotlight (negative Z-axis)

    // SpotLight parameters
    float cutOff = cos(MATH_DEG_TO_RAD(15.5f));  // Spotlight's inner cone cutoff (12.5 degrees)
    float outerCutOff = cos(MATH_DEG_TO_RAD(40.5f));  // Spotlight's outer cone cutoff (17.5 degrees)

    // Create light objects
    auto light1 = std::make_shared<PointLight>(LightPositions[0], lightColor, 1.0f, 0.09f, 0.032f, pointLightShader);  // Point light
    auto light2 = std::make_shared<SpotLight>(LightPositions[1], lightDirection, lightColor2, cutOff, outerCutOff, spotLightShader);  // Spotlight

    light2->SetPosition(LightPositions[1]);

    scene.AddLight("PointLight", light1);
    scene.AddLight("SpotLight", light2);
    UpdateLights(scene, camera);

    // DEBUG CUBE OF LINES ============
    const Vec3 cubeOrigin(0.0f, 0.0f, 0.0f); // Cube starting position
    const Color cubeColor = Color::Orange;  // Replace with your `Color::Orange` definition

    std::vector<DebugVertex> cubeLines = GenerateDebugCube(cubeOrigin, cubeColor);

    // Add lines to your debug renderer
    for (size_t i = 0; i < cubeLines.size(); i += 2)
    {
        DebugRenderer::AddLine(new Line(cubeLines[i], cubeLines[i + 1]));
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
        
        // Set the uniform in the shader
        //UpdateLights(scene, camera);


        // RENDER DEBUG STUFF ==================================================
        DebugRenderer::Render();

        

        lightSourceShader->setUniformMat4f("model", lightSphere->GetTransformMatrix());
        lightSourceShader->setUniform3f("sourceColor", lightColor);

        // Render the light source
        lightSphere->Render(lightSourceShader, camera.GetViewUniform());

        // Render point lights
        //for (const auto& light : scene.GetLights("PointLight"))
        UpdateLights(scene, camera);

        /*
        light1->GetShader()->setUniformMat4f("model", floor->GetTransformMatrix());
        floor->Render(light1->GetShader(), viewUniformsData);
        light1->GetShader()->setUniformMat4f("model", cube1m->GetTransformMatrix());
        cube1m->Render(light1->GetShader(), viewUniformsData);
        light1->GetShader()->setUniformMat4f("model", cube1m2->GetTransformMatrix());
        cube1m2->Render(light1->GetShader(), viewUniformsData);
        */

        light2->SetSpotlightDirection(camera.GetFront());
        light2->SetPosition(camera.GetTransform()->GetPosition());

        light2->GetShader()->setUniformMat4f("model", floor->GetTransformMatrix());
        floor->Render(light2->GetShader(), camera.GetViewUniform());
        light2->GetShader()->setUniformMat4f("model", cube1m->GetTransformMatrix());
        cube1m->Render(light2->GetShader(), camera.GetViewUniform());
        light2->GetShader()->setUniformMat4f("model", cube1m2->GetTransformMatrix());
        cube1m2->Render(light2->GetShader(), camera.GetViewUniform());

        //nightSky.Render(camera.GetViewMatrix(), camera.GetProjectionMatrix());

        wnd.update();
    }

    wnd.closed();
    return 0;
}