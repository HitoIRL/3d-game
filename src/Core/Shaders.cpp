#include "Shaders.hpp"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Debug/Log.hpp"

Shaders::Shaders(std::string_view vertexPath, std::string_view fragmentPath) : program(glCreateProgram()), binded(false) {
	const auto vertex = CreateShader(vertexPath, GL_VERTEX_SHADER);
	const auto fragment = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);

	glLinkProgram(program);

	// check if linked correctly
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		std::vector<char>log(logLen);
		int written;
		glGetProgramInfoLog(program, logLen, &written, log.data());
		LOG_ERROR("Failed to link program:\n{}", log.data());
		std::exit(EXIT_FAILURE);
	}

	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// retrieve all the uniforms
	int uniformCount = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);

	if (uniformCount != 0) {
		int maxNameLength = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

		auto uniformName = std::make_unique<char[]>(maxNameLength);

		int length = 0;
		int count = 0;
		std::uint32_t type = GL_NONE;

		for (auto i = 0u; i < uniformCount; ++i) {
			glGetActiveUniform(program, i, maxNameLength, &length, &count, &type, uniformName.get());
			UniformInfo info{};
			info.location = glGetUniformLocation(program, uniformName.get());
			info.count = count;
			uniforms.emplace(uniformName.get(), info);
		}
	}
}

Shaders::~Shaders() {
	glDeleteProgram(program);
}

void Shaders::Bind(bool state) {
	if (binded == state)
		return;

	glUseProgram(state ? program : 0);
	binded = state;
}

int Shaders::GetAttribLocation(std::string_view name) const {
	return glGetAttribLocation(program, name.data());
}

void Shaders::SetInt(std::string_view name, int value) const {
	glUniform1i(GetUniformInfo(name).location, value);
}

void Shaders::SetIntArray(std::string_view name, int* value) const {
	auto& info = GetUniformInfo(name);
	glUniform1iv(info.location, 16, value);
}

void Shaders::SetFloat(std::string_view name, float value) const {
	glUniform1f(GetUniformInfo(name).location, value);
}

void Shaders::SetVec2(std::string_view name, const glm::vec2& value) const {
	glUniform2fv(GetUniformInfo(name).location, 1, glm::value_ptr(value));
}

void Shaders::SetVec3(std::string_view name, const glm::vec3& value) const {
	glUniform3fv(GetUniformInfo(name).location, 1, glm::value_ptr(value));
}

void Shaders::SetMat4(std::string_view name, const glm::mat4& value) const {
	glUniformMatrix4fv(GetUniformInfo(name).location, 1, GL_FALSE, glm::value_ptr(value));
}

std::uint32_t Shaders::CreateShader(std::string_view path, std::uint32_t type) const {
	std::ifstream file(path.data());

	if (file.fail()) {
		LOG_ERROR("Couldn't open '{}' shader file", path);
		std::exit(EXIT_FAILURE);
	}

	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	auto source = str.c_str();

	const auto shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == false) {
		int maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		maxLength -= 1; // get rid of last NULL character (we are using spdlog for logging which add NULL character at the end of the line)
		std::vector<char> log(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]);
		glDeleteShader(shader);
		LOG_ERROR("Failed to compile '{}' shader:\n{}", path, log);
		std::exit(EXIT_FAILURE);
	}

	glAttachShader(program, shader);
	return shader;
}

const Shaders::UniformInfo& Shaders::GetUniformInfo(std::string_view name) const {
	if (uniforms.contains(name.data()))
		return uniforms.at(name.data());

	LOG_ERROR("Uniform '{}' not found", name);
	return UniformInfo{};
}
