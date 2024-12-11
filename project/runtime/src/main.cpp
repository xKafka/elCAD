#include <iostream>

#include "Elcad.hpp"

#include <spdlog/spdlog.h>

int main(int argc, const char* argv[]){
	spdlog::set_level(spdlog::level::trace);

	auto app = elcad::runtime::Elcad{ argc, argv };

	try
	{
		app.init();

		return app.exec();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}