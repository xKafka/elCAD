#pragma once

#include <util/Types.hpp>
#include <system/Executor.hpp>

#include "MwCtrl.hpp"

namespace elcad::model
{
    class MainModel;
}

namespace elcad::view
{
    class MainView;
}

namespace elcad::ctrl
{
    class MainCtrl
    {
        auto getView() -> SPtr<view::MainView>;

        auto getModel() -> SPtr<model::MainModel>;

    public:
        MainCtrl(SPtr<fw::Executor> executor);

        auto init(WPtr<model::MainModel> model, WPtr<view::MainView> view) -> void;

        auto run() -> void;

        auto update() -> void;

        auto shutdown() -> void;
        
    private:
        SPtr<fw::Executor>      m_executor{};

        WPtr<view::MainView>    m_view{};

        WPtr<model::MainModel>  m_model{};

        SPtr<MwCtrl>            m_mwCtrl{};
    };
}