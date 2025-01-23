#pragma once

#include <util/Types.hpp>
#include <system/Executor.hpp>

namespace elcad::

namespace elcad::model
{
    class MwModel
    {
        auto loadGeometryFromFileAsync(StringView path) -> void;

    public:
        MwModel(SPtr<fw::Executor> executor);

        auto init(WPtr<ctrl::) -> void;

        auto loadGeometryFromFileAsync(StringView path) -> void;

    private:
        SPtr<fw::Executor>  m_executor{};
    };
}