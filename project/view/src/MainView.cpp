#include "MainView.hpp"

#include <MainCtrl.hpp>

namespace elcad::view
{
	MainView::MainView(SPtr<fw::Executor> executor)
		: m_executor{ executor }
		, m_mwView{ makeShared<MwView>() }
	{
		initRenderer();
	}

	auto MainView::init(WPtr<ctrl::MainCtrl> ctrl) -> void
	{
		if (ctrl.expired())
		{
			throw std::runtime_error{ "Controller to view expired" };
		}

		m_ctrl = ctrl;
	}

	auto MainView::initRenderer() -> void
	{
		m_renderer = makeUnique<renderer::Frontend>
		(
			renderer::BackendType::Vulkan
		);

		m_renderer->init("AppName", 0, m_mwView);
	}

	auto MainView::getCtrl() -> SPtr<ctrl::MainCtrl>
	{
		if (m_ctrl.expired())
		{
			throw std::runtime_error{ "Controller to view expired" };
		}

		return m_ctrl.lock();
	}

	auto MainView::run() -> void
	{
		m_mwView->show();
	}

	auto MainView::shutdown() -> void
	{
		m_renderer->shutdown();
	}

	auto MainView::updateRenderer() -> void
	{
		auto packet = makeShared<renderer::Packet>();

		packet->delta = static_cast<f32>(0.0);

		m_renderer->drawFrame(packet);
	}

	auto MainView::update() -> void
	{
		m_executor->onMain
		(
			[&]()
			{
				updateRenderer();

				m_mwView->update();
			}
		);
	}
}