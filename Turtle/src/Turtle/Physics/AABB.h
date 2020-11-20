#pragma once

namespace Turtle {
	class AABB : Collider {
		public:
			AABB() = default;
			~ AABB() = default;

			glm::vec2 GetSize() {return m_Size;}
			void SetSize(glm::vec2 size) {m_Size = size;}
		private:
			glm::vec2 m_Size;
	}

}