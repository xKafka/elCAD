#pragma once

#include <system/TaskQueue.hpp>

#include <variant>
#include <condition_variable>

#include <model/assets/Geometry.hpp>

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
    namespace req
    {
        struct LoadGeometry
        {
            String path{};
        };

        using Requests = std::variant<LoadGeometry>;
    }

    namespace res
    {
        struct GeometryLoaded
        {
            Result<SPtr<model::Geometry>, String> geometryMaybe{};
        };

        using Responses = std::variant<GeometryLoaded>;
    }
   
    class MwWorker
    {
        auto switchRequest(const req::Requests& req) -> void;

        auto processRequest() -> void;

        auto loadGeometryFromFile(const req::LoadGeometry& req) -> void;

    public:
        MwWorker();

        auto addRequest(req::Requests&& request) -> void;

        auto tryResponse() -> Opt<res::Responses>;

        auto start() -> void;

    private:
        fw::AsyncQueue<req::Requests>   m_requests{};

        fw::AsyncQueue<res::Responses>  m_responses{};

        std::jthread                    m_thread{};

        std::atomic_bool                m_working{};
    };

    class MwCtrl
    {
        auto getView() -> SPtr<view::MwView>;

        auto getModel() -> SPtr<model::MwModel>;

        auto switchResponse(const res::Responses& resp) -> void;

        auto onGeometryLoaded(const res::GeometryLoaded& resp) -> void;

        auto reportError(StringView what) -> void;

        auto loadGeometryInView(SPtr<model::Geometry> geometry) -> void;

    public:
        MwCtrl();

        auto init(WPtr<model::MwModel> model, WPtr<view::MwView> view) -> void;

        auto poll() -> void;

        auto loadGeometryFromFileAsync(StringView path) -> void;

    private:
        WPtr<view::MwView>      m_view{};

        WPtr<model::MwModel>    m_model{};

        SPtr<MwWorker>          m_worker{};
    };
}