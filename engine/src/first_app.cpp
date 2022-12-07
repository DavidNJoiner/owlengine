#include "first_app.hpp"

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
#include <vector>

namespace lve {

	FirstApp::FirstApp() {loadGameObjects();}
	FirstApp::~FirstApp() {};

    void FirstApp::run() {

        SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };
        LveCamera camera{};

		std::cout << "maxPushConstantSize: " << lveDevice.properties.limits.maxPushConstantsSize << std::endl;

        auto viewerObject = LveGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        // Stats

        auto lag = 0.0;
        auto previous = std::chrono::high_resolution_clock::now();
        double SECONDS_PER_UPDATE = 1.0 / 60.0;

        // Game loop
		while (!lveWindow.shouldClose()) {
			glfwPollEvents();

            auto current = std::chrono::high_resolution_clock::now(); // get current time
            float elapsed = std::chrono::duration<float, std::chrono::seconds::period>(current - previous).count();
            previous = current;

            //Update lag based on how much real time passed.
            //This measures how far the game�s clock is behind compared to the real world
            lag += elapsed;

            //We then have an inner loop to update the game, one fixed step at a time,
            //until it's caught up. 
            while (lag >= SECONDS_PER_UPDATE)
            {
                // Game Logic
                lag -= SECONDS_PER_UPDATE;
            }
            // --------------------------------------------------------------------------------
            // Rendering outside of the game logic frees up a bunch of CPU time.
            //The game simulates at a constant rate using safe fixed time step
            //The player�s visible window into the game gets choppier on a slower machine but
            //the logic keeps being updated.

            cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), elapsed, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

			if (auto commandBuffer = lveRenderer.beginFrame()) 
			{
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera); // here we should pass the lag
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/colored_cube.obj");

        auto gameObj = LveGameObject::createGameObject();
        gameObj.model = lveModel;
        gameObj.transform.translation = { .0f, .0f, 2.5f };
        gameObj.transform.scale = glm::vec3(0.5f);
        gameObjects.push_back(std::move(gameObj));
	}
}//namespace lve