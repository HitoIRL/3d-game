#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

Window::Window(const WindowSettings& settings) {
	glfwInit();

	// opengl core 4.6
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(settings.size.x, settings.size.y, settings.title.data(), settings.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1); // toggles vsync

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	glViewport(0, 0, settings.size.x, settings.size.y);
}

Window::~Window() {
	glfwTerminate();
}

void Window::SetBackgroundColor(const glm::vec4& color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

bool Window::IsOpen() const {
	glfwSwapBuffers(_window);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwPollEvents();

	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, 1);

	return !glfwWindowShouldClose(_window);
}
