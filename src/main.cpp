#include <pch.hpp>
#include <OWL/OWL.hpp>
#include <OWL/Main.hpp>
#include <OWL/Time.hpp>
#include <glad/glad.h>
#include <OWL/OpenGL.hpp>

int main(int argc, char** args) {
	OWL::OpenGLContext context;
	OWL::Window window;
	window.setContext(context);

	int version = gladLoadGLLoader((GLADloadproc)context.getLoaderFunction());
	// int version = gladLoadGL();
	if (version == 0) {
        std::cout << "Failed to initialize OpenGL context\n";
        return -1;
    }
	OWL::FPSLimiter eventDelay(60);
	while(window.isRunning()) {
		eventDelay.start();
		window.pollEvents();

		if(window.keyboard.isKeyPressed(window.keyboard.Escape)) {
			window.close();
		}
		if(window.keyboard.getKeyData().keyEnum == window.keyboard.F11) {
			window.setFullScreen(!window.isFullScreen());
		}

		unsigned int uiSize = (
			(window.getSize().x > window.getSize().y) ? 
			window.getSize().y : 
			window.getSize().x
		);

		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		context.swapBuffers();
		eventDelay.end();
	}

	return 0;
}