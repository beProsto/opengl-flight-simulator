#include "OWL/Utility/Vec3.hpp"
#include <OWL/Main.hpp>
#include <OWL/OWL.hpp>
#include <OWL/Time.hpp>
#include <pch.hpp>

#include <glad/glad.h>

#include <OWL/OpenGL.hpp>

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

#include "./mesh.hpp"

int main(int argc, char **args) {
  OWL::OpenGLContext context;
  OWL::Window window(OWL::Vec2ui(1280, 720), "OpenGL Flight Sim");
  window.setContext(context);

  std::cout << "Welcome to the OpenGL Flight Sim!\n";

  int version = gladLoadGLLoader((GLADloadproc)context.getLoaderFunction());
  if (version == 0) {
    std::cout << "Failed to initialize OpenGL context\n";
    return -1;
  }
  std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";
  std::cout << "GPU: " << glGetString(GL_RENDERER) << "\n";

  std::vector<Mesh> meshes = Mesh::loadFromOBJ("./res/outside.obj");
  // Create a shader program that includes a projection, view and model matrix
  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout(location = 0) in vec3 aPos;\n"
      "layout(location = 1) in vec3 aNormal;\n"
      "layout(location = 2) in vec2 aTexCoords;\n"
      "uniform mat4 projection;\n"
      "uniform mat4 view;\n"
      "uniform mat4 model;\n"
      "void main() {\n"
      "  gl_Position = projection *  view * model * vec4(aPos / 2.0, 1.0);\n"
      "}\n";
  const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main() {\n"
      "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n";

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glUseProgram(shaderProgram);

  // Make a simple perspective projection matrix
  float fov = 45.0f;
  float aspect = (float)window.getSize().x / (float)window.getSize().y;
  float near = 0.1f;
  float far = 100.0f;
  float top = near * tan(fov * 0.5f * 3.14159f / 180.0f);
  float bottom = -top;
  float right = top * aspect;
  float left = -right;
  float a = (right + left) / (right - left);
  float b = (top + bottom) / (top - bottom);
  float c = (far + near) / (far - near);
  float d = (2.0f * far * near) / (far - near);
  float projectionMatrix[] = {2.0f * near / (right - left),
                              0.0f,
                              0.0f,
                              0.0f,
                              0.0f,
                              2.0f * near / (top - bottom),
                              0.0f,
                              0.0f,
                              a,
                              b,
                              -c,
                              -1.0f,
                              0.0f,
                              0.0f,
                              -d,
                              0.0f};

  // Set the projection matrix as a uniform
  GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);

  // Create an identity view matrix
  float viewMatrix[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  // Translate the view matrix back
  viewMatrix[14] = -4.0f;

  // Set the view matrix as a uniform
  GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewMatrix);

  // Create an identity model matrix
  float modelMatrix[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  // rotate 45 about Y
  modelMatrix[0] = cos(45.0f * 3.14159f / 180.0f);
  modelMatrix[2] = sin(45.0f * 3.14159f / 180.0f);
  modelMatrix[8] = -sin(45.0f * 3.14159f / 180.0f);
  modelMatrix[10] = cos(45.0f * 3.14159f / 180.0f);

  // Set the model matrix as a uniform
  GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLocation, 1, GL_FALSE, modelMatrix);

  // disable culling
  glDisable(GL_CULL_FACE); // todo remove

  OWL::FPSLimiter eventDelay(60);
  while (window.isRunning()) {
    eventDelay.start();
    window.pollEvents();

    if (window.keyboard.isKeyPressed(window.keyboard.Escape)) {
      window.close();
    }
    if (window.keyboard.getKeyData().keyEnum == window.keyboard.F11) {
      window.setFullScreen(!window.isFullScreen());
    }

    unsigned int uiSize =
        ((window.getSize().x > window.getSize().y) ? window.getSize().y
                                                   : window.getSize().x);

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Mesh &mesh : meshes) {
      mesh.draw();
    }

    context.swapBuffers();
    eventDelay.end();
  }

  return 0;
}