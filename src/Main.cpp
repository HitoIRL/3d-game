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

	Model model("assets/backpack.obj");
	Entity entity(model, { 0, 0, -3 }, { 0, 0, 0 }, 1);

	Renderer renderer(window->GetSize(), shaders);

	shaders->Bind(true);
	while (window->IsOpen()) {
		Input::Update();
		camera->Update();
		renderer.Render(entity, camera->GetViewMatrix());
	}
}
