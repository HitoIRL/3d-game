#include "Texture.hpp"

#include <glm/vec2.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>

#include "../Debug/Log.hpp"

Texture::Texture(std::string_view path, TextureType type) : type(type), path(path) {
	glm::ivec2 size;
	int channels;

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
		LOG_ERROR("Failed to load texture '{}'", path);
		std::exit(EXIT_FAILURE);
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::Bind(std::uint16_t slot) const {
	glBindTextureUnit(slot, id);
}
