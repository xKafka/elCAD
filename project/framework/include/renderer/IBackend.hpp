#pragma once

#include <util/Types.hpp>

#include "renderer/Scene.hpp"

namespace elcad::win
{
	class Window;
}

namespace elcad::renderer
{
	class IBackend
	{
	public:
		virtual ~IBackend() {};

		virtual auto init(StringView appName, u32 version, SPtr<const win::Window> window) -> void = 0;

		virtual auto shutdown() -> void = 0;

		virtual auto resize(u32 width, u32 height) -> void = 0;

		virtual auto beginFrame(f32 delta) -> bool = 0;

		virtual auto endFrame(f32 delta) -> bool = 0;

		virtual auto renderTest() -> void = 0;

		virtual auto renderScene(SPtr<Scene> scene) -> void;
	};

	struct Packet
	{
		f32 delta{};
	};
}