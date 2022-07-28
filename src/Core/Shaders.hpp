#pragma once

#include <unordered_map>

#include <glm/mat4x2.hpp>

class Shaders {
public:
	Shaders(std::string_view vertexPath, std::string_view fragmentPath);
	~Shaders();

	void Bind(bool state);

	int GetAttribLocation(std::string_view name) const;

	// uniform setters
	void SetInt(std::string_view name, int value) const;
	void SetIntArray(std::string_view name, int* value) const;
	void SetFloat(std::string_view name, float value) const;
	void SetVec2(std::string_view name, const glm::vec2& value) const;
	void SetVec3(std::string_view name, const glm::vec3& value) const;
	void SetMat4(std::string_view name, const glm::mat4& value) const;
private:
	int GetLocation(std::string_view name) const;
	std::uint32_t CreateShader(std::string_view path, std::uint32_t type) const;

	std::uint32_t program;
	bool binded;
	std::unordered_map<std::string, int> uniforms;
};
