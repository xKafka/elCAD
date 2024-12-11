#pragma once

#include <window/Window.hpp>

namespace elcad::ctrl
{
	class MwController;
}

namespace elcad::view
{
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

	private:
		WPtr<ctrl::MwController>	m_ctrl{};
	};
}