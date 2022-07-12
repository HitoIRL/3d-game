#include <chrono>
#include <array>

#include <glad/glad.h>

#include "Core/Window.hpp"
#include "Core/Shaders.hpp"
#include "Debug/Log.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
};

int main() {
	auto startTime = std::chrono::system_clock::now();

	Log::Init();
	LOG_INFO("Hello!");

	auto window = std::make_unique<Window>(WindowSettings {
		"3d game es",
		{ 1280, 720 }
	});
	auto shaders = std::make_unique<Shaders>();

	const std::array<Vertex, 3> vertices {{
		{{ -0.5f, -0.5f, 0.01f }, { 1.0f, 0.0f, 0.0f }},
		{{ 0.5f, -0.5f, -0.01f }, { 0.0f, 1.0f, 0.0f }},
		{{ 0.0f, 0.5f, 0.01f }, { 0.0f, 0.0f, 1.0f }}
	}};

	const std::array<Vertex, 3> background {{
		{{ -1.0f, -1.0f, 0.0f }, { 0.12f, 0.14f, 0.16f }},
		{{ 3.0f, -1.0f, 0.0f }, { 0.12f, 0.14f, 0.16f }},
		{{ -1.0f, 3.0f, 0.0f }, { 0.8f, 0.8f, 0.82f }}
	}};

	// i tried to make it as DSA as possible
	auto createBuffer = [](const std::array<Vertex, 3>& vertices) -> std::uint32_t {
		std::uint32_t buffer;
		glCreateBuffers(1, &buffer);
		glNamedBufferStorage(buffer, vertices.size() * sizeof(Vertex), vertices.data(), GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
		return buffer;
	};

	auto backgroundBuffer = createBuffer(background);
	auto foregroundBuffer = createBuffer(vertices);

	std::uint32_t vao;
	glCreateVertexArrays(1, &vao);

	// setup vertex format
	auto positionAttrib = shaders->GetAttribLocation("position");
	glEnableVertexArrayAttrib(vao, positionAttrib);
	glVertexArrayAttribFormat(vao, positionAttrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(vao, positionAttrib, 0);

	auto colorAttrib = shaders->GetAttribLocation("color");
	glEnableVertexArrayAttrib(vao, colorAttrib);
	glVertexArrayAttribFormat(vao, colorAttrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
	glVertexArrayAttribBinding(vao, colorAttrib, 0);

	// attach buffer to read from
	//glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));

	glBindVertexArray(vao);
	//window->SetBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	while (window->IsOpen()) {
		shaders->Bind(true);
		glVertexArrayVertexBuffer(vao, 0, backgroundBuffer, 0, sizeof(Vertex));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glVertexArrayVertexBuffer(vao, 0, foregroundBuffer, 0, sizeof(Vertex));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &backgroundBuffer);
	glDeleteBuffers(1, &foregroundBuffer);
}
