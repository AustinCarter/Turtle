#include "turtpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Turtle {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}