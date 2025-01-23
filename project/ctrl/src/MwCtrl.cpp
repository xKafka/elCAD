#include "MwCtrl.hpp"
#include <MwView.hpp>
#include <model/MwModel.hpp>

#include <model/importers/Importers.hpp>
#include <model/assets/Geometry.hpp>

#include <print>
#include <format>

namespace elcad::ctrl
{
    auto MwWorker::start() -> void
    {
        auto loop = [&]()
        {
            while (m_working)
            {
                processRequest();
            }
        };

        m_thread = std::jthread(loop);
    }

    auto MwWorker::addRequest(req::Requests&& request) -> void
    {
        m_requests.push
        (
            std::move(request)
        );
    }

    auto MwWorker::tryResponse() -> Opt<res::Responses>
    {
        return m_responses.tryPop();
    }

    auto MwWorker::loadGeometryFromFile(const req::LoadGeometry& req) -> void
    {
        try
        {
            auto geometry = model::importer::loadFromFile<model::Geometry>
            (
                req.path
            );

            m_responses.push
            (
                res::GeometryLoaded
                {
                    .geometryMaybe = geometry
                }
            );
        }
        catch (const std::exception& e)
        {
            m_responses.push
            (
                res::GeometryLoaded
                {
                    .geometryMaybe = std::unexpected{ e.what() }
                }
            );
        }
    }

    auto MwWorker::switchRequest(const req::Requests& req) -> void
    {
        std::visit
        (
            [self = this](const auto &var)
            {
                using T = std::decay_t<decltype(var)>;

                if constexpr (std::is_same_v<T, req::LoadGeometry>)
                {
                    self->loadGeometryFromFile(var);
                }
            },
            req
        );
    }

    auto MwWorker::processRequest() -> void
    {
        if (auto req = m_requests.blockingPop())
        {
            switchRequest(req.value());
        }
    }

    MwCtrl::MwCtrl()
        : m_worker{ makeShared<MwWorker>() }
    {

    }

    auto MwCtrl::init(WPtr<model::MwModel> model, WPtr<view::MwView> view) -> void
    {
        if (model.expired())
        {
            throw std::invalid_argument{ "model empty" };
        }

        m_model = model;

        if (view.expired())
        {
            throw std::invalid_argument{ "view empty" };
        }

        m_view = view;
    }

    auto MwCtrl::getView() -> SPtr<view::MwView>
    {
        if (m_view.expired())
        {
            throw std::runtime_error{ "view expired" };
        }

        return m_view.lock();
    }

    auto MwCtrl::getModel() -> SPtr<model::MwModel>
    {
        if (m_model.expired())
        {
            throw std::runtime_error{ "model expired" };
        }

        return m_model.lock();
    }

    auto MwCtrl::poll() -> void
    {
        for (auto resp{ m_worker->tryResponse() }; resp; resp = m_worker->tryResponse())
        {
            switchResponse(resp.value());
        }
    }

    auto MwCtrl::switchResponse(const res::Responses& resp) -> void
    {
        std::visit
        (
            [self = this](auto &&var)
            {
                using T = std::decay_t<decltype(var)>;

                if constexpr (std::is_same_v<T, res::GeometryLoaded>)
                {
                    self->onGeometryLoaded(var);
                }
            },
            resp
        );
    }

    auto MwCtrl::loadGeometryFromFileAsync(StringView path) -> void
    {
        m_worker->addRequest
        (
            req::LoadGeometry
            {
                .path = String{ path }
            }
        );
    }

    auto MwCtrl::onGeometryLoaded(const res::GeometryLoaded& resp) -> void
    {
        if (resp.geometryMaybe)
        {
            loadGeometryInView
(
                resp.geometryMaybe.value()
            );
        }
        else
        {
            reportError
            (
                resp.geometryMaybe.error()
            );

            std::print("{}", resp.geometryMaybe.error());
        }
    }

    auto MwCtrl::reportError(StringView what) -> void
    {
        std::print("{}", what);
    }

    auto MwCtrl::loadGeometryInView(SPtr<model::Geometry> geometry) -> void
    {
        std::print("{}", geometry->getName());
    }
}