#pragma once

#include <system/Executor.hpp>

namespace elcad::model
{
    class MwModel;
}

namespace elcad::view
{
    class MwView;
}

namespace elcad::ctrl
{
    class MwCtrl
    {
        auto getView() -> SPtr<view::MwView>;

        auto getModel() -> SPtr<model::MwModel>;

        auto loadGeometryFromFile(StringView path) -> void;

    public:
        MwCtrl(SPtr<fw::Executor> executor);

        auto init(WPtr<model::MwModel> model, WPtr<view::MwView> view) -> void;

        auto update() -> void;

        auto loadGeometryFromFileAsync(StringView path) -> void;

    private:
        SPtr<fw::Executor>  m_executor{};

        WPtr<view::MwView>  m_view{};
    };
}