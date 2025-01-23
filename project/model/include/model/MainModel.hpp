#pragma once

#include <util/Types.hpp>
#include <system/Executor.hpp>

#include "MwModel.hpp"

namespace elcad::model
{
    class MainModel
    {
    public:
        MainModel(SPtr<fw::Executor> executor);

        auto init() -> void;

        [[nodiscard]] auto getMwModel() -> SPtr<MwModel>;

    private:
        SPtr<fw::Executor>  m_executor{};

        SPtr<MwModel>       m_mwModel{};
    };
}