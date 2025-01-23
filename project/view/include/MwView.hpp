#pragma once

#include <window/Window.hpp>

namespace elcad::ctrl
{
	class MwCtrl;
}

namespace elcad::view
{
	class MwView final : public win::Window
	{
		auto getCtrl() -> SPtr<ctrl::MwCtrl>;

		auto addRenderables() -> void;

		auto onLoadMeshButtonClicked() -> void;

	private:
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

	public:
		MwView();

		auto init(WPtr<ctrl::MwCtrl> ctrl) -> void;

	private:
		WPtr<ctrl::MwCtrl>	m_ctrl{};
	};
}