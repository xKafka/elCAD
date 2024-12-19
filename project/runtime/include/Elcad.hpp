#pragma once

#include <util/Types.hpp>

#include <MwController.hpp>
#include <MwView.hpp>

namespace elcad::runtime
{
	class Elcad
	{

	public:
		Elcad(i32 argc, const char* argv[]);

		~Elcad();

		auto init() -> void;

		auto exec() -> i32;

	private:
		SPtr<ctrl::MwController>	m_mwCtrl{};

		SPtr<view::MwView>			m_mwView{};
	};
}