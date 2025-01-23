#pragma once

#include <util/Math.hpp>
#include <util/Types.hpp>

namespace elcad::renderer
{
    class Camera
    {
        /**
         * @brief Updates the camera's view matrix based on the current position, target, and orientation.
         */
        auto updateViewMatrix() -> void;

        /**
         * @brief Updates the camera's projection matrix based on the current FOV, aspect ratio, and clipping planes.
         */
        auto updateProjectionMatrix() -> void;

        /**
         * @brief Clamps the pitch angle to avoid gimbal lock.
         *
         * @param pitch The pitch angle to clamp (in radians).
         * @return The clamped pitch angle.
         */
        auto clampPitch(f32 pitch) const->f32;

        /**
         * @brief Calculates the right vector of the camera's orientation.
         *
         * @return The normalized right vector.
         */
        auto calculateRightVector() const -> math::vec3;

        /**
         * @brief Calculates the up vector of the camera's orientation based on the right vector.
         *
         * @param right The right vector of the camera's orientation.
         * @return The normalized up vector.
         */
        auto calculateUpVector(const math::vec3& right) const->math::vec3;

        /**
         * @brief Calculates the camera's position based on its target, distance, pitch, and yaw.
         *
         * This function uses the spherical coordinates of the camera (pitch, yaw, and distance)
         * to calculate its Cartesian position relative to the target.
         *
         * @return The calculated position of the camera as a 3D vector.
         */
        auto calculatePosition() -> math::vec3;

        /**
         * @brief Updates the camera's internal matrices (view and projection).
         */
        auto update() -> void;

    public:
        Camera();

        Camera(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);

        /**
         * @brief Sets the target point the camera orbits around.
         *
         * @param target The new target position as a 3D vector.
         */
        auto setTarget(const math::vec3& target) -> void;

        /**
         * @brief Sets the distance of the camera from the target.
         *
         * @param distance The distance to set. Must be greater than zero.
         */
        auto setDistance(f32 distance) -> void;

        /**
          * @brief Sets the camera's orientation angles.
          *
          * @param pitch The vertical rotation angle (in radians).
          * @param yaw The horizontal rotation angle (in radians).
          */
        auto setAngles(f32 pitch, f32 yaw) -> void;

        /**
         * @brief Sets the field of view (FOV) of the camera in degrees.
         *
         * @param fov The new field of view in degrees.
         */
        auto setFov(f32 fov) -> void;

        /**
         * @brief Sets the aspect ratio for the camera's projection matrix.
         *
         * @param ratio The aspect ratio (width/height).
         */
        auto setAspectRatio(f32 ratio) -> void;

        /**
         * @brief Sets the near clipping plane for the camera's projection.
         *
         * @param nearPlane The near clipping distance.
         */
        auto setNearPlane(f32 nearPlane) -> void;

        /**
         * @brief Sets the far clipping plane for the camera's projection.
         *
         * @param farPlane The far clipping distance.
         */
        auto setFarPlane(f32 farPlane) -> void;

        /**
         * @brief Rotates the camera by the specified pitch and yaw deltas.
         *
         * @param deltaPitch The change in pitch angle (in radians).
         * @param deltaYaw The change in yaw angle (in radians).
         */
        auto rotate(f32 deltaPitch, f32 deltaYaw) -> void;

        /**
         * @brief Zooms the camera by modifying the distance to the target.
         *
         * @param deltaDistance The change in distance to the target. Positive values zoom out, negative values zoom in.
         */
        auto zoom(f32 deltaDistance) -> void;

        /**
         * @brief Pans the camera horizontally and vertically.
         *
         * @param deltaX The horizontal movement amount.
         * @param deltaY The vertical movement amount.
         */
        auto pan(f32 deltaX, f32 deltaY) -> void;

        /**
         * @brief Gets the view matrix representing the camera's orientation and position.
         *
         * @return A constant reference to the view matrix.
         */
        [[nodiscard]] auto getViewMatrix() const -> const math::mat4&;

        /**
         * @brief Gets the projection matrix representing the camera's projection settings.
         *
         * @return A constant reference to the projection matrix.
         */
        [[nodiscard]] auto getProjectionMatrix() const -> const math::mat4&;

    private:
        math::vec3  m_target{};

        f32         m_distance{}, m_pitch{}, m_yaw{};

        f32         m_fov{}, m_aspectRatio{}, m_nearPlane{}, m_farPlane{};

        math::vec3  m_position{};

        math::mat4  m_viewMatrix{};

        math::mat4  m_projectionMatrix{};
    };
}