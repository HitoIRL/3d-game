#pragma once

#include <unordered_map>

#include <glm/mat4x2.hpp>

class Shader {
public:
	Shader();
	~Shader();

	void Bind(bool state);

	void UniformMat4(std::string_view name, const glm::mat4& matrix);
	void Uniform1iv(std::string_view name, int* arr, int size);
private:
	std::uint32_t CreateShader(const char* source, std::uint32_t type) const;
	int GetUniformLocation(std::string_view name);

	std::uint32_t program;
	bool binded;
	std::unordered_map<std::string_view, int> cache; // uniform location cache
};
