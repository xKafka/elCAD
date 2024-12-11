#include "renderer/Frontend.hpp"

#include "renderer/vulkan/VBackend.hpp"

namespace elcad::renderer
{
	Frontend::Frontend(BackendType type)
		: m_frameNumber{ 0 }
	{
		switch (type)
		{
		case elcad::renderer::Vulkan:
		{
			m_backend = makeUnique<VBackend>();
		}
		break;
		default:
			break;
		}
	}

	auto Frontend::init(StringView appName, u32 version, SPtr<const win::Window> window) -> void
	{
		m_backend->init(appName, version, window);
	}

	auto Frontend::shutdown() -> void
	{
		m_backend->shutdown();
	}

	auto Frontend::resize(u32 width, u32 height) -> void
	{
		m_backend->resize(width, height);
	}

	auto Frontend::beginFrame(f32 delta) -> bool
	{
		return m_backend->beginFrame(delta);
	}

	auto Frontend::endFrame(f32 delta) -> bool
	{
		++m_frameNumber;

		return m_backend->endFrame(0.0);
	}

	auto Frontend::drawFrame(SPtr<Packet> packet) -> void
	{
		if (!beginFrame(0.0))
		{
			return;
		}

		//we'll not recover from this
		if (!endFrame(0.0))
		{
			throw std::runtime_error{ "renderer end frame failed, Shutting down..." };
		}
	}
}