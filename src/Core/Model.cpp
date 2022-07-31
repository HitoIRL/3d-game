#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glad/glad.h>

#include "../Debug/Log.hpp"

Model::Model(std::string_view path) : directory(path.substr(0, path.find_last_of('/'))) {
	Assimp::Importer importer;
	auto scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG_ERROR("Couldn't load assimp mesh '{}'", path);
		std::exit(EXIT_FAILURE);
	}

	for (auto i = 0u; i < scene->mNumMeshes; i++) {
		const auto mesh = scene->mMeshes[i];
		CreateMesh(mesh, scene);
	}

	LOG_INFO("Create buffers");

	// vbo, ibo
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
	auto positionAttrib = 0u;//shaders->GetAttribLocation("position");
	glEnableVertexArrayAttrib(vao, positionAttrib);
	glVertexArrayAttribFormat(vao, positionAttrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(vao, positionAttrib, 0);

	auto normalAttrib = 1u;//shaders->GetAttribLocation("normalAttrib");
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

Model::~Model() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &buffer);
}

void Model::Draw() const {
	for (auto i = 0u; i < textures.size(); i++)
		textures[i]->Bind(i);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(indicesOffset));
}

void Model::BindVAO(bool state) const {
	glBindVertexArray(state ? vao : 0);
}

void Model::CreateMesh(const aiMesh* mesh, const aiScene* scene) {
	for (auto i = 0u; i < mesh->mNumVertices; i++) {
		aiVector3D zero(0);
		const auto position = mesh->mVertices[i];
		const auto normal = mesh->HasNormals() ? mesh->mNormals[i] : zero;
		const auto texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zero;

		Vertex vertex {
				{ position.x, position.y, position.z },
				{ normal.x, normal.y, normal.z },
				{ texCoord.x, texCoord.y },
		};

		vertices.emplace_back(vertex);
	}

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
