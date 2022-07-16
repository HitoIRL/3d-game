#pragma once

#include <unordered_map>

#include <glm/mat4x2.hpp>

class Shaders {
public:
	Shaders();
	~Shaders();

	void Bind(bool state);

	int GetAttribLocation(std::string_view name) const;

	// uniform setters
	void UniformFloat(std::string_view name, float value);
	void UniformVec2(std::string_view name, const glm::vec2& value);
	void UniformMat4(std::string_view name, const glm::mat4& value);
private:
	std::uint32_t CreateShader(const char* source, std::uint32_t type) const;
	int GetUniformLocation(std::string_view name);

	std::uint32_t program;
	bool binded;
	std::unordered_map<std::string_view, int> cache; // uniform location cache
};
