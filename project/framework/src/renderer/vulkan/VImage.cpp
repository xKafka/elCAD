#include "renderer/vulkan/VImage.hpp"

#include <spdlog/spdlog.h>

namespace elcad::renderer
{
	VImage::VImage(const CreateInfo& info)
		: m_ctx{ info.ctx }
		, m_width{ info.width }
		, m_height{ info.height }
	{
		createImage(info.type, info.format, info.tiling, info.usage, info.memFlags, info.viewAspectFlags);

		if (info.createView)
		{
			createVImageView(info.format, info.viewAspectFlags);
		}
	}

	VImage::~VImage()
	{
		spdlog::info("VImage::~VImage()");

		destroy();
	}

	auto VImage::createImage(VkImageType type, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memFlags, VkImageAspectFlags viewAspectFlags) -> void
	{
		auto createInfo = VkImageCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
		};

		createInfo.imageType = VK_IMAGE_TYPE_2D;

		createInfo.extent.width = m_width;
		createInfo.extent.height = m_height;
		createInfo.extent.depth = 1;		//TODO: Support configurable depth

		createInfo.mipLevels = 4;			//TODO: Support mip mapping
		createInfo.arrayLayers = 1;			//TODO: Support number of layers in the image
		createInfo.format = format;
		createInfo.tiling = tiling;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		createInfo.usage = usage;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;				//TODO: Configurable sample count
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;		//TODO: Configurable sharing mode

		//vkCheck(vkCreateImage(m_ctx->device()->logical(), &createInfo, m_ctx->allocator().get(), &m_handler));

		//auto memoryRequirements = VkMemoryRequirements{};
		//vkGetImageMemoryRequirements(m_ctx->device()->logical(), m_handler, &memoryRequirements);

		//auto memoryType = m_ctx->device()->findMemoryIndex(memoryRequirements.memoryTypeBits, memFlags);
		//
		//if(!memoryType)
		//{ 
		//	throw std::runtime_error{ "Required memory type not found. Image not valid" };
		//}

		//auto memoryAllocInfo = VkMemoryAllocateInfo
		//{
		//	.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO
		//};

		//memoryAllocInfo.allocationSize = memoryRequirements.size;
		//memoryAllocInfo.memoryTypeIndex = memoryType.value();

		//vkCheck(vkAllocateMemory(m_ctx->device()->logical(), &memoryAllocInfo, m_ctx->allocator().get(), &m_memory));

		//vkCheck(vkBindImageMemory(m_ctx->device()->logical(), m_handler, m_memory, 0));
	}

	auto VImage::createVImageView(VkFormat format, VkImageAspectFlags aspectFlags) -> void
	{
		auto createInfo = VkImageViewCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
		};

		createInfo.image = m_handler;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.subresourceRange.aspectMask = aspectFlags;

		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		//vkCheck(vkCreateImageView(m_ctx->device()->logical(), &createInfo, m_ctx->allocator().get(), &m_view));
	}

	auto VImage::destroy() -> void
	{
		if (m_view)
		{
			//vkDestroyImageView(m_ctx->device()->logical(), m_view, m_ctx->allocator().get());

			m_view = VK_NULL_HANDLE;
		}

		if (m_memory)
		{
			//vkFreeMemory(m_ctx->device()->logical(), m_memory, m_ctx->allocator().get());

			m_memory = VK_NULL_HANDLE;
		}

		if (m_handler)
		{
			//vkDestroyImage(m_ctx->device()->logical(), m_handler, m_ctx->allocator().get());

			m_handler = VK_NULL_HANDLE;
		}
	}
}