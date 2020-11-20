#pragma once

namespace Turtle {
	class CollisonManager
	{
	public:
		CollisonManager();
		~CollisonManager();
		
	private:
		std::vector<Collider> m_Colliders;
	};
}