#include "renderer/Camera.hpp"

namespace elcad::renderer
{
    Camera::Camera()
        : Camera{ 45.0f, 16.0f / 9.0f, 0.1f, 100.0f }
    {
    }

    Camera::Camera(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane)
        : m_target{ 0.0f, 0.0f, 0.0f }
        , m_distance{ 5.0f }
        , m_pitch{ math::radians(30.0f) }
        , m_yaw{ math::radians(45.0f) }
        , m_fov{ fov }
        , m_aspectRatio{ aspectRatio }
        , m_nearPlane{ nearPlane }
        , m_farPlane{ farPlane }
    {
        updateProjectionMatrix();

        updateViewMatrix();
    }

    // Setters
    auto Camera::setTarget(const math::vec3& target) -> void
    {
        m_target = target;

        updateViewMatrix();
    }

    auto Camera::setDistance(f32 distance) -> void
    {
        static constexpr f32 Min{ 0.1f };

        static constexpr f32 Max{ 100.0f };

        m_distance = math::clamp
        (
            distance, Min, Max
        );

        updateViewMatrix();
    }

    auto Camera::setAngles(f32 pitch, f32 yaw) -> void
    {
        m_pitch = clampPitch(pitch);

        m_yaw = yaw;

        updateViewMatrix();
    }

    auto Camera::setFov(f32 fov) -> void
    {
        m_fov = math::clamp
        (
            fov, 1.0f, 120.0f
        );

        updateProjectionMatrix();
    }

    auto Camera::setAspectRatio(f32 aspectRatio) -> void
    {
        m_aspectRatio = aspectRatio;

        updateProjectionMatrix();
    }

    auto Camera::setNearPlane(f32 nearPlane) -> void
    {
        m_nearPlane = nearPlane;

        updateProjectionMatrix();
    }

    auto Camera::setFarPlane(f32 farPlane) -> void
    {
        m_farPlane = farPlane;

        updateProjectionMatrix();
    }

    // Controls
    auto Camera::rotate(f32 deltaPitch, f32 deltaYaw) -> void
    {
        m_pitch = clampPitch
        (
            m_pitch + math::radians(deltaPitch)
        );

        m_yaw += math::radians(deltaYaw);

        updateViewMatrix();
    }

    auto Camera::zoom(f32 deltaDistance) -> void
    {
        setDistance(m_distance - deltaDistance);
    }

    auto Camera::pan(f32 deltaX, f32 deltaY) -> void
    {
        const auto right = calculateRightVector();

        const auto up = calculateUpVector(right);

        m_target += right * deltaX + up * deltaY;

        updateViewMatrix();
    }

    // Getters
    auto Camera::getViewMatrix() const -> const math::mat4&
    {
        return m_viewMatrix;
    }

    auto Camera::getProjectionMatrix() const -> const math::mat4&
    {
        return m_projectionMatrix;
    }

    auto Camera::updateViewMatrix() -> void
    {
        m_position = calculatePosition();

        m_viewMatrix = math::lookAt
        (
            m_position, m_target, { 0.0f, 1.0f, 0.0f }
        );
    }

    auto Camera::updateProjectionMatrix() -> void
    {
        m_projectionMatrix = math::perspective
        (
            math::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane
        );
    }

    auto Camera::clampPitch(f32 pitch) const -> f32
    {
        return math::clamp
        (
            pitch, -glm::half_pi<f32>(), glm::half_pi<f32>()
        );
    }

    auto Camera::calculateRightVector() const -> math::vec3
    {
        return math::normalize
        (
            math::cross(m_position - m_target, { 0.0f, 1.0f, 0.0f })
        );
    }

    auto Camera::calculateUpVector(const math::vec3& right) const -> math::vec3
    {
        return math::normalize
        (
            math::cross(right, m_position - m_target)
        );
    }

    auto Camera::calculatePosition() -> math::vec3
    {
        const auto pitchCos = math::cos(m_pitch);

        return math::vec3
        {
            m_target.x + m_distance * pitchCos * math::sin(m_yaw),
            m_target.y + m_distance * math::sin(m_pitch),
            m_target.z + m_distance * pitchCos * math::cos(m_yaw)
        };
    }
}