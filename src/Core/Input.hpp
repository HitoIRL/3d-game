#pragma once

#include <functional>

#include <glm/vec2.hpp>

enum class Key {
	W = 87,
	S = 83,
	A = 65,
	D = 68,
};

struct GLFWwindow;

namespace Input {
	using CursorPanCallback = std::function<void(glm::vec2)>;

	void Init(GLFWwindow* native);
	void Update();

	void AddCursorPanCallback(const CursorPanCallback& callback);

	bool IsKeyHeld(Key key);
	float GetDeltaTime();
}
