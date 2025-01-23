#include "window/Button.hpp"

#include <imgui.h>

namespace elcad::win
{
	Button::Button(StringView text)
		: m_text{ text }
	{
	}

	auto Button::render() -> void
	{
		ImGui::SetCursorPos
		(
			ImVec2{ m_position.x, m_position.y }
		);

		ImGui::Button
		(
			m_text.c_str(), ImVec2{ m_size.x, m_size.y }
		);

		if (ImGui::IsItemClicked() && m_onClicked)
		{
			m_onClicked.value()();
		}

		if (ImGui::IsItemHovered() && m_onHovered)
		{
			m_onHovered.value()();
		}

		if (ImGui::IsItemFocused() && m_onFocused)
		{
			m_onFocused.value()();
		}
	}

	auto Button::setText(StringView text) -> void
	{
		m_text = String{ text };
	}

	auto Button::setPosition(f32 x, f32 y) -> void
	{
		m_position = glm::vec2{ x, y };
	}

	auto Button::setSize(f32 width, f32 height) -> void
	{
		m_size = glm::vec2{ width, height };
	}

	auto Button::onClicked(std::function<void()> callback) -> void
	{
		m_onClicked = callback;
	}

	auto Button::onHovered(std::function<void()> callback) -> void
	{
		m_onHovered = callback;
	}

	auto Button::onFocused(std::function<void()> callback) -> void
	{
		m_onFocused = callback;
	}
}