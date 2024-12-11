
#pragma once

#include <functional>

#include "VContext.hpp"

namespace elcad::renderer
{
	class VImage
	{
		auto destroy() -> void;

		auto createImage(VkImageType type, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memFlags, VkImageAspectFlags viewAspectFlags) -> void;

		auto createVImageView(VkFormat format, VkImageAspectFlags aspectFlags) -> void;

	public:
		struct CreateInfo
		{
			SPtr<VContext>			ctx{};
				
			u32						width{};
			u32						height{};

			VkImageType				type{};
			VkFormat				format{};
			VkImageTiling			tiling{};
			VkImageUsageFlags		usage{};
			VkMemoryPropertyFlags	memFlags{};
			VkImageAspectFlags		viewAspectFlags{};

			bool					createView{};
		};

		VImage(const CreateInfo& info);

		VImage() = default;

		~VImage();

	private:
		SPtr<VContext>	m_ctx{};

		VkImage			m_handler{};

		VkDeviceMemory	m_memory{};

		VkImageView		m_view{};

		u32				m_width{}, m_height{};
	};
}


