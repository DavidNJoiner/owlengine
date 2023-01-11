//core
#include <core/application.hpp>

//renderer
#include <renderer/lve_buffer.hpp>

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>
#include <iostream>

namespace lve {

    // object to pass read only data to the pipeline shaders 
    struct GlobalUbo {
        glm::mat4 projectionView{ 1.f };
        glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f }; // w is intensity
        glm::vec4 lightPosition{ -1.f }; // vec4 for alignment
        glm::vec4 lightColor{ 1.f }; // w is light intensity
    };

	Application::Application(application_config &app_config_info) 
    {
        app_state.width = app_config_info.width;
        app_state.is_initialized = TRUE;
        loadGameObjects();
    }
	Application::~Application() { app_state.is_running = FALSE; };


    void Application::run() {
  
        //Initialize subsystems
        initialize_logging();
        app_state.is_running = TRUE;

        // Create an uniform buffer object
        std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<LveBuffer>(
                lveDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };
        LveCamera camera{};

		std::cout << "maxPushConstantSize: " << lveDevice.properties.limits.maxPushConstantsSize << std::endl;

        auto viewerObject = LveGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        // Stats
        auto lag = 0.0;
        auto last_time = std::chrono::high_resolution_clock::now();
        double TARGET_FRAME_SECOND = 1.0 / 60.0;

        // Game loop
		while (!lveWindow.shouldClose()) {
			glfwPollEvents();

            auto current_time = std::chrono::high_resolution_clock::now(); // get current time
            float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - last_time).count();
            app_state.last_time = delta_time;
            last_time = current_time;

            //Update lag based on how much real time passed.
            //This measures how far the game's clock is behind compared to the real world
            lag += delta_time;

            //We then have an inner loop to update the game, one fixed step at a time,
            //until it's caught up. 
            while (lag >= TARGET_FRAME_SECOND)
            {
                // Game Logic
                lag -= TARGET_FRAME_SECOND;
            }
            // --------------------------------------------------------------------------------
            // Rendering outside of the game logic frees up a bunch of CPU time.
            //The game simulates at a constant rate using safe fixed time step
            //The player's visible window into the game gets choppier on a slower machine but
            //the logic keeps being updated at constant time.

            cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), delta_time, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();

            // FOV 50, aspect ratio, near, far
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

			if (auto commandBuffer = lveRenderer.beginFrame()) 
			{
                int frameIndex = lveRenderer.getFrameIndex();
                FrameInfo frameInfo{ frameIndex, delta_time, commandBuffer, camera };

                //update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects); // here we should pass the lag
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(lveDevice.device());
	}

	void Application::loadGameObjects()
	{
        std::shared_ptr<LveModel> lveModel2 = LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");

        auto flatVase = LveGameObject::createGameObject();
        flatVase.model = lveModel2;
        flatVase.transform.translation = { -.5f, .5f, 2.5f };
        flatVase.transform.scale = glm::vec3(3.f);
        gameObjects.push_back(std::move(flatVase));
	}
}//namespace lve