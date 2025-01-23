#include "MwView.hpp"
#include <MwCtrl.hpp>

#include <nfd.h>
#include <spdlog/spdlog.h>

#include <window/Button.hpp>

namespace elcad::view
{
	MwView::MwView()
		: Window{ "ElCAD", 800, 600 }
	{
		addRenderables();
	}

	auto MwView::addRenderables() -> void
	{
		auto loadMesh = makeUnique<win::Button>
		(
			"Load mesh"
		);

		loadMesh->setSize(200.0f, 100.0f);

		loadMesh->setPosition(0.0f, 0.0f);

		loadMesh->onClicked
		(
			[self = this]()
			{
				self->onLoadMeshButtonClicked();
			}
		);

		addRenderable
		(
			std::move(loadMesh)
		);
	}

	auto MwView::init(WPtr<ctrl::MwCtrl> ctrl) -> void
	{
		if (ctrl.expired())
		{
			throw std::runtime_error{ "Controller to view expired" };
		}

		m_ctrl = ctrl;
	}

	auto MwView::getCtrl() -> SPtr<ctrl::MwCtrl>
	{
		if (m_ctrl.expired())
		{
			throw std::runtime_error{ "Controller to view expired" };
		}

		return m_ctrl.lock();
	}

	auto MwView::mousePosChanged(u32 x, u32 y) -> void
	{
	}

	auto MwView::mouseLeftButtonClicked(win::KeyModifier mod) -> void
	{
	}

	auto MwView::mouseRightButtonClicked(win::KeyModifier mod) -> void
	{
	}

	auto MwView::mouseMiddleButtonClicked(win::KeyModifier mod) -> void
	{
	}

	auto MwView::keyPressedEvent(win::Key key, win::KeyModifier mod) -> void
	{
	}

	auto MwView::keyReleasedEvent(win::Key key, win::KeyModifier mod) -> void
	{
	}

	auto MwView::keyRepeatEvent(win::Key key, win::KeyModifier mod) -> void
	{
	}

	auto MwView::resizedEvent(u32 width, u32 height) -> void
	{
	}

	auto MwView::closeEvent() -> void
	{
	}

	auto MwView::onLoadMeshButtonClicked() -> void
	{
		nfdchar_t* outPath = NULL;

		auto result = NFD_OpenDialog(nullptr, nullptr, &outPath);

		if (result == NFD_OKAY)
		{
			getCtrl()->loadGeometryFromFileAsync
			(
				StringView{ outPath }
			);
		}
	}
}