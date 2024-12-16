#include "window/Window.hpp"

#include "renderer/vulkan/VContext.hpp"

#include <stdexcept>

#include <spdlog/spdlog.h>

namespace elcad::win
{
	class Window::Impl
	{
		static auto framebufferResizeCb(GLFWwindow* window, i32 width, i32 height) -> void;

		static auto keyHandlerCb(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mod) -> void;

		static auto mouseKeyHandlerCb(GLFWwindow* window, i32 button, i32 action, i32 mod) -> void;

		static auto mousePosMoveCb(GLFWwindow* window, f64 x, f64 y) -> void;

		static auto windowCloseCb(GLFWwindow* window) -> void;

	public:
		Impl(Window* parent, std::string_view title, u32 width, u32 height);

		~Impl();

		auto init() -> void;

		auto catchResize(u32 width, u32 height) -> void;

		auto shouldClose() -> bool;

		auto show() -> void;

		auto showFullScreen() -> void;

		auto hide() -> void;

		auto close() -> void;

		auto resize(u32 width, u32 height) -> void;

		auto update() -> void;

		auto vulkanSurface(const vk::Instance& instance) const -> vk::SurfaceKHR;

		auto canQueuePresent(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, u32 queueFamily) const -> bool;

		[[nodiscard]] inline auto getWidth() const -> u32
		{
			return m_width;
		}

		[[nodiscard]] inline auto getHeight() const -> u32
		{
			return m_height;
		}

		[[nodiscard]] inline auto getGlfwHandler() const -> const GLFWwindow*
		{
			return m_glfw;
		}

		static auto requiredVulkanInstanceExtensions() -> Vec<const char*>;

	private:
		Window*				m_parent{};

		GLFWwindow*			m_glfw{};

		std::string			m_title{};

		u32					m_width{}, m_height{};

		bool				m_isShown{};
	};

	Window::Impl::Impl(Window* parent, std::string_view title, u32 width, u32 height)
		: m_parent{ parent }
		, m_title{ title }
		, m_glfw{ nullptr }
		, m_width{ width }
		, m_height{ height }
		, m_isShown{ false }
	{
	}

	Window::Impl::~Impl()
	{
		if (m_glfw)
		{
			glfwDestroyWindow(m_glfw);
		}

		glfwTerminate();
	}

	auto Window::Impl::framebufferResizeCb(GLFWwindow* window, i32 width, i32 height) -> void
	{
		auto self = static_cast<Window::Impl*>(glfwGetWindowUserPointer(window));

		self->catchResize(static_cast<u32>(width), static_cast<u32>(height));
	}

	auto Window::Impl::catchResize(u32 width, u32 height) -> void
	{
		m_width = width;

		m_height = height;

		m_parent->resizedEvent(width, height);
	}

	auto Window::Impl::keyHandlerCb(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mod) -> void
	{
		auto self = static_cast<Window::Impl*>(glfwGetWindowUserPointer(window));

		switch (static_cast<Action>(action))
		{
		case Action::Press:
		{
			self->m_parent->keyPressedEvent(static_cast<Key>(key), static_cast<KeyModifier>(mod));
		}
		break;
		case Action::Release:
		{
			self->m_parent->keyReleasedEvent(static_cast<Key>(key), static_cast<KeyModifier>(mod));
		}
		break;
		case Action::Repeat:
		{
			self->m_parent->keyRepeatEvent(static_cast<Key>(key), static_cast<KeyModifier>(mod));
		}
		break;
		default:
			break;
		}
	}

	auto Window::Impl::windowCloseCb(GLFWwindow* window) -> void
	{
		auto self = static_cast<Window::Impl*>(glfwGetWindowUserPointer(window));

		self->m_parent->closeEvent();
	}

	auto Window::Impl::mousePosMoveCb(GLFWwindow* window, f64 x, f64 y) -> void
	{
		auto self = static_cast<Window::Impl*>(glfwGetWindowUserPointer(window));

		self->m_parent->mousePosChanged(static_cast<u32>(x), static_cast<u32>(y));
	}

	auto Window::Impl::mouseKeyHandlerCb(GLFWwindow* window, i32 button, i32 action, i32 mod) -> void
	{
		auto self = static_cast<Window::Impl*>(glfwGetWindowUserPointer(window));

		switch (static_cast<MouseButton>(button))
		{
		case MouseButton::Left:
		{
			self->m_parent->mouseLeftButtonClicked(static_cast<KeyModifier>(mod));
		}
		break;
		case MouseButton::Right:
		{
			self->m_parent->mouseRightButtonClicked(static_cast<KeyModifier>(mod));
		}
		break;
		case MouseButton::Middle:
		{
			self->m_parent->mouseMiddleButtonClicked(static_cast<KeyModifier>(mod));
		}
		break;
		default:
			break;
		}
	}

