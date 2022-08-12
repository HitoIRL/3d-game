#pragma once

class Texture {
public:
	Texture(std::string_view path, std::uint16_t slot);
	~Texture();

	//void Bind() const;

	const std::string& GetPath() const { return path; }
	std::uint16_t GetSlot() const { return slot; }
	bool IsTransparent() const { return transparent; };
private:
	const std::string path;
	std::uint32_t id;
	std::uint16_t slot;
	bool transparent;
};

namespace TextureManager {
	std::shared_ptr<Texture> CreateTexture(std::string_view path); // limited to 16 textures XD
};
