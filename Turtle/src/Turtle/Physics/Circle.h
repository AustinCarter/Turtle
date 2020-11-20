#pragma once

#include "Turtle/Physics/Collider.h"

namespace Turtle {a

	class Circle : Collider
	{
	public:
		Circle() = default;
		~Circle() = default;

		float GetRadius() {return m_Radius;}
		void SetRadius(float radius) {m_Radius = radius;}
	private:
		float m_Radius;
	};

} // Turtle