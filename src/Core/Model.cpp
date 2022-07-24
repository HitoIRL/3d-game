#include "Model.hpp"

#include <glad/glad.h>

RawModel::RawModel(const std::vector<Vertex>& vertices, const std::vector<int>& indices) : indexCount(indices.size()) {
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

RawModel::~RawModel() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &buffer);
}

void RawModel::BindVAO(bool state) const {
	glBindVertexArray(state ? vao : 0);
}

TexturedModel::TexturedModel(const RawModel& rawModel, const Texture& texture) : rawModel(rawModel), texture(texture) {

}

void TexturedModel::Draw() const {
	glDrawElements(GL_TRIANGLES, rawModel.GetIndexCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(rawModel.GetIndicesOffset()));
}

void TexturedModel::BindTexture() const {
	texture.Bind(0);
}
