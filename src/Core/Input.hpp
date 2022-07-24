#pragma once

enum class Key {
	W = 87,
	S = 83,
	A = 65,
	D = 68,
};

namespace Input {
	void Update();
	void KeyCallback(int key, int action);
	//void CursorPositionCallback(float x, float y);

	bool IsKeyHeld(Key key);
	float GetDeltaTime();
}
