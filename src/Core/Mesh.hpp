#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <assimp/scene.h>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
};

class Mesh {
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);
	~Mesh();

	void Draw() const;
private:
	std::uint32_t vao, buffer, indexCount, indicesOffset;
};

class Model {
public:
	Model(std::string_view path);

	void Draw() const;
private:
	void CreateMesh(const aiMesh* mseh);

	std::vector<std::shared_ptr<Mesh>> meshes;
};
