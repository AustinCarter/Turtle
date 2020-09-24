#include "turtpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Turtle{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		TURT_CORE_ASSERT(windowHandle, "Window hanlde is null.");
	}

	void OpenGLContext::Init()
	{
		TURT_PROFILE_FUNCTION();
		
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TURT_CORE_ASSERT(status, "Failed to initialize GLAD");

		TURT_CORE_INFO("OpenGL Info: ");
		TURT_CORE_INFO("  Vendor: {0}",glGetString(GL_VENDOR));
		TURT_CORE_INFO("  Renderer: {0}",glGetString(GL_RENDERER));
		TURT_CORE_INFO("  Version: {0}",glGetString(GL_VERSION));


	}

	void OpenGLContext::SwapBuffers()
	{
		TURT_PROFILE_FUNCTION();
		
		glfwSwapBuffers(m_windowHandle);
	}

}
