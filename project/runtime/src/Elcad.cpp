#include "Elcad.hpp"

namespace elcad::runtime
{
	Elcad::Elcad(i32 argc, const char* argv[])
		: m_mwCtrl{ makeShared<ctrl::MwController>() }
		, m_mwView{ makeShared<view::MwView>() }
	{
		//auto context = makeShared<renderer::vulkan::Context>("test", 1, true);

		//context->setupForWindow(m_mainWindow);

		//auto pipeline = renderer::vulkan::Pipeline
		//{
		//	context,
		//	renderer::vulkan::Shader::fromFile(context, "C:\\Users\\filip\\Documents\\elCAD\\resource\\shaders\\simple_vertex.spv"),
		//	renderer::vulkan::Shader::fromFile(context, "C:\\Users\\filip\\Documents\\elCAD\\resource\\shaders\\simple_frag.spv")
		//};

		//auto i = 60;
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
		m_mwCtrl->run();

		return 0;
	}
}