#pragma once

#ifndef ELCAD_WINDOW_GENERIC_H
#define ELCAD_WINDOW_GENERIC_H

#include "renderer/vulkan/VContext.hpp"

#include "Key.hpp"

namespace elcad::win
{
	class Window
	{
		class Impl;

	public:
		Window(StringView title, u32 width, u32 height);

		virtual ~Window();

		auto show() -> void;

		auto showFullScreen() -> void;

		auto hide() -> void;

		auto close() -> void;

		auto shouldClose() -> bool;

		auto resize(u32 width, u32 height) -> void;

		auto update() -> void;

		auto getWidth() const -> u32;

		auto getHeight() const -> u32;

		auto createVulkanSurface(const vk::Instance& instance) const -> vk::SurfaceKHR;

		auto canQueuePresent(const vk::Instance& instance, const vk::PhysicalDevice& device, u32 queueFamily) const -> bool;

		auto getHandler() const -> const GLFWwindow*;

		static auto requiredVulkanInstanceExtensions() -> Vec<const char*>;

	public:
		virtual auto render() const -> void = 0;

	protected:
		virtual auto mousePosChanged(u32 x, u32 y) -> void = 0;

		virtual auto mouseLeftButtonClicked(KeyModifier mod) -> void = 0;

		virtual auto mouseRightButtonClicked(KeyModifier mod) -> void = 0;

		virtual auto mouseMiddleButtonClicked(KeyModifier mod) -> void = 0;

		virtual auto keyPressedEvent(Key key, KeyModifier mod) -> void = 0;

		virtual auto keyReleasedEvent(Key key, KeyModifier mod) -> void = 0;

		virtual auto keyRepeatEvent(Key key, KeyModifier mod) -> void = 0;

		virtual auto resizedEvent(u32 width, u32 height) -> void = 0;

		virtual auto closeEvent() -> void = 0;

	private:
		SPtr<Impl>	m_impl{};
	};
}

#endif
