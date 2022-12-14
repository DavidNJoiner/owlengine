#include "lve_cubemap.hpp"

namespace lve {

	LveCubemap::LveCubemap(
		LveDevice& device,
		CubeMapConfigInfo& cubeMapConfigInfo)
	{
		cubeMapConfigInfo.vkImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		cubeMapConfigInfo.vkImageCreateInfo.arrayLayers = 6;
		cubeMapConfigInfo.vkImageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		cubeMapConfigInfo.vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		cubeMapConfigInfo.vkImageViewCreateInfo.subresourceRange.layerCount = 6;
	}
	LveCubemap::~LveCubemap()
	{
	}
}