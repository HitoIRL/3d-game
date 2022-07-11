#include "Core/Window.hpp"
#include "Core/Shader.hpp"
#include "Debug/Log.hpp"

int main() {
	Log::Init();

	auto window = std::make_unique<Window>(WindowSettings {
		"3d game es",
		{ 1280, 720 }
	});
	auto shader = std::make_unique<Shader>();

	LOG_INFO("elo testuwa");

	window->SetBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	while (window->IsOpen()) {

	}
}
