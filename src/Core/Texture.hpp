#pragma once

class Texture {
public:
	Texture(std::string_view path);
	~Texture();

	void Bind(std::uint16_t slot) const;
private:
	std::uint32_t id;
};
