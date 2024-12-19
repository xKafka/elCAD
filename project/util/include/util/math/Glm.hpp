#pragma once

#pragma warning (push, 0)

#ifndef GLM_FORCE_PURE
#define GLM_FORCE_PURE
#endif

#ifndef GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_LEFT_HANDED
#endif

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>

#pragma warning (pop)

namespace elcad
{
	namespace math = glm;
}