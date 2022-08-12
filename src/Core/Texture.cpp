#include "Texture.hpp"

#include <glm/vec2.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>

#include "../Debug/Log.hpp"

Texture::Texture(std::string_view path, std::uint16_t slot) : path(path), slot(slot), transparent(false) {
	glm::ivec2 size;
	int channels;

	LOG_INFO("Creating new texture from '{}'", path);
	if (const auto data = stbi_load(path.data(), &size.x, &size.y, &channels, 0)) {
		transparent = channels == 4;

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

	glBindTextureUnit(slot, id);
}

Texture::~Texture() {
	LOG_INFO("Deleting texture");
	glDeleteTextures(1, &id);
}

std::uint16_t freeTexture = 0;

std::shared_ptr<Texture> TextureManager::CreateTexture(std::string_view path) {
	return std::make_shared<Texture>(path, freeTexture++);
}
