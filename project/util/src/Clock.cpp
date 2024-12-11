#include "Clock.hpp"

namespace elcad::util
{
    Clock::Clock()
        : m_running{ false }
        , m_start{ }
        , m_elapsed{ std::chrono::duration<f64>{ 0.0 } }
    {
    }

    auto Clock::update() -> f64
    {
        if (m_running)
        {
            m_elapsed = std::chrono::duration<f64>{ ClockType::now() - m_start };

            return m_elapsed.count();
        }

        return 0.0f;
    }

    auto Clock::start() -> void
    {
        m_start = ClockType::now();

        m_elapsed = std::chrono::duration<f64>{ 0.0 };

        m_running = true;
    }

    auto Clock::stop() -> void
    {
        m_running = false;
    }
}