#include <Turtle.h>
#include <Turtle/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


#include "EditorLayer.h"

namespace Turtle{
	class Tortuga : public Application
	{
	public:
		Tortuga()
			:Application("Tortuga")
		{
			PushLayer(new EditorLayer());
		}

		~Tortuga()
		{

		}

	};

	Application* CreateApplication()
	{
		return new Tortuga();
	}
}

