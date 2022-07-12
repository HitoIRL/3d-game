#include "Shaders.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Debug/Log.hpp"

constexpr auto VertexShader = R"glsl(
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertex_color;

void main() {
	gl_Position = vec4(position, 1.0);
	vertex_color = color;
}
)glsl";

constexpr auto FragmentShader = R"glsl(
#version 460 core

in vec3 vertex_color;

out vec4 color;

void main() {
	color = vec4(vertex_color, 1.0);
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
