#include <filesystem>
#include <format>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Debug/Log.hpp"
#include "Core/Window.hpp"
#include "Core/Renderer.hpp"
#include "Core/Shaders.hpp"
#include "Core/Input.hpp"
#include "Core/Camera.hpp"

const std::vector<Vertex> CubeVertices = {
		{{-0.5f,0.5f,-0.5f}, {0.0f, 0.0f, -1.0f}, { 0.0f, 1.0f }},
		{{-0.5f,-0.5f,-0.5f}, {0.0f, 0.0f, -1.0f}, { 0.0f, 0.0f }},
		{{0.5f,-0.5f,-0.5f}, {0.0f, 0.0f, -1.0f}, { 1.0f, 0.0f }},
		{{0.5f,0.5f,-0.5f}, {0.0f, 0.0f, -1.0f}, { 1.0f, 1.0f }},

		{{-0.5f,0.5f,0.5f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f }},
		{{-0.5f,-0.5f,0.5f}, {0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f }},
		{{0.5f,-0.5f,0.5f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f }},
		{{0.5f,0.5f,0.5f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 1.0f }},

		{{0.5f,0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f }},
		{{0.5f,-0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, { 0.0f, 1.0f }},
		{{0.5f,-0.5f,0.5f}, {1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f }},
		{{0.5f,0.5f,0.5f}, {1.0f, 0.0f, 0.0f}, { 1.0f, 0.0f }},

		{{-0.5f,0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f }},
		{{-0.5f,-0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, { 0.0f, 1.0f }},
		{{-0.5f,-0.5f,0.5f}, {-1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f }},
		{{-0.5f,0.5f,0.5f}, {-1.0f, 0.0f, 0.0f}, { 1.0f, 0.0f }},

		{{-0.5f,0.5f,0.5f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f }},
		{{-0.5f,0.5f,-0.5f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 1.0f }},
		{{0.5f,0.5f,-0.5f}, {0.0f, 1.0f, 0.0f}, { 1.0f, 1.0f }},
		{{0.5f,0.5f,0.5f}, {0.0f, 1.0f, 0.0f}, { 1.0f, 0.0f }},

		{{-0.5f,-0.5f,0.5f}, {0.0f, -1.0f, 0.0f}, { 0.0f, 0.0f }},
		{{-0.5f,-0.5f,-0.5f}, {0.0f, -1.0f, 0.0f}, { 0.0f, 1.0f }},
		{{0.5f,-0.5f,-0.5f}, {0.0f, -1.0f, 0.0f}, { 1.0f, 1.0f }},
		{{0.5f,-0.5f,0.5f}, {0.0f, -1.0f, 0.0f}, { 1.0f, 0.0f }}
};

const std::vector<std::uint32_t> CubeIndices = {
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
};

class Cube {
public:
	Cube(const glm::vec3& position, float scale) : position(position), scale(scale) {
		// vbo & ibo
		const auto verticesSize = CubeVertices.size() * sizeof(Vertex);
		const auto indicesSize = CubeIndices.size() * sizeof(std::uint32_t);

		indicesOffset = verticesSize;

		glCreateBuffers(1, &buffer);
		glNamedBufferStorage(buffer, verticesSize + indicesSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

		glNamedBufferSubData(buffer, 0, verticesSize, CubeVertices.data());
		glNamedBufferSubData(buffer, indicesOffset, indicesSize, CubeIndices.data());

		// vao
		glCreateVertexArrays(1, &vao);

		// vertex buffer layout
		auto positionAttrib = 0u;//shaders->GetAttribLocation("position");
		glEnableVertexArrayAttrib(vao, positionAttrib);
		glVertexArrayAttribFormat(vao, positionAttrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
		glVertexArrayAttribBinding(vao, positionAttrib, 0);

		auto normalAttrib = 1u;//shaders->GetAttribLocation("normal");
		glEnableVertexArrayAttrib(vao, normalAttrib);
		glVertexArrayAttribFormat(vao, normalAttrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
		glVertexArrayAttribBinding(vao, normalAttrib, 0);

		auto texCoordAttrib = 2u;//shaders->GetAttribLocation("texCoord");
		glEnableVertexArrayAttrib(vao, texCoordAttrib);
		glVertexArrayAttribFormat(vao, texCoordAttrib, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
		glVertexArrayAttribBinding(vao, texCoordAttrib, 0);

		// attach buffers
		glVertexArrayVertexBuffer(vao, 0, buffer, 0, sizeof(Vertex));
		glVertexArrayElementBuffer(vao, buffer);
	}
	~Cube() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &buffer);
	}

	void Draw() const {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, CubeIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(indicesOffset));
		glBindVertexArray(0);
	}

	glm::mat4 GetModelMatrix() const {
		glm::mat4 temp(1);
		temp = glm::translate(temp, position);
		temp = glm::scale(temp, glm::vec3(scale));
		return temp;
	}
	const glm::vec3& GetPosition() const { return position; }
	void SetPosition(const glm::vec3& v) { position = v; }
private:
	std::uint32_t vao, buffer, indicesOffset;
	glm::vec3 position;
	const float scale;
};

const glm::vec3 CubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
};

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

	//auto shaders = std::make_shared<Shaders>("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
	auto cubeShaders = std::make_shared<Shaders>("assets/shaders/cube.vs", "assets/shaders/cube.fs");
	auto lightShaders = std::make_shared<Shaders>("assets/shaders/light.vs", "assets/shaders/light.fs");
	auto outlineShaders = std::make_shared<Shaders>("assets/shaders/outline.vs", "assets/shaders/outline.fs");

	auto camera = std::make_shared<Camera>();

	Model model("assets/backpack/backpack.obj");
	Entity entity(model, { 0, 0, -5 }, { 0, 0, 0 }, 0.9f);
	Entity outline(model, { 0, 0, -5 }, { 0, 0, 0 }, 1.0f);

	Renderer renderer(window->GetSize(), cubeShaders);

	Cube light({ 1.2, 1.0f, 2.0f }, 0.2f);

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

	cubeShaders->Bind(false);

	while (window->IsOpen()) {
		Input::Update();
		camera->Update();

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0x00);

		lightShaders->Bind(true);
		for (auto& pointLight : PointLightPositions) {
			light.SetPosition(pointLight);
			auto mvp = renderer.GetProjectionMatrix() * camera->GetViewMatrix() * light.GetModelMatrix();
			lightShaders->SetMat4("mvp", mvp);
			light.Draw();
		}
		lightShaders->Bind(false);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		cubeShaders->Bind(true);
		cubeShaders->SetVec3("viewPos", camera->GetPosition());
		cubeShaders->SetVec3("spotLight.position", camera->GetPosition());
		cubeShaders->SetVec3("spotLight.direction", camera->GetFront());
		cubeShaders->SetMat4("model", entity.GetModelMatrix());
		renderer.Render(entity, camera->GetViewMatrix());
		cubeShaders->Bind(false);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		outlineShaders->Bind(true);
		{
			auto& tempModel = outline.GetModel();
			auto mvp = renderer.GetProjectionMatrix() * camera->GetViewMatrix() * outline.GetModelMatrix();
			outlineShaders->SetMat4("mvp", mvp);
			tempModel.Draw();
		}
		outlineShaders->Bind(false);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}
}
