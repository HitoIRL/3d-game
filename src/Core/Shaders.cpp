#include "Shaders.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Debug/Log.hpp"

constexpr auto VertexShader = R"glsl(
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 v_color;

vec3 remappedColour = (normal + vec3(1.f)) / 2.f;

uniform mat4 transform;

void main(){
	gl_Position = transform * vec4((position * vec3(1.0f, 1.0f, -1.0f)) + (vec3(0, -0.5, 0)), 1.0f);

	v_color = remappedColour;
}
)glsl";

constexpr auto FragmentShader = R"glsl(
#version 460 core

in vec3 v_color;

out vec4 color;

void main() {
	color = vec4(v_color, 1.0);
}
)glsl";

Shaders::Shaders() : program(glCreateProgram()) {
	const auto vertex = CreateShader(VertexShader, GL_VERTEX_SHADER);
	const auto fragment = CreateShader(FragmentShader, GL_FRAGMENT_SHADER);

	glLinkProgram(program);

	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
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

void Shaders::UniformVec2(std::string_view name, const glm::vec2& value) {
	glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shaders::UniformFloat(std::string_view name, float value) {
	glUniform1f(GetUniformLocation(name), value);
}

void Shaders::UniformMat4(std::string_view name, const glm::mat4& value) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

std::uint32_t Shaders::CreateShader(const char* source, std::uint32_t type) const {
	const auto shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == false) {
		int maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		maxLength -= 1; // gets rid of last NULL character (we are using spdlog for logging which adds NULL character at the end of line automatically)
		std::vector<char> log(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]);
		glDeleteShader(shader);
		LOG_ERROR("Failed to compile shader:\n{}", log);
		std::exit(EXIT_FAILURE);
	}

	glAttachShader(program, shader);
	return shader;
}

int Shaders::GetUniformLocation(std::string_view name) {
	if (cache.contains(name))
		return cache[name];

	auto location = glGetUniformLocation(program, name.data());

	cache[name] = location;
	return location;
}
