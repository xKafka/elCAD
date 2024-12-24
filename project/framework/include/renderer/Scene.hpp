#pragma once

#include <util/Types.hpp>

#include <variant>

#include "Camera.hpp"

namespace elcad::renderer
{
    class CameraNode
    {
    public:

    private:
        UPtr<Camera> m_camera{};
    };

    class RenderNode
    {

    };

    using SceneNode = std::variant<CameraNode, RenderNode>;

    class Scene
    {
    public:
        auto getNodes() const -> Span<const SceneNode>;

    private:
        Vec<SPtr<SceneNode>> m_nodes{};
      
    };
}