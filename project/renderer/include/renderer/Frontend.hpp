#pragma once

#include "IBackend.hpp"
#include "Scene.hpp"

namespace elcad::model
{
	class Model;
}

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

		auto buildFrame() -> void;

	public:
		Frontend(BackendType type);

		auto init(WPtr<Model> model) -> void;



		auto init(StringView appName, u32 version, SPtr<const win::Window> window) -> void;

		auto shutdown() -> void;

		auto resize(u32 width, u32 height) -> void;

		auto drawFrame(SPtr<Packet> packet) -> void;

		auto renderScene(SPtr<Scene> scene) -> void;

		auto renderTest() -> void;

	private:
		UPtr<IBackend>	m_backend{};

		u64				m_frameNumber{};
	};
}