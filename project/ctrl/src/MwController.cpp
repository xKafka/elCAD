#include "MwController.hpp"
#include "MwView.hpp"

#include <stdexcept>

namespace elcad::ctrl
{
    MwController::MwController()
        : m_renderer{ makeUnique<renderer::Frontend>(renderer::BackendType::Vulkan) }
    {

    }

    auto MwController::init(SPtr<view::MwView> mwView) -> void
    {
        if (!mwView)
        {
            throw std::invalid_argument{ "view empty" };
        }

        m_mwView = mwView;

        m_renderer->init("AppName", 0, m_mwView);
    }

    auto MwController::view() -> SPtr<view::MwView>
    {
        return m_mwView;
    }

    auto MwController::resize(u32 width, u32 height) -> void
    {
        m_renderer->resize(width, height);
    }

    auto MwController::renderSomething() -> void
    {
        m_renderer->renderTest();
    }

    auto MwController::run() -> void
    {
        m_clock.start();

        view()->show();

        while (!m_mwView->shouldClose())
        {
            const auto delta = m_clock.update();

            auto packet = makeShared<renderer::Packet>();

            packet->delta = static_cast<f32>(delta);

            m_renderer->drawFrame(packet);

            m_mwView->update();
        }

        shutdown();
    }

    auto MwController::shutdown() -> void
    {
        m_renderer->shutdown();
    }
}