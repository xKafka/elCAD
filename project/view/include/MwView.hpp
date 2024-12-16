#pragma once

#include <window/Window.hpp>

#include <glm/glm.hpp>

namespace elcad::ctrl
{
	class MwController;
}

namespace elcad::view
{
	class Renderable
	{
	public:
		virtual auto render() -> void = 0;
	};

	class Button : public Renderable
	{
	public:
		Button(StringView text);

		auto render() -> void final;

		auto setPosition(f32 x, f32 y) -> void;

		auto setSize(f32 width, f32 height) -> void;

		auto setText(StringView text) -> void;

		auto onClicked(std::function<void()> callback) -> void;

		auto onHovered(std::function<void()> callback) -> void;

		auto onFocused(std::function<void()> callback) -> void;

	private:
		std::string						m_text{};

		glm::vec2						m_position{};

		glm::vec2						m_size{};

		Opt<std::function<void()>>		m_onClicked{};

		Opt<std::function<void()>>		m_onHovered{};

		Opt<std::function<void()>>		m_onFocused{};
	};

	class MwView final : public win::Window
	{
		//parent functions
		auto mousePosChanged(u32 x, u32 y) -> void;

		auto mouseLeftButtonClicked(win::KeyModifier mod) -> void;

		auto mouseRightButtonClicked(win::KeyModifier mod) -> void;

		auto mouseMiddleButtonClicked(win::KeyModifier mod) -> void;

		auto keyPressedEvent(win::Key key, win::KeyModifier mod) -> void;

		auto keyReleasedEvent(win::Key key, win::KeyModifier mod) -> void;

		auto keyRepeatEvent(win::Key key, win::KeyModifier mod) -> void;

		auto resizedEvent(u32 width, u32 height) -> void;

		auto closeEvent() -> void;

	private:
		auto ctrl() -> SPtr<ctrl::MwController>;

	public:
		MwView();

		auto init(WPtr<ctrl::MwController> ctrl) -> void;

		auto render() const -> void;

	private:
		WPtr<ctrl::MwController>	m_ctrl{};

		Vec<UPtr<Renderable>>		m_renderables{};
	};
}

//// A simple OOP-style ImGui window class
//class MyWindow {
//public:
//    using ButtonCallback = std::function<void()>;
//
//    MyWindow(std::string title,
//        std::string button1Label, ButtonCallback button1Callback,
//        std::string button2Label, ButtonCallback button2Callback)
//        : m_title(std::move(title))
//        , m_button1Label(std::move(button1Label))
//        , m_button2Label(std::move(button2Label))
//        , m_button1Callback(std::move(button1Callback))
//        , m_button2Callback(std::move(button2Callback))
//    {
//    }
//
//    // Call this each frame to draw the window and its contents
//    void render() {
//        // Begin the window
//        ImGui::Begin(m_title.c_str());
//
//        // Calculate centering of buttons
//        float windowVisibleX = ImGui::GetContentRegionAvail().x;
//        float buttonWidth = 100.0f;
//        float totalButtonsWidth = buttonWidth * 2.0f + ImGui::GetStyle().ItemSpacing.x;
//        float offsetX = (windowVisibleX - totalButtonsWidth) * 0.5f;
//        if (offsetX > 0.0f) {
//            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
//        }
//
//        // Draw first button
//        if (ImGui::Button(m_button1Label.c_str(), ImVec2(buttonWidth, 0.0f))) {
//            if (m_button1Callback) {
//                m_button1Callback();
//            }
//        }
//
//        ImGui::SameLine();
//
//        // Draw second button
//        if (ImGui::Button(m_button2Label.c_str(), ImVec2(buttonWidth, 0.0f))) {
//            if (m_button2Callback) {
//                m_button2Callback();
//            }
//        }
//
//        ImGui::End();
//    }
//
//private:
//    std::string m_title;
//    std::string m_button1Label;
//    std::string m_button2Label;
//    ButtonCallback m_button1Callback;
//    ButtonCallback m_button2Callback;
//};