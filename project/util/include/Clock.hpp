#pragma	once

#include "Types.h"

#include <chrono>

namespace elcad::util
{
	class Clock
	{
		using ClockType = std::chrono::high_resolution_clock;

	public:
		Clock();

		// Updates the provided clock. Should be called just before checking elapsed time.
		auto update() -> f64;

		// Starts the provided clock. Resets elapsed time.
		auto start() -> void;

		// Stops the provided clock. Does not reset elapsed time.
		auto stop() -> void;

	private:
		bool								m_running{};

		std::chrono::time_point<ClockType>	m_start{};

		std::chrono::duration<f64>			m_elapsed{};
	};
}