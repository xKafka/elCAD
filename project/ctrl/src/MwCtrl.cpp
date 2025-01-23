#include "MwCtrl.hpp"
#include <
#include "MainWindow.hpp"

#include <model/importers/Importers.hpp>
#include <model/Geometry.hpp>

namespace elcad::ctrl
{
    MwController::MwController(SPtr<fw::Executor> executor)
        : m_executor{ executor }
    {

    }

    auto MwController::init(SPtr<view::MwView> mwView) -> void
    {
        if (!mwView)
        {
            throw std::invalid_argument{ "view empty" };
        }

        m_mwView = mwView;
    }

    auto MwController::view() -> SPtr<view::MwView>
    {
        return m_mwView;
    }



    auto MwController::run() -> void
    {
        m_clock.start();

        m_executor->onMain
        (
            [&]()
            {
                view()->show();

            }
        )
    }

    auto MwController::update() -> void
    {

    }

    auto MwController::shutdown() -> void
    {
        m_mwView->shutdown();
    }
}