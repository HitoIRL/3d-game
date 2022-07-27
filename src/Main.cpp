#include <filesystem>

#include "Debug/Log.hpp"
#include "Core/Window.hpp"
#include "Core/Renderer.hpp"
#include "Core/Input.hpp"
#include "Core/Camera.hpp"

int main() {
	Log::Init();
	auto cwd = std::filesystem::current_path();
	LOG_INFO("Current working directory: {}", cwd.string());

	auto window = std::make_unique<Window>("3d game es", glm::vec2(1280, 720));
	Input::Init(window->GetNative());

	auto shaders = std::make_shared<Shaders>("assets/vertex.glsl", "assets/fragment.glsl");
	auto camera = std::make_shared<Camera>();

	RawModel model({
			{{-0.5f,0.5f,-0.5f}, {0,0}},
			{{-0.5f,-0.5f,-0.5f}, {0,1}},
			{{0.5f,-0.5f,-0.5f}, {1, 1}},
			{{0.5f,0.5f,-0.5f}, {1,0}},

			{{-0.5f,0.5f,0.5f}, {0,0}},
			{{-0.5f,-0.5f,0.5f}, {0,1}},
			{{0.5f,-0.5f,0.5f}, {1, 1}},
			{{0.5f,0.5f,0.5f}, {1,0}},

			{{0.5f,0.5f,-0.5f}, {0,0}},
			{{0.5f,-0.5f,-0.5f}, {0,1}},
			{{0.5f,-0.5f,0.5f}, {1, 1}},
			{{0.5f,0.5f,0.5f}, {1,0}},

			{{-0.5f,0.5f,-0.5f}, {0,0}},
			{{-0.5f,-0.5f,-0.5f}, {0,1}},
			{{-0.5f,-0.5f,0.5f}, {1, 1}},
			{{-0.5f,0.5f,0.5f}, {1,0}},

			{{-0.5f,0.5f,0.5f}, {0,0}},
			{{-0.5f,0.5f,-0.5f}, {0,1}},
			{{0.5f,0.5f,-0.5f}, {1, 1}},
			{{0.5f,0.5f,0.5f}, {1,0}},

			{{-0.5f,-0.5f,0.5f}, {0,0}},
			{{-0.5f,-0.5f,-0.5f}, {0,1}},
			{{0.5f,-0.5f,-0.5f}, {1, 1}},
			{{0.5f,-0.5f,0.5f}, {1,0}}
	}, {
			0,1,3,
			3,1,2,
			4,5,7,
			7,5,6,
			8,9,11,
			11,9,10,
			12,13,15,
			15,13,14,
			16,17,19,
			19,17,18,
			20,21,23,
			23,21,22
	});
	Texture texture("assets/bomba.png");
	TexturedModel texturedModel(model, texture);
	Entity entity(texturedModel, { 0, 0, -3 }, { 0, 0, 0 }, 1);
	Renderer renderer(window->GetSize(), shaders);

	while (window->IsOpen()) {
		Input::Update();
		camera->Update();
		shaders->SetMat4("_view", camera->GetViewMatrix());
		renderer.Render(entity);
	}
}
