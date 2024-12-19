#pragma once

#include <string>
#include <memory>

#include <util/Types.hpp>
#include <util/Time.hpp>

#include <renderer/Frontend.hpp>

namespace elcad::view
{
    class MwView;
}

namespace elcad::ctrl
{
    class MwController
    {
        auto view() -> SPtr<view::MwView>;

    public:
        MwController();

        auto init(SPtr<view::MwView> mwView) -> void;

        auto run() -> void;

        auto shutdown() -> void;

        auto resize(u32 width, u32 height) -> void;
        
        auto renderSomething() -> void;

    private:
        time::Clock                 m_clock{};

        SPtr<view::MwView>          m_mwView{};

        UPtr<renderer::Frontend>    m_renderer{};
    };
}