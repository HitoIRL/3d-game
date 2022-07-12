#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct WindowSettings {
	std::string_view title;
	glm::uvec2 size;
	bool fullscreen;
};

struct GLFWwindow;

class Window {
public:
	Window(const WindowSettings& settings);
	~Window();

	bool IsOpen() const;
private:
	GLFWwindow* _window;
};
