#include <pch.hpp>
#include <iostream>
#include <OWL/OWL.hpp>
#include <OWL/Main.hpp>
#include <OWL/Time.hpp>
#include <OWL/OpenGL.hpp>

int main(int argc, char** args) {
	OWL::OpenGLContext context;
	OWL::Window window;
	window.setContext(context);

	window.gamepads.setCount(1);
	
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

		eventDelay.end();
	}

	return 0;
}