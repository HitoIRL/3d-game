#include <chrono>
#include <filesystem>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Window.hpp"
#include "Core/Shaders.hpp"
#include "Core/Texture.hpp"
#include "Debug/Log.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
};

class RawModel {
public:
	RawModel(const std::vector<Vertex>& vertices, const std::vector<int>& indices) : indexCount(indices.size()) {
		// vbo & ibo
		const auto verticesSize = vertices.size() * sizeof(Vertex);
		const auto indicesSize = indices.size() * sizeof(std::uint32_t);

		indicesOffset = verticesSize;

		glCreateBuffers(1, &buffer);
		glNamedBufferStorage(buffer, verticesSize + indicesSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

		glNamedBufferSubData(buffer, 0, verticesSize, vertices.data());
		glNamedBufferSubData(buffer, indicesOffset, indicesSize, indices.data());

		// vao
		glCreateVertexArrays(1, &vao);

		// vertex buffer layout
		auto positionAttrib = 0;//shaders->GetAttribLocation("position");
		glEnableVertexArrayAttrib(vao, positionAttrib);
		glVertexArrayAttribFormat(vao, positionAttrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
		glVertexArrayAttribBinding(vao, positionAttrib, 0);

		auto texCoordAttrib = 1;//shaders->GetAttribLocation("position");
		glEnableVertexArrayAttrib(vao, texCoordAttrib);
		glVertexArrayAttribFormat(vao, texCoordAttrib, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
		glVertexArrayAttribBinding(vao, texCoordAttrib, 0);

		// attach buffers
		glVertexArrayVertexBuffer(vao, 0, buffer, 0, sizeof(Vertex));
		glVertexArrayElementBuffer(vao, buffer);
	}
	~RawModel() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &buffer);
	}

	void BindVAO() const {
		glBindVertexArray(vao);
	}

	std::uint32_t GetIndexCount() const {
		return indexCount;
	}

	std::uint32_t GetIndicesOffset() const {
		return indicesOffset;
	}
private:
	std::uint32_t vao, buffer;
	std::uint32_t indexCount, indicesOffset;
};

class TexturedModel {
public:
	TexturedModel(const RawModel& rawModel, const Texture& texture) : rawModel(rawModel), texture(texture) {

	}

	void Draw() const {
		glDrawElements(GL_TRIANGLES, rawModel.GetIndexCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(rawModel.GetIndicesOffset()));
	}

	void BindTexture() const {
		texture.Bind(0);
	}
private:
	RawModel rawModel;
	Texture texture;
};

int main() {
	auto startTime = std::chrono::system_clock::now();

	Log::Init();
	LOG_INFO("Hello!");
	auto cwd = std::filesystem::current_path();
	LOG_INFO("Current working directory: {}", cwd.string());

	auto window = std::make_unique<Window>("3d game es", glm::vec2(1280, 720));
	auto shaders = std::make_unique<Shaders>("assets/vertex.glsl", "assets/fragment.glsl");

	RawModel model({
		{{ -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f }},
		{{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f }},
		{{ 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f  }},
		{{ 0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f }}
	}, {
		0, 1, 3,
		3, 1, 2
	});
	Texture texture("assets/bomba.png");
	TexturedModel texturedModel(model, texture);

	shaders->Bind(true);
	model.BindVAO();
	texturedModel.BindTexture();
	shaders->SetInt("_texture", 0);

	while (window->IsOpen()) {
		auto currentTime = std::chrono::duration<float>(std::chrono::system_clock::now() - startTime).count();
		texturedModel.Draw();
	}
}
