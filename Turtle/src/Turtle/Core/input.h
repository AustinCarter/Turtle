#pragma once

#include "Turtle/Core/Core.h"

namespace Turtle {
	class Input 
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float,float> GetMousePos();
	};
}