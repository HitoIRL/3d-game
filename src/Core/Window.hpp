#pragma once

#include <functional>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct GLFWwindow;

using keyCb = std::function<void(int, int)>;
using cursorPosCb = std::function<void(float, float)>;

class Window {
public:
	Window(std::string_view title, const glm::uvec2& size);
	~Window();

	bool IsOpen() const;

	void SetKeyCallback(const keyCb& callback);
	void SetCursorPosCallback(const cursorPosCb& callback);
	const glm::uvec2& GetSize() const { return size; }
private:
	GLFWwindow* _window;
	glm::uvec2 size;
};
