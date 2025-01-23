#include "model/SystemPaths.hpp"

#include <util/Types.hpp>
#include <util/Filesystem.hpp>

namespace elcad::model
{
namespace path
{
	auto getSystemRootPath() -> StringView
	{
		static const String RootPath
		{
			fs::current_path().generic_string() 
		};

		return RootPath;
	}

	auto getResourcePath(StringView path) -> String
	{
		return cfm("{}/resources/{}", getSystemRootPath(), path);
	}
}
}