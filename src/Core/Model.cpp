#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glad/glad.h>

#include "../Debug/Log.hpp"

RawModel::RawModel(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices) {
	LOG_INFO("Create buffers");

	// vbo, ibo
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	indicesSize = indices.size() * sizeof(std::uint32_t);
	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, indicesSize, indices.data(), GL_DYNAMIC_STORAGE_BIT);

	// vao
	glCreateVertexArrays(1, &vao);

	// vertex buffer layout
	auto attribIndex = 0u;

	glEnableVertexArrayAttrib(vao, attribIndex);
	glVertexArrayAttribFormat(vao, attribIndex, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(vao, attribIndex++, 0);

	glEnableVertexArrayAttrib(vao, attribIndex);
	glVertexArrayAttribFormat(vao, attribIndex, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));
	glVertexArrayAttribBinding(vao, attribIndex++, 0);

	glEnableVertexArrayAttrib(vao, attribIndex);
	glVertexArrayAttribIFormat(vao, attribIndex, 1, GL_INT, offsetof(Vertex, texIndex));
	glVertexArrayAttribBinding(vao, attribIndex++, 0);

	// attach buffers
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(vao, ibo);
}

RawModel::~RawModel() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void RawModel::Draw() const {
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, nullptr);
}

void RawModel::BindVAO(bool state) const {
	glBindVertexArray(state ? vao : 0);
}

Model::Model(std::string_view path) : directory(path.substr(0, path.find_last_of('/'))) {
	Assimp::Importer importer;
	auto scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG_ERROR("Couldn't load '{}' model", path);
		std::exit(EXIT_FAILURE);
	}

	for (auto i = 0u; i < scene->mNumMeshes; i++) {
		const auto mesh = scene->mMeshes[i];
		CreateMesh(mesh, scene);
	}
}

void Model::Draw() const {
	for (auto i = 0u; i < textures.size(); i++)
		textures[i]->Bind(i);

	rawModel->Draw(); // vao binding is handled by renderer
}

void Model::CreateMesh(const aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	for (auto i = 0u; i < mesh->mNumVertices; i++) {
		aiVector3D zero(0);
		const auto position = mesh->mVertices[i];
		//const auto normal = mesh->HasNormals() ? mesh->mNormals[i] : zero;
		const auto texCoords = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zero;

		Vertex vertex {
				{ position.x, position.y, position.z },
				{ texCoords.x, texCoords.y },
				1
		};
		vertices.emplace_back(vertex);
	}

	std::vector<std::uint32_t> indices;
	for (auto i = 0u; i < mesh->mNumFaces; ++i) {
		const auto& face = mesh->mFaces[i];
		for (auto j = 0u; j < face.mNumIndices; j++)
			indices.emplace_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		const auto material = scene->mMaterials[mesh->mMaterialIndex];
		auto diffuseMaps = FetchTextures(material, aiTextureType_DIFFUSE);
		auto specularMaps = FetchTextures(material, aiTextureType_SPECULAR);
	};

	rawModel = std::make_unique<RawModel>(vertices, indices);
}

std::vector<std::shared_ptr<Texture>> Model::FetchTextures(const aiMaterial* material, aiTextureType type) {
	for (auto i = 0u; i < material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);
		auto path = directory + "/" + str.C_Str();

		bool skip = false;
		for (auto& texture : textures) {
			if (texture->GetPath() == path) {
				skip = true;
				break;
			}
		}

		if (!skip) {
			auto texture = std::make_shared<Texture>(path);
			textures.emplace_back(texture);
		}
	}

	return textures;
}
