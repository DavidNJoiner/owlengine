#pragma once
#include <lve_window.hpp>
#include <lve_device.hpp>

namespace lve {

	struct CubeMapConfigInfo {

		VkImageCreateInfo vkImageCreateInfo;
		VkImageViewCreateInfo vkImageViewCreateInfo;

	};

	class LveCubemap {

	public: 
		LveCubemap(
			LveDevice& device,
			CubeMapConfigInfo& cubeMapConfigInfo
		);
		~LveCubemap();

	private:

		static VkImage cubemapImage;
	};
}