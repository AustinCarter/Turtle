#pragma once


#include "Turtle/Scene/Entity.h"
#include "Turtle/Scene/Scene.h"

namespace Turtle {

	class ComponentSelectPanel
	{
	public:
		ComponentSelectPanel() = default;

		void Display();
		void Close();

		bool Active() {return m_Active;}
		bool HasSelected() {return m_ComponentSelected;}
		int GetComponentSelection() {return m_ComponentType;}

		void Open() 
			{ m_ComponentSelected = false; m_ComponentType = -1; m_Active = true;}
	private:
		bool m_ComponentSelected = false, m_Active = false;
		int m_ComponentType;
	};
}