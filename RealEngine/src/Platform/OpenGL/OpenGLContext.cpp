#include "repch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace RealEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		RE_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init() {
		RE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RE_CORE_ASSERT(status, "Failed to initialize Glad!");

		RE_CORE_INFO("OpenGL Info:");
		RE_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		RE_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		RE_CORE_INFO("  Version: {0}", (const char*) glGetString(GL_VERSION));
	
		RE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "RealEngine requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers() {
		RE_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
