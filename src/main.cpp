#include <pch.hpp>
#include <OWL/OWL.hpp>
#include <OWL/Main.hpp>
#include <OWL/Time.hpp>
#include <glad/glad.h>
#include <OWL/OpenGL.hpp>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main(int argc, char** args) {
	OWL::OpenGLContext context;
	OWL::Window window(OWL::Vec2ui(1280, 720), "OpenGL Flight Sim");
	window.setContext(context);
    
	std::cout << "Welcome to the OpenGL Flight Sim!\n";

	const aiScene * p_Scene = aiImportFile(
		"./res/outside.obj",
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_ImproveCacheLocality |
		aiProcess_SortByPType
	);

	if (!p_Scene) {
		std::cout << "Failed to open scene file: " << aiGetErrorString() << "\n";
		return -2;
	}

	int version = gladLoadGLLoader((GLADloadproc)context.getLoaderFunction());
	if (version == 0) {
        std::cout << "Failed to initialize OpenGL context\n";
        return -1;
    }
	std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GPU: " << glGetString(GL_RENDERER) << "\n";

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