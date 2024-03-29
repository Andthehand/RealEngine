#include "repch.h"
#include "WindowsWindow.h"

#include "RealEngine/Core/Input.h"

#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/MouseEvent.h"
#include "RealEngine/Events/KeyEvent.h"

#include "RealEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace RealEngine {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		RE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		RE_PROFILE_FUNCTION();

		Init(props);
	}
	
	WindowsWindow::~WindowsWindow() {
		RE_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		RE_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Title = props.Title;

		RE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0) {
			RE_PROFILE_FUNCTION("glfwInit");
			int success = glfwInit();
			RE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		//MSAA
		glfwWindowHint(GLFW_SAMPLES, props.MMSA);

		{
			RE_PROFILE_FUNCTION("glfwCreateWindow");

			if (props.Maximized)
				glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		#if defined(RE_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		{
			RE_PROFILE_FUNCTION("Setting Window Callback");

			//Set Window Event Callback
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, false);;
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, true);
					data.EventCallback(event);
					break;
				}
				}
			});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
		}
	}

	void WindowsWindow::Shutdown() {
		RE_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);

		if (--s_GLFWWindowCount == 0) {
			RE_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}
	
	void WindowsWindow::OnUpdate() {
		RE_PROFILE_FUNCTION();
		
		{
			RE_PROFILE_FUNCTION("Polling events");
			glfwPollEvents();
		}
	}

	void WindowsWindow::OnRender() {
		RE_PROFILE_FUNCTION();

		m_Context->SwapBuffers();
	}
	
	void WindowsWindow::SetVSync(bool enabled) {
		RE_PROFILE_FUNCTION();

		glfwSwapInterval(enabled);

		m_Data.VSync = enabled;
	}

	void WindowsWindow::SetTitle(const std::string& title) {
		RE_PROFILE_FUNCTION();

		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, title.c_str());
	}
	
	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

	bool WindowsWindow::SetParent(const void* parentHandle) {
		return ::SetParent(glfwGetWin32Window(m_Window), (HWND)parentHandle);
	}
	
	void WindowsWindow::SetCursorEnabled(bool enabled) {
		glfwSetInputMode(m_Window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
}
