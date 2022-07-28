#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "../Debug/Log.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<std::shared_ptr<Texture>>& textures) : textures(textures) {
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
	auto index = 0u;
	for (auto& texture : textures) {
		texture->Bind(index++);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(indicesOffset));
	glBindVertexArray(0);
}

Model::Model(std::string_view path) : directory(path.substr(0, path.find_last_of('/'))) {
	Assimp::Importer importer;
	auto scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG_ERROR("Couldn't load assimp mesh '{}'", path);
		std::exit(EXIT_FAILURE);
	}

	meshes.reserve(scene->mNumMeshes); // needed to avoid mesh copying
	for (auto i = 0u; i < scene->mNumMeshes; i++) {
		const auto mesh = scene->mMeshes[i];
		CreateMesh(mesh, scene);
	}
}

void Model::Draw() const {
	for (const auto& mesh : meshes)
		mesh.Draw();
}

void Model::CreateMesh(const aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	for (auto i = 0u; i < mesh->mNumVertices; i++) {
		const auto position = mesh->mVertices[i];
		const auto texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0.0, 0.0, 0.0f);
		Vertex vertex {
				{ position.x, position.y, position.z },
				{ texCoord.x, texCoord.y }
		};
		vertices.emplace_back(vertex);
	}

	std::vector<std::uint32_t> indices;
	for (auto i = 0u; i < mesh->mNumFaces; i++) {
		const auto& face = mesh->mFaces[i];
		for (auto j = 0u; j < face.mNumIndices; j++)
			indices.emplace_back(face.mIndices[j]);
	}

	std::vector<std::shared_ptr<Texture>> textures;
	if (mesh->mMaterialIndex >= 0) {
		const auto material = scene->mMaterials[mesh->mMaterialIndex];
		for (auto i = 0u; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);
			auto path = directory + "/" + str.C_Str();

			bool skip = false;
			for (auto& loadedTexture : loadedTextures) {
				if (loadedTexture->GetPath() == path) {
					textures.emplace_back(loadedTexture);
					skip = true;
					break;
				}
			}

			if (!skip) {
				auto texture = std::make_shared<Texture>(path);
				textures.emplace_back(texture);
				loadedTextures.emplace_back(texture);
			}
		}
	}

	meshes.emplace_back(vertices, indices, textures);
}

Texture::Texture(std::string_view path) : path(path) {
	glm::ivec2 size;
	int channels;

	LOG_INFO("Creating new texture from '{}'", path);

	if (const auto data = stbi_load(path.data(), &size.x, &size.y, &channels, 0)) {
		glCreateTextures(GL_TEXTURE_2D, 1, &id);

		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureStorage2D(id, 1, channels == 4 ? GL_RGBA8 : GL_RGB8, size.x, size.y);
		glTextureSubImage2D(id, 0, 0, 0, size.x, size.y, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	} else {
		LOG_ERROR("Failed to load texture '{}' | {}", path, stbi_failure_reason());
		std::exit(EXIT_FAILURE);
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::Bind(std::uint16_t slot) const {
	glBindTextureUnit(slot, id);
}
