#pragma once

#include <renderer/Frontend.hpp>
#include <system/Executor.hpp>

#include "MwView.hpp"

namespace elcad::ctrl
{
	class MainCtrl;
}

namespace elcad::view
{
	class MainView
	{
		auto getCtrl() -> SPtr<ctrl::MainCtrl>;

		auto initRenderer() -> void;

		auto updateRenderer() -> void;

	public:
		MainView(SPtr<fw::Executor> executor);

		auto init(WPtr<ctrl::MainCtrl> controller) -> void;

		auto run() -> void;

		auto update() -> void;

		auto shutdown() -> void;

	private:
		SPtr<fw::Executor>			m_executor{};

		WPtr<ctrl::MainCtrl>		m_ctrl{};

		UPtr<renderer::Frontend>    m_renderer{};

		SPtr<MwView>				m_mwView{};
	};
}