#include <filesystem>
#include <iostream>

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

	auto cubeShaders = std::make_shared<Shaders>("assets/shaders/cube.vs", "assets/shaders/cube.fs");
	auto camera = std::make_shared<Camera>();

	auto model = std::make_shared<Model>("assets/stall/stall.obj");
	std::vector<Entity> entities;
	for (auto i = 0u; i < 6; i++)
		entities.emplace_back(model, glm::vec3(i * 10, 0, -5), glm::vec3(0), 1.0f);
	Texture stallTexture("assets/stall/stallTexture.png");
	Texture terrainTexture("assets/grass.png");
	Renderer renderer(window->GetSize(), cubeShaders);

	cubeShaders->Bind(true);

	int samplers[16];
	for (auto i = 0; i < 16; i++)
		samplers[i] = i;
	cubeShaders->SetIntArray("samplers[0]", samplers);

	Terrain terrain1({ 0, 0 }, terrainTexture);

	terrainTexture.Bind(0);
	stallTexture.Bind(1);
	while (window->IsOpen()) {
		Input::Update();
		camera->Update();

		for (auto& entity : entities)
			renderer.Add(entity);

		renderer.Add(terrain1);
		renderer.Render(camera->GetViewMatrix());
	}
}
