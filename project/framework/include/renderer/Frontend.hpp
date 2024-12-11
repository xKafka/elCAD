#pragma once

#include "IBackend.hpp"

namespace elcad::renderer
{
	enum BackendType : u8
	{
		Vulkan = 0,

		Count
	};

	class Frontend
	{
		auto beginFrame(f32 delta) -> bool;

		auto endFrame(f32 delta) -> bool;

	public:
		Frontend(BackendType type);

		auto init(StringView appName, u32 version, SPtr<const win::Window> window) -> void;

		auto shutdown() -> void;

		auto resize(u32 width, u32 height) -> void;

		auto drawFrame(SPtr<Packet> packet) -> void;

	private:
		UPtr<IBackend>	m_backend{};

		u64				m_frameNumber{};
	};
}