#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Turtle/Core/Core.h"
#include "Turtle/Core/Log.h"

#include "Turtle/Debug/Instrumentor.h"	

#ifdef TURT_PLATFORM_WINDOWS
	#include <Windows.h>
#endif