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

	auto cubeShaders = std::make_shared<Shaders>("assets/shaders/cube.vs", "assets/shaders/cube.fs");
	auto camera = std::make_shared<Camera>();

	auto stallTexture = TextureManager::CreateTexture("assets/stall/stallTexture.png");
	auto terrainTexture = TextureManager::CreateTexture("assets/grass.png");
	auto grassTexture = TextureManager::CreateTexture("assets/grassTexture.png");

	auto stallModel = std::make_shared<Model>("assets/stall/stall.obj", stallTexture);
	std::vector<Entity> entities;
	for (auto i = 0u; i < 6; i++)
		entities.emplace_back(stallModel, glm::vec3(10 * i, 0, 0), glm::vec3(0), 1.0f);

	Terrain terrain1({ 0, 0 }, terrainTexture);

	auto grassModel = std::make_shared<Model>("assets/grassModel.obj", grassTexture);
	Entity grassEntity(grassModel, { 40, 0, 0 }, glm::vec3(0), 1.0f);

	Renderer renderer(window->GetSize(), cubeShaders);

	cubeShaders->Bind(true);

	int samplers[16];
	for (auto i = 0; i < 16; i++)
		samplers[i] = i;
	cubeShaders->SetIntArray("samplers[0]", samplers);

	while (window->IsOpen()) {
		Input::Update();
		camera->Update();

		for (auto& entity : entities)
			renderer.Add(entity);

		renderer.Add(grassEntity);

		renderer.Add(terrain1);
		renderer.Render(camera->GetViewMatrix());
	}
}
