#pragma once

#include "RealEngine/Core/PlatformDetection.h"

#ifdef RE_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <optional>
#include <algorithm>
#include <functional>

#include <sstream>
#include <array>
#include <string>
#include <vector>
#include <unordered_set>

#include "RealEngine/Core/Core.h"

#include "RealEngine/Core/Log.h"

#include "RealEngine/Debug/Instrumentor.h"

#ifdef RE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif 
