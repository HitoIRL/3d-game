#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "../Debug/Log.hpp"

// todo: move it somewhere else
const std::unordered_map<GLenum, std::string> errorSourceMap {
		{GL_DEBUG_SOURCE_API, "SOURCE_API"},
		{GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM"},
		{GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER"},
		{GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY"},
		{GL_DEBUG_SOURCE_APPLICATION, "APPLICATION"},
		{GL_DEBUG_SOURCE_OTHER, "OTHER"}
};

const std::unordered_map<GLenum, std::string> errorTypeMap {
		{GL_DEBUG_TYPE_ERROR, "ERROR"},
		{GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
		{GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
		{GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
		{GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
		{GL_DEBUG_TYPE_OTHER, "OTHER"},
		{GL_DEBUG_TYPE_MARKER, "MARKER"}
};

const std::unordered_map<GLenum, std::string> severityMap {
		{GL_DEBUG_SEVERITY_HIGH, "HIGH"},
		{GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
		{GL_DEBUG_SEVERITY_LOW, "LOW"},
		//{GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}
};

void __stdcall DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	auto src = errorSourceMap.at(source);
	auto tp = errorTypeMap.at(type);
	auto sv = severityMap.at(severity);
	LOG_WARN("GL: {} type: {} severity: {} message: {}", src, tp, sv, message);
}

Window::Window(std::string_view title, const glm::uvec2& size) : size(size) {
	if (!glfwInit()) {
		LOG_ERROR("GLFW library iitialization failed");
		std::exit(EXIT_FAILURE);
	}

	// we want latest available version of gl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
	if (!_window) {
		LOG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1); // toggles vsync

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	glViewport(0, 0, size.x, size.y);
	glEnable(GL_DEPTH_TEST);

	// enabling debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallback, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}

Window::~Window() {
	glfwTerminate();
}

bool Window::IsOpen() const {
	glfwSwapBuffers(_window);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();

	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, 1);

	return !glfwWindowShouldClose(_window);
}

const glm::uvec2& Window::GetSize() const {
	return size;
}
