#include "MwView.hpp"
#include "MwController.hpp"

#include <spdlog/spdlog.h>

namespace elcad::view
{
	MwView::MwView()
		: Window{ "ElCAD", 800, 600 }
	{

	}

	auto MwView::init(WPtr<ctrl::MwController> ctrl) -> void
	{
		if (ctrl.expired())
		{
			throw std::runtime_error{ "Controller to view expired" };
		}

		m_ctrl = ctrl;
	}

	auto MwView::ctrl() -> SPtr<ctrl::MwController>
	{
		if (m_ctrl.expired())
		{
			throw std::runtime_error{ "Controller to view expired" };
		}

		return m_ctrl.lock();
	}

	auto MwView::mousePosChanged(u32 x, u32 y) -> void
	{
		//fmt::println("MainWindow::mousePosChanged x [{}] y [{}]\n", x, y);
	}

	auto MwView::mouseLeftButtonClicked(win::KeyModifier mod) -> void
	{
		//fmt::println("MainWindow::mouseLeftButtonClicked mods [{}]\n", std::to_underlying(mod));
	}

	auto MwView::mouseRightButtonClicked(win::KeyModifier mod) -> void
	{
		//fmt::println("MainWindow::mouseRightButtonClicked mods [{}]\n", std::to_underlying(mod));
	}

	auto MwView::mouseMiddleButtonClicked(win::KeyModifier mod) -> void
	{
		//fmt::println("MainWindow::mouseMiddleButtonClicked mods [{}]\n", std::to_underlying(mod));
	}

	auto MwView::keyPressedEvent(win::Key key, win::KeyModifier mod) -> void
	{
		//fmt::println("MainWindow::keyPressedEvent key [{}] mods [{}]\n", std::to_underlying(key), std::to_underlying(mod));
	}

	auto MwView::keyReleasedEvent(win::Key key, win::KeyModifier mod) -> void
	{
		//fmt::println("MainWindow::keyReleasedEvent key [{}] mods [{}]\n", std::to_underlying(key), std::to_underlying(mod));
	}

	auto MwView::keyRepeatEvent(win::Key key, win::KeyModifier mod) -> void
	{
		//fmt::println("MainWindow::keyRepeatEvent key [{}] mods [{}]\n", std::to_underlying(key), std::to_underlying(mod));
	}

	auto MwView::resizedEvent(u32 width, u32 height) -> void
	{
		ctrl()->resize(width, height);

		//fmt::println("MainWindow::resizedEvent width [{}] height [{}]\n", width, height);
	}

	auto MwView::closeEvent() -> void
	{
		//fmt::println("MainWindow::closeEvent\n");
	}
}