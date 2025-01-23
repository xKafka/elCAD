#pragma once

#include <util/Types.hpp>
#include <util/Math.hpp>

#include "IRenderable.hpp"

namespace elcad::win
{
	class Button : public IRenderable
	{
	public:
		Button(StringView text);

		auto setPosition(f32 x, f32 y) -> void;

		auto setSize(f32 width, f32 height) -> void;

		auto setText(StringView text) -> void;

		auto onClicked(std::function<void()> callback) -> void;

		auto onHovered(std::function<void()> callback) -> void;

		auto onFocused(std::function<void()> callback) -> void;

		auto render() -> void final;

	private:
		std::string						m_text{};

		glm::vec2						m_position{};

		glm::vec2						m_size{};

		Opt<std::function<void()>>		m_onClicked{};

		Opt<std::function<void()>>		m_onHovered{};

		Opt<std::function<void()>>		m_onFocused{};
	};
}