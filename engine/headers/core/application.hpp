#pragma once
#include <core/logger.hpp>

#include <renderer/lve_window.hpp>
#include <renderer/lve_renderer.hpp>
#include <renderer/lve_device.hpp>
#include <renderer/simple_render_system.hpp>

#include "lve_game_object.hpp"
#include "keyboard_movement_controller.hpp"
#include "lve_camera.hpp"

//std
#include <memory>
#include <vector>

namespace lve {

	// Application configuration.
	struct application_config 
	{
		int width;
		int height;
	};

	struct AppState 
	{
		bool is_initialized = FALSE;
		bool is_running = FALSE;
		float last_time;
		int width;
		int height;
		//game* game_inst;
	};

	class Application {
	public:
		
		Application(application_config &app_config_info);
		~Application();

		//deleting copy constructor
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		LVE_API void run();

	private:

		AppState app_state;
		void loadGameObjects(); 

		LveWindow lveWindow{ 800, 800, "OwlEngineEditor"};
		LveDevice lveDevice{ lveWindow };
		LveRenderer lveRenderer{ lveWindow, lveDevice };

		std::vector<LveGameObject> gameObjects;
	};

}//namespace lve