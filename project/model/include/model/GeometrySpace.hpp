#pragma once

#include <util/Types.hpp>
#include <util/Math.hpp>

#include <system/Executor.hpp>

#include "MwModel.hpp"

#include <unordered_map>
#include <unordered_set>

namespace elcad::model
{
    namespace event
    {

    }

    class CtrlWindow
    {
    public:

    private:
    };

    class RenderWindow
    {
    public:

    private:
    };












    namespace event
    {
        struct GeometryClicked
        {
            u32 id;
        };

        struct Resized
        {
            f32 width{}, height{};
        };

        using E = std::variant<Resized, GeometryClicked>;
    }

    class CtrlSpace
    {
    public:
        CtrlSpace(Model* parent);

        auto addGeometry() -> void
        {
            m_parent->addGeometry();
        }

        auto update() -> void
        {
            for (const auto& e : m_events)
            {
                std::visit
                (

                )
            }
        }

    private:
        Vec<event::E>   m_events{};
    };

    template<typename E>
    class EventBus
    {
    public:
        auto publish() -> void
        {

        }

        auto consume() -> void
        {

        }

    private:
        std::queue<E> m_events{};
    };

    class RenderSpace
    {
    public:
        auto update() -> void
        {
            for (const auto& e : m_events)
            {

            }
        }

    private:
        UPtr<EventBus> 

        std::unordered_map<u32, SPtr<GeometryGpu>>  m_geometriesOnGpu{};

        Vec<Event>                                  m_events{};
    };

    class Render

    class GeometryRendererSpace
    {
        void uploadToGpu(u32 id, const SPtr<Geometry>& geometry)
        {
            // Upload geometry to GPU
        }

        void addToRenderQueue(u32 id)
        {
            m_toRender[id] = m_geometriesGpu[id];
        }

        void removeFromRenderQueue(u32 id)
        {
            m_toRender.erase(id);
        }

    public:
        GeometryRendererSpace();

        auto stageGeometry(u32 id, SPtr<const Geometry> geometry) -> void
        {
            m_toGpuStage.insert(id, geometry);
        }

        auto removeGeometry(u32 id) -> void
        {
            m_toRemove.emplace_back(id);
        }

        auto hideGeometry(u32 id) -> void
        {
            m_toHide.emplace_back(id);
        }

        auto showGeometry(u32 id) -> void
        {
            m_toShow.emplace_back(id);
        }

        auto update() -> void
        {
            for (const auto& [id, stage] : m_toGpuStage)
            {
                auto geometryGpu = makeShared<GeometryGpu>(stage);

                m_geometriesGpu.insert(id, geometryGpu);

                m_toRender.insert(id, geometryGpu);
            }

            m_toGpuStage.clear();

            for (const auto toHide : m_toHide)
            {
                m_toRemove.remove(toHide);
            }

            m_toHide.clear();

            for (const auto toShow : m_toShow)
            {
                m_toRender.insert
                (
                    toShow, m_geometriesGpu[toShow]
                );
            }

            m_toShow.clear();

            for (const auto toRemove : m_toRemove)
            {
                m_geometriesGpu.erase(toRemove);

                m_toRender.erase(toRemove);
            }

            m_toRemove.clear();
        }

    private:
        std::unordered_map<u32, SPtr<const Geometry>>   m_toGpuStage{};

        std::unordered_map<u32, SPtr<GeometryGpu>>      m_geometriesGpu{};

        std::unordered_map<u32, SPtr<GeometryGpu>>      m_toRender{};

        Vec<u32>                                        m_toHide{};

        Vec<u32>                                        m_toShow{};

        Vec<u32>                                        m_toRemove{};
    };

    class GeometryModel
    {
    public:
        auto addGeometry(u32 id, SPtr<Geometry> geometry) -> void
        {
            m_ctrlSpace->addGeometry(id, geometry);

            m_renderSpace->addGeometry(id, geometry);
        }

        auto removeGeometry(u32 id) -> void
        {
            m_ctrlSpace->removeGeometry(id);

            m_renderSpace->removeGeometry(id);
        }

        auto hideGeometry(u32 id) -> void
        {

        }

    private:
        std::unordered_map<u32, SPtr<Geometry>>     m_geometries{};

        SPtr<GeometryRendererSpace>                 m_renderSpace{};
    };
}

