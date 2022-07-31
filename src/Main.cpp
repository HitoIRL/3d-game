#include <filesystem>

#include <glm/gtc/matrix_transform.hpp>

#include "Debug/Log.hpp"
#include "Core/Window.hpp"
#include "Core/Renderer.hpp"
#include "Core/Input.hpp"
#include "Core/Camera.hpp"

const glm::vec3 PointLightPositions[] = {
		glm::vec3( 0.7f,  0.2f,  2.0f),
		glm::vec3( 2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -3.0f)
};

int main() {
	Log::Init();
	auto cwd = std::filesystem::current_path();
	LOG_INFO("Current working directory: {}", cwd.string());

	auto window = std::make_unique<Window>("3d game es", glm::vec2(1280, 720));
	Input::Init(window->GetNative());

	auto cubeShaders = std::make_shared<Shaders>("assets/shaders/cube.vs", "assets/shaders/cube.fs");
	auto lightShaders = std::make_shared<Shaders>("assets/shaders/light.vs", "assets/shaders/light.fs");

	auto camera = std::make_shared<Camera>();

	auto model = std::make_shared<Model>("assets/backpack/backpack.obj");
	std::vector<Entity> entities;
	for (auto i = 0u; i < 6; i++)
		entities.emplace_back(model, glm::vec3(i * 5, 0, -5), glm::vec3(0), 1.0f);
		//Entity entity(model, { 0, 0, -5 }, { 0, 0, 0 }, 1.0f);

	Renderer renderer(window->GetSize(), cubeShaders);

	cubeShaders->Bind(true);
	cubeShaders->SetInt("material.diffuse", 0);
	cubeShaders->SetInt("material.specular", 1);
	cubeShaders->SetFloat("material.shininess", 32.0f);

	cubeShaders->SetVec3("dirLight.direction", { -0.2f, -1.0f, -0.3f });
	cubeShaders->SetVec3("dirLight.ambient", { 0.05f, 0.05f, 0.05f });
	cubeShaders->SetVec3("dirLight.diffuse", { 0.4f, 0.4f, 0.4f });
	cubeShaders->SetVec3("dirLight.specular", { 0.5f, 0.5f, 0.5f });

	for (auto i = 0u; i < 4; i++) {
		std::string prefix = std::format("pointLights[{}].", std::to_string(i));
		cubeShaders->SetVec3(prefix+"position", PointLightPositions[i]);
		cubeShaders->SetVec3(prefix+"ambient", { 0.05f, 0.05f, 0.05f });
		cubeShaders->SetVec3(prefix+"diffuse", { 0.8f, 0.8f, 0.8f });
		cubeShaders->SetVec3(prefix+"specular", { 1.0f, 1.0f, 1.0f });
		cubeShaders->SetFloat(prefix+"constant", 1.0f);
		cubeShaders->SetFloat(prefix+"linear", 0.09f);
		cubeShaders->SetFloat(prefix+"quadratic", 0.032f);
	}

	cubeShaders->SetVec3("spotLight.ambient", { 0.0f, 0.0f, 0.0f });
	cubeShaders->SetVec3("spotLight.diffuse", { 1.0f, 1.0f, 1.0f });
	cubeShaders->SetVec3("spotLight.specular", { 1.0f, 1.0f, 1.0f });
	cubeShaders->SetFloat("spotLight.constant", 1.0f);
	cubeShaders->SetFloat("spotLight.linear", 0.09f);
	cubeShaders->SetFloat("spotLight.quadratic", 0.032f);
	cubeShaders->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	cubeShaders->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	//cubeShaders->Bind(false);

	while (window->IsOpen()) {
		Input::Update();
		camera->Update();

		/*
		lightShaders->Bind(true);
		for (auto& pointLight : PointLightPositions) {
			light.SetPosition(pointLight);
			auto mvp = renderer.GetProjectionMatrix() * camera->GetViewMatrix() * light.GetModelMatrix();
			lightShaders->SetMat4("mvp", mvp);
			light.Draw();
		}
		lightShaders->Bind(false);*/

		//cubeShaders->Bind(true);
		cubeShaders->SetVec3("viewPos", camera->GetPosition());
		cubeShaders->SetVec3("spotLight.position", camera->GetPosition());
		cubeShaders->SetVec3("spotLight.direction", camera->GetFront());
		for (auto& entity : entities)
			renderer.Add(entity);

		renderer.Render(camera->GetViewMatrix());
		//cubeShaders->Bind(false);
	}
}
