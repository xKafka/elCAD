#include "MwView.hpp"
#include "MwController.hpp"

#include <spdlog/spdlog.h>

#include <imgui.h>

namespace elcad::view
{
	Button::Button(StringView text)
		: m_text{ text }
	{
	}

	auto Button::render() -> void
	{
		ImGui::SetCursorPos
		(
			ImVec2{ m_position.x, m_position.y }
		);

		ImGui::Button
		(
			m_text.c_str(), ImVec2{ m_size.x, m_size.y }
		);

		if (ImGui::IsItemClicked() && m_onClicked)
		{
			m_onClicked.value()();
		}

		if (ImGui::IsItemHovered() && m_onHovered)
		{
			m_onHovered.value()();
		}

		if (ImGui::IsItemFocused() && m_onFocused)
		{
			m_onFocused.value()();
		}
	}

	auto Button::setText(StringView text) -> void
	{
		m_text = std::string{ text };
	}

	auto Button::setPosition(f32 x, f32 y) -> void
	{
		m_position = glm::vec2{ x, y };
	}

	auto Button::setSize(f32 width, f32 height) -> void
	{
		m_size = glm::vec2{ width, height };
	}

	auto Button::onClicked(std::function<void()> callback) -> void
	{
		m_onClicked = callback;
	}

	auto Button::onHovered(std::function<void()> callback) -> void
	{
		m_onHovered = callback;
	}

	auto Button::onFocused(std::function<void()> callback) -> void
	{
		m_onFocused = callback;
	}

	MwView::MwView()
		: Window{ "ElCAD", 800, 600 }
	{
		auto button1 = makeUnique<Button>("button1");

		button1->setSize(200.0f, 100.0f);

		button1->setPosition(0.0f, 0.0f);

		button1->onClicked
		(
			[&]()
			{
			}
		);

		button1->onHovered
		(
			[&]()
			{
			}
		);

		button1->onFocused
		(
			[&]()
			{
			}
		);

		auto button2 = makeUnique<Button>("button2");

		button2->setSize(200.0f, 100.0f);

		button2->setPosition(200.0f, 0.0f);

		button2->onClicked
		(
			[&]()
			{
				m_ctrl.lock()->renderSomething();
			}
		);

		button2->onHovered
		(
			[&]()
			{
			}
		);

		button2->onFocused
		(
			[&]()
			{
			}
		);

		m_renderables.emplace_back(std::move(button1));

		m_renderables.emplace_back(std::move(button2));
	}

	auto MwView::render() const -> void
	{
		for (auto& renderable : m_renderables)
		{
			renderable->render();
		}
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