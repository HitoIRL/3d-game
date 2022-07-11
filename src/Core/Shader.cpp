#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

constexpr auto VertexShader = R"glsl(
#version 460 core

in vec2 i_position;

uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * vec4(i_position, 0.0, 1.0);
}
)glsl";

constexpr auto FragmentShader = R"glsl(
#version 460 core

out vec4 o_color;

void main() {
	o_color = vec4(1.0, 1.0, 1.0, 1.0);
}
)glsl";

Shader::Shader() : program(glCreateProgram()) {
	const auto vertex = CreateShader(VertexShader, GL_VERTEX_SHADER);
	const auto fragment = CreateShader(FragmentShader, GL_FRAGMENT_SHADER);

	glLinkProgram(program);

	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::Bind(bool state) {
	if (binded == state)
		return;

	glUseProgram(state ? program : 0);
	binded = state;
}

std::uint32_t Shader::CreateShader(const char* source, std::uint32_t type) const {
	const auto shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader); // idk i probably should log (and assert?) if compilation failed

	int compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == false) {
		int maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> log(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]);
		glDeleteShader(shader);
		// kurwa abort pls here but idk how
	}

	glAttachShader(program, shader);
	return shader;
}

int Shader::GetUniformLocation(std::string_view name) {
	if (cache.contains(name))
		return cache[name];

	auto location = glGetUniformLocation(program, name.data());

	cache[name] = location;
	return location;
}
