#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glad/glad.h>

#include "../Debug/Log.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices) {
	indexCount = indices.size();

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

	auto texCoordAttrib = 1;//shaders->GetAttribLocation("texCoord");
	glEnableVertexArrayAttrib(vao, texCoordAttrib);
	glVertexArrayAttribFormat(vao, texCoordAttrib, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
	glVertexArrayAttribBinding(vao, texCoordAttrib, 0);

	// attach buffers
	glVertexArrayVertexBuffer(vao, 0, buffer, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(vao, buffer);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &buffer);
}

void Mesh::Draw() const {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(indicesOffset));
	glBindVertexArray(0);
}

Model::Model(std::string_view path) {
	Assimp::Importer importer;
	auto scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG_ERROR("Couldn't load assimp mesh '{}'", path);
		std::exit(EXIT_FAILURE);
	}

	for (auto i = 0u; i < scene->mNumMeshes; i++) {
		const auto mesh = scene->mMeshes[i];
		CreateMesh(mesh);
	}
}

void Model::Draw() const {
	for (const auto& mesh : meshes)
		mesh->Draw();
}

void Model::CreateMesh(const aiMesh* mesh) {
	std::vector<Vertex> vertices;
	std::vector<std::uint32_t> indices;

	for (auto i = 0u; i < mesh->mNumVertices; i++) {
		const auto position = mesh->mVertices[i];
		const auto texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0.0, 0.0, 0.0f);
		Vertex vertex {
				{ position.x, position.y, position.z },
				{ texCoord.x, texCoord.y }
		};
		vertices.emplace_back(vertex);
	}

	for (auto i = 0u; i < mesh->mNumFaces; i++) {
		const auto& face = mesh->mFaces[i];
		for (auto j = 0u; j < face.mNumIndices; j++)
			indices.emplace_back(face.mIndices[j]);
	}

	meshes.emplace_back(std::make_shared<Mesh>(vertices, indices));
}
