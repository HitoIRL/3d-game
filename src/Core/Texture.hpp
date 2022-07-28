#pragma once

enum class TextureType {
	Diffuse,
	Specular,
};

class Texture {
public:
	Texture(std::string_view path, TextureType type);
	~Texture();

	void Bind(std::uint16_t slot) const;

	const std::string& GetPath() const { return path; }
private:
	std::uint32_t id;
	const TextureType type;
	const std::string path;
};
