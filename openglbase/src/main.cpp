#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <filesystem>

#include "stb_image.h"

// Logic
#include "renderer/math/math.h"

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
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

static void UpdateLights(const Scene& scene, Camera& camera)
{
    Matrix view = camera.GetViewMatrix();
    Matrix projection = camera.GetProjectionMatrix();
    Vec3 viewpos = camera.GetTransform()->GetPosition();

    // Iterate through lights in the scene
    const auto& pointLights = scene.GetLights("PointLight");

    for (const auto& light : pointLights) 
    {
        auto pointLight = std::static_pointer_cast<PointLight>(light);
        std::shared_ptr<Shader> shader = pointLight->GetShader();
        shader->setUniformMat4f("view", view);
        shader->setUniformMat4f("projection", projection);
        shader->setUniform3f("viewPos", viewpos);
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
    /*
    if (auto spotLight = dynamic_cast<SpotLight*>(light)) {
        Shader* shader = spotLight->GetShader();
        shader->setUniformMat4f("view", view);
        shader->setUniformMat4f("projection", projection);
        shader->setUniform3f("viewPos", viewpos);
        shader->setUniform1i("material.diffuse", 0);
        shader->setUniform1i("material.specular", 2);
        shader->setUniform1f("material.shininess", 56.0f);
        shader->setUniform3f("pointLights[].position", spotLight->GetPosition());
        shader->setUniform3f("pointLights[].ambient", spotLight->GetColor() * 0.1f);
        shader->setUniform3f("pointLights[].diffuse", spotLight->GetColor() * 0.2f);
        shader->setUniform3f("pointLights[].specular", spotLight->GetColor() * 0.3f);
        shader->setUniform1f("pointLights[].cutOff", spotLight->GetCutOff());
        shader->setUniform1f("pointLights[].outerCutOff", spotLight->GetOuterCutOff());
    }
    */
}

int main()
{
    Window wnd("Graphics GL", SCR_WIDTH, SCR_WIDTH);
    wnd.backgroundColor = Vec4(0.4, 0.4f, 0.4f, 1.0f);
    //std::cout << "Current Working Directory: " << std::filesystem::current_path() << "\n" << std::endl;

    Vec3 LightPositions[] =
    {
        Vec3(0.f, 0.3f, 0.f),
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
    auto pointLightShader = resourceManager->LoadResource<Shader>("res/shaders/pointlight");
    auto lightSourceShader = resourceManager->LoadResource<Shader>("res/shaders/lightsource");
    auto gridShader = resourceManager->LoadResource<Shader>("res/shaders/floorgrid");
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
    CameraController* cameraController = new CameraController(&camera, Vec3(0.f, 0.f, -10.f));

    //==========================

    // Enable cursor capture
    glfwSetInputMode(wnd.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Lights
    const Vec3 lightColor = Vec3(0.9686f, 0.9216f, 0.7922f);
    auto light1 = std::make_shared<PointLight>(LightPositions[0], lightColor, 1.0f, 0.09f, 0.032f, pointLightShader);

    scene.AddLight("PointLight", light1);
    UpdateLights(scene, camera);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Init View Uniform Object (view, proj, camPos)
    ViewUniforms viewUniformsData = { Matrix(1.0f), Matrix(1.0f), Vec3(0.0f) };

    while (!glfwWindowShouldClose(wnd.getGLFWwindow()))
    {
        // Update deltaTime before using it
        timeManager->UpdateDeltaTime();

        inputManager->Update(timeManager->GetDeltaTime());

        // Check for ESC key press to close the window
        if (inputManager->IsKeyPressed(OWL_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(wnd.getGLFWwindow(), true);
        }

        cameraController->Update(timeManager->GetDeltaTime());
        systemManager.UpdateSystems(timeManager->GetDeltaTime());

        // Render scene
        wnd.clear();

        // Set the uniform in the shader
        //UpdateLights(scene, camera);

        glDepthMask(GL_FALSE);

        GLuint quadVAO;
        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);

        // Plane rendering setup
        Matrix model(1.0f);
        viewUniformsData.view = camera.GetViewMatrix();
        viewUniformsData.proj = camera.GetProjectionMatrix();
        viewUniformsData.pos = camera.GetTransform()->GetPosition();
     
        // Set uniforms for the grid shader
        gridShader->setUniform1f("near", nearPlane);
        gridShader->setUniform1f("far", farPlane);
        gridShader->setViewUniforms(viewUniformsData);


        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glDepthMask(GL_TRUE);
        
        // Render grid plane
        //gridPlane->Render(gridShader, viewUniformsData);
  
        lightSourceShader->setUniformMat4f("model", lightSphere->GetTransformMatrix());
        lightSourceShader->setUniform3f("sourceColor", lightColor);

        
        // Render the light source
        lightSphere->Render(lightSourceShader, viewUniformsData);

        // Render point lights
        for (const auto& light : scene.GetLights("PointLight"))
        {
            //light->GetShader()->setUniformMat4f("model", Room312->GetTransformMatrix());
            //Room312->Render(light->GetShader(), viewUniformsData);
            //light->GetShader()->setUniformMat4f("model", stairs->GetTransformMatrix());
            //stairs->Render(light->GetShader(), viewUniformsData);
            light->GetShader()->setUniformMat4f("model", floor->GetTransformMatrix());
            floor->Render(light->GetShader(), viewUniformsData);
            light->GetShader()->setUniformMat4f("model", cube1m->GetTransformMatrix());
            cube1m->Render(light->GetShader(), viewUniformsData);
            light->GetShader()->setUniformMat4f("model", cube1m2->GetTransformMatrix());
            cube1m2->Render(light->GetShader(), viewUniformsData);
        }
 
        //nightSky.Render(camera.GetViewMatrix(), camera.GetProjectionMatrix());
        
        wnd.update();
    }

    wnd.closed();
    return 0;
}


