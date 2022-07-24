#include "Input.hpp"

#include <GLFW/glfw3.h>

#include "../Debug/Log.hpp"

bool keys[GLFW_KEY_LAST];
float lastFrame, deltaTime;

void Input::Update() {
	auto currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Input::KeyCallback(int key, int action) {
	keys[key] = action;
}

//void Input::CursorPositionCallback(float x, float y) {
//	float xOffset = x - lastX;
//	float yOffset = lastY - y;
//	lastX = x;
//	lastY = y;
//
//	const float sensitivity = 0.1f;
//	xOffset *= sensitivity;
//	yOffset *= sensitivity;
//}

bool Input::IsKeyHeld(Key key) {
	return keys[static_cast<int>(key)] != GLFW_RELEASE;
}

float Input::GetDeltaTime() {
	return deltaTime;
}
