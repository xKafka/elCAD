#pragma once

#include <functional>

#include "VTypes.hpp"

namespace elcad::renderer
{
	class VAllocationCallbacks : public vk::AllocationCallbacks
	{
	public:
		VAllocationCallbacks();
	};
}
