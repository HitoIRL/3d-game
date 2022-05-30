#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>

constexpr glm::uvec2 Size = { 1280, 720 };

int main() {
	if (!glfwInit())
		return -1;

	// opengl core 4.6
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(Size.x, Size.y, "3d game", nullptr, nullptr);
	if (!window) { // failed to create window
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // toggles vsync

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	// needs to be after loading GL (glad)
	glViewport(0, 0, Size.x, Size.y);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// destroys all windows & glfw
	glfwTerminate();
	return 0;
}