	auto Window::Impl::init() -> void
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_glfw = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

		glfwSetWindowUserPointer(m_glfw, this);

		glfwSetFramebufferSizeCallback(m_glfw, framebufferResizeCb);

		glfwSetKeyCallback(m_glfw, keyHandlerCb);
		glfwSetCursorPosCallback(m_glfw, mousePosMoveCb);
		glfwSetMouseButtonCallback(m_glfw, mouseKeyHandlerCb);
		glfwSetWindowCloseCallback(m_glfw, windowCloseCb);
	}

	auto Window::Impl::shouldClose() -> bool
	{
		if (!m_glfw)
		{
			return false;
		}

		return glfwWindowShouldClose(m_glfw);
	}

	auto Window::Impl::show() -> void
	{
		if (!m_glfw)
		{
			return;
		}

		if (m_isShown)
		{
			return;
		}

		m_isShown = true;

		glfwShowWindow(m_glfw);
	}

	auto Window::Impl::showFullScreen() -> void
	{
	}

	auto Window::Impl::hide() -> void
	{
		if (!m_glfw)
		{
			return;
		}

		if (!m_isShown)
		{
			return;
		}

		m_isShown = false;

		glfwHideWindow(m_glfw);
	}

	auto Window::Impl::close() -> void
	{
		if (m_glfw)
		{
			glfwWindowShouldClose(m_glfw);
		}
	}

	auto Window::Impl::resize(u32 width, u32 height) -> void
	{
		if (!m_glfw)
		{
			return;
		}

		m_width = width;

		m_height = height;

		glfwSetWindowSize(m_glfw, static_cast<i32>(width), static_cast<i32>(height));
	}

	auto Window::Impl::update() -> void
	{
		glfwPollEvents();
	}

	auto Window::Impl::canQueuePresent(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, u32 queueFamily) const -> bool
	{
		return glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, queueFamily) == GLFW_TRUE;
	}

	auto Window::Impl::vulkanSurface(const vk::Instance& instance) const -> vk::SurfaceKHR
	{
		auto surface = VkSurfaceKHR{};

		auto result = glfwCreateWindowSurface(instance, m_glfw, nullptr, &surface);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}

		spdlog::info("Vulkan surface created");

		return surface;
	}

	auto Window::Impl::requiredVulkanInstanceExtensions() -> Vec<const char*>
	{
		u32 glfwExtensionCount{ 0 };

		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		auto extensions = Vec<const char*>(glfwExtensionCount);

		for (u32 i{ 0 }; i < glfwExtensionCount; ++i)
		{
			extensions[i] = glfwExtensions[i];
		}

		return extensions;
	}

	Window::Window(std::string_view title, u32 width, u32 height)
		: m_impl{ makeShared<Impl>(this, title, width, height) }
	{
		m_impl->init();
	}

	Window::~Window()
	{

	}

	auto Window::show() -> void
	{
		m_impl->show();
	}

	auto Window::showFullScreen() -> void
	{
		m_impl->showFullScreen();
	}

	auto Window::hide() -> void
	{
		m_impl->hide();
	}

	auto Window::shouldClose() -> bool
	{
		return m_impl->shouldClose();
	}

	auto Window::close() -> void
	{
		m_impl->close();
	}

	auto Window::resize(u32 width, u32 height) -> void
	{
		m_impl->resize(width, height);
	}

	auto Window::update() -> void
	{
		m_impl->update();
	}

	auto Window::getWidth() const -> u32
	{
		return m_impl->getWidth();
	}

	auto Window::getHeight() const -> u32
	{
		return m_impl->getHeight();
	}

	auto Window::createVulkanSurface(const vk::Instance& instance) const -> vk::SurfaceKHR
	{
		return m_impl->vulkanSurface(instance);
	}

	auto Window::canQueuePresent(const vk::Instance& instance, const vk::PhysicalDevice& device, u32 queueFamily) const -> bool
	{
		return m_impl->canQueuePresent(instance, device, queueFamily);
	}

	auto Window::getHandler() const -> const GLFWwindow*
	{
		return m_impl->getGlfwHandler();
	}

	auto Window::requiredVulkanInstanceExtensions() -> Vec<const char*>
	{
		return Impl::requiredVulkanInstanceExtensions();
	}
}