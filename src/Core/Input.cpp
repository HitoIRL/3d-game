#include "Input.hpp"

#include <GLFW/glfw3.h>

bool keys[GLFW_KEY_LAST];
float lastFrame, deltaTime;
std::vector<Input::CursorPanCallback> cursorPanCallbacks;

void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);
	keys[key] = action;
}

void CursorPositionCallback(GLFWwindow* window, double x, double y) {
	glm::vec2 position = { x, y };
	for (auto& callback : cursorPanCallbacks)
		callback(position);
}

void Input::Init(GLFWwindow* native) {
	glfwSetKeyCallback(native, KeyCallback);
	glfwSetCursorPosCallback(native, CursorPositionCallback);
}

void Input::Update() {
	auto currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Input::AddCursorPanCallback(const CursorPanCallback& callback) {
	cursorPanCallbacks.emplace_back(callback);
}

bool Input::IsKeyHeld(Key key) {
	return keys[static_cast<int>(key)] != GLFW_RELEASE;
}

float Input::GetDeltaTime() {
	return deltaTime;
}

float Input::GetElapsedTime() {
	return glfwGetTime();
}
