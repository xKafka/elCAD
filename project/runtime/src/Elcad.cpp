#include "Elcad.hpp"

namespace elcad::runtime
{
	Elcad::Elcad(i32 argc, const char* argv[])
		: m_executors{ makeShared<fw::Executor>() }
	{
		m_mwCtrl = makeShared<ctrl::MwController>
		(
			m_executors
		);

		m_mwView = makeShared<view::MwView>
		(
			m_executors
		);
	}

	Elcad::~Elcad()
	{

	}

	auto Elcad::init() -> void
	{
		m_mwView->init(m_mwCtrl);

		m_mwCtrl->init(m_mwView);
	}

	auto Elcad::exec() -> i32
	{
		m_executors->start();

		while (true)
		{
			m_executors->poll();
		}

		m_mwCtrl->run();

		return 0;
	}
}