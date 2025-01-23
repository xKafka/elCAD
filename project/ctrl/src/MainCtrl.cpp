#include "MainCtrl.hpp"
#include <MainView.hpp>

namespace elcad::ctrl
{
    MainCtrl::MainCtrl(SPtr<fw::Executor> executor)
        : m_executor{ executor }
    {

    }

    auto MainCtrl::init(WPtr<model::MainModel> model, WPtr<view::MainView> view) -> void
    {
        if (view.expired())
        {
            throw std::invalid_argument{ "view empty" };
        }

        m_view = view;

        if (model.expired())
        {
            throw std::invalid_argument{ "model empty" };
        }

        m_model = model;
    }

    auto MainCtrl::getView() -> SPtr<view::MainView>
    {
        if (m_view.expired())
        {
            throw std::runtime_error{ "view has expired" };
        }

        return m_view.lock();
    }

    auto MainCtrl::getModel() -> SPtr<model::MainModel>
    {
        if (m_model.expired())
        {
            throw std::runtime_error{ "model has expired" };
        }

        return m_model.lock();
    }

    auto MainCtrl::run() -> void
    {
        m_drawCtrl->run();
    }

    auto MainCtrl::update() -> void
    {
        auto updater = [&]()
        {
            m_drawCtrl->update();
        };

        m_executor->onMain(updater);
    }

    auto MainCtrl::shutdown() -> void
    {
    }
}