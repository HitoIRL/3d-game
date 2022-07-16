#include <chrono>
#include <array>
#include <filesystem>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Core/Window.hpp"
#include "Core/Shaders.hpp"
#include "Debug/Log.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Texture {
	std::uint32_t id;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<std::uint32_t> indices;
	std::vector<Texture> textures;
};

struct RawMeshData {
	RawMeshData() : positions(1), normals(1), texCoords(1) {}

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::ivec3> faceIndices;
};

struct MeshDataSplit {
	std::vector<Vertex> vertices;
};

constexpr auto packChars = [](char a, char b) -> std::uint32_t {
	return (static_cast<uint32_t>(a) << 8) | static_cast<uint32_t>(b);
};
constexpr uint32_t v = packChars('v', ' ');
constexpr uint32_t vn = packChars('v', 'n');
constexpr uint32_t vt = packChars('v', 't');
constexpr uint32_t vp = packChars('v', 'p');
constexpr uint32_t f = packChars('f', ' ');

RawMeshData ReadObjRaw(std::string_view path) { // https://dokipen.com/modern-opengl-part-11-creating-a-basic-obj-loader-and-loading-models-into-buffers/
	RawMeshData meshData;
	auto fp = fopen(path.data(), "r");
	if (!fp) {
		LOG_ERROR("Couldn't open file '{}'", path);
		std::exit(EXIT_FAILURE);
	}

	char line[128];
	size_t line_size;
	std::vector<int> spacePositions(8);
	char* end;
	uint32_t key;

	while (fgets(line, 128, fp)) {
		line_size = strlen(line);
		spacePositions.clear();
		key = packChars(line[0], line[1]);

		for (auto i = 0u; i < line_size; ++i) {
			if (line[i] == ' ') {
				line[i] = '\0';
				spacePositions.push_back(i + 1);
			}
		}
		spacePositions.push_back((int)line_size);

		switch (key) {
		case v:
			meshData.positions.emplace_back(std::strtof(&line[spacePositions[0]], nullptr), std::strtof(&line[spacePositions[1]], nullptr), std::strtof(&line[spacePositions[2]], nullptr));
			break;
		case vn:
			meshData.normals.emplace_back(std::strtof(&line[spacePositions[0]], nullptr),std::strtof(&line[spacePositions[1]], nullptr),std::strtof(&line[spacePositions[2]], nullptr));
			break;
		case vt:
			meshData.texCoords.emplace_back(std::strtof(&line[spacePositions[0]], nullptr),std::strtof(&line[spacePositions[1]], nullptr));
			break;
		case f: {
			// is face
			int a = std::strtol(&line[spacePositions[0]], &end, 10);
			int b = std::strtol(end + (*end == '/'), &end, 10);
			int c = std::strtol(end + (*end == '/'), &end, 10);
			meshData.faceIndices.emplace_back(a, b, c);
			int d = std::strtol(&line[spacePositions[1]], &end, 10);
			int e = std::strtol(end + (*end == '/'), &end, 10);
			int f = std::strtol(end + (*end == '/'), &end, 10);
			meshData.faceIndices.emplace_back(d, e, f);
			int g = std::strtol(&line[spacePositions[2]], &end, 10);
			int h = std::strtol(end + (*end == '/'), &end, 10);
			int i = std::strtol(end + (*end == '/'), &end, 10);
			meshData.faceIndices.emplace_back(g, h, i);
			if (spacePositions.size() == 5) {
				// face 0
				meshData.faceIndices.emplace_back(a, b, c);
				// face 2
				meshData.faceIndices.emplace_back(g, h, i);
				// reuse def as those temps aren't needed
				d = std::strtol(&line[spacePositions[3]], &end, 10);
				e = std::strtol(end + (*end == '/'), &end, 10);
				f = std::strtol(end + (*end == '/'), &end, 10);
				meshData.faceIndices.emplace_back(d, e, f);
			}
			break;
		}
		default:

			break;
		}
	}

	return meshData;
}

MeshDataSplit ReadObjSplit(std::string_view path) {
	auto rawMeshData = ReadObjRaw(path);
	MeshDataSplit meshData;
	meshData.vertices.resize(rawMeshData.faceIndices.size());
	if (rawMeshData.texCoords.empty()) {
		rawMeshData.texCoords.resize(rawMeshData.faceIndices.size());
	}
	if (rawMeshData.normals.empty()) {
		rawMeshData.normals.resize(rawMeshData.faceIndices.size());
	}
#pragma omp parallel for
	for (int i = 0u; i < rawMeshData.faceIndices.size(); ++i) {
		meshData.vertices[i] = {
				rawMeshData.positions[rawMeshData.faceIndices[i].x],
				rawMeshData.normals[rawMeshData.faceIndices[i].z],
				//rawMeshData.texCoords[rawMeshData.faceIndices[i].y]
		};
	}
	return meshData;
}

int main() {
	auto startTime = std::chrono::system_clock::now();

	Log::Init();
	LOG_INFO("Hello!");
	auto cwd = std::filesystem::current_path();
	LOG_INFO("Current working directory: {}", cwd.string());

	auto meshData = ReadObjSplit("mesh.obj");

	auto window = std::make_unique<Window>("3d game es", glm::vec2(1280, 720));
	auto shaders = std::make_unique<Shaders>();

	// i tried to make it as DSA as possible
	auto createBuffer = [](const std::vector<Vertex>& vertices) -> std::uint32_t {
		std::uint32_t buffer;
		glCreateBuffers(1, &buffer);
		glNamedBufferStorage(buffer, vertices.size() * sizeof(Vertex), vertices.data(), GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
		return buffer;
	};

	//auto backgroundBuffer = createBuffer(background);
	//auto foregroundBuffer = createBuffer(vertices);
	auto meshBuffer = createBuffer(meshData.vertices);

	std::uint32_t vao;
	glCreateVertexArrays(1, &vao);

	// setup vertex format
	auto positionAttrib = shaders->GetAttribLocation("position");
	glEnableVertexArrayAttrib(vao, positionAttrib);
	glVertexArrayAttribFormat(vao, positionAttrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(vao, positionAttrib, 0);

	auto normalAttrib = shaders->GetAttribLocation("normal");
	glEnableVertexArrayAttrib(vao, normalAttrib);
	glVertexArrayAttribFormat(vao, normalAttrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	glVertexArrayAttribBinding(vao, normalAttrib, 0);

	// attach buffer to read from
	glVertexArrayVertexBuffer(vao, 0, meshBuffer, 0, sizeof(Vertex));

	glBindVertexArray(vao);
	shaders->Bind(true);
	//window->SetBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	LOG_INFO("Mesh object vertices size: {}", meshData.vertices.size());
	while (window->IsOpen()) {
		auto currentTime = std::chrono::duration<float>(std::chrono::system_clock::now() - startTime).count();

		auto trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, currentTime, glm::vec3(0.0, 1.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.09, 0.09, 0.09));
		shaders->UniformMat4("transform", trans);

		glDrawArrays(GL_TRIANGLES, 0, meshData.vertices.size());
	}

	glDeleteVertexArrays(1, &vao);
	//glDeleteBuffers(1, &backgroundBuffer);
	glDeleteBuffers(1, &meshBuffer);
}
