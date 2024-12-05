#include <OWL/Main.hpp>
#include <OWL/OWL.hpp>
#include <OWL/Time.hpp>
#include <iostream>

#include <glad/glad.h>

#include <OWL/OpenGL.hpp>

#include <vector>

#include "./mesh.hpp"
#include <util/pch.hpp>

#include <joystick/joystick.hpp>

void projection(float m[16], float aspect) {
  // Make a simple perspective projection matrix
  float fov = 45.0f;
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
  std::copy(projectionMatrix, projectionMatrix + 16, m);
}

int main(int argc, char **args) {
  OWL::Window window(OWL::Vec2ui(1280, 720), "OpenGL Flight Sim");
  OWL::OpenGLContext context;
  window.setContext(context);

  Joystick joy;

  std::cout << "Welcome to the OpenGL Flight Sim!\n";

  Math::Mat4f proj = {1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 2.0f, 3.0f, 4.0f};
  std::cout << proj << std::endl << proj.Transpose();

  int version = gladLoadGLLoader((GLADloadproc)context.getLoaderFunction());
  if (version == 0) {
    std::cout << "Failed to initialize OpenGL context\n";
    return -1;
  }
  std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";
  std::cout << "GPU: " << glGetString(GL_RENDERER) << "\n";

  std::vector<Mesh> meshes = Mesh::loadFromOBJ("./res/outside.obj");
  // Create a shader program that includes a projection, view and model matrix
  std::string vertexShaderSource = Util::loadFileAsString("./res/std-mat.vs");
  std::string fragmentShaderSource = Util::loadFileAsString("./res/std-mat.fs");

  const char *vss = &vertexShaderSource[0];
  const char *fss = &fragmentShaderSource[0];

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vss, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fss, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glUseProgram(shaderProgram);

  float projectionMatrix[16];
  projection(projectionMatrix, window.getAspect());

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
  glEnable(GL_CULL_FACE); 
  glEnable(GL_DEPTH_TEST);

  // remember window size so you don't refresh the projection matrix when it's
  // not needed
  OWL::Vec2ui winSizePrev = window.getSize();

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

    if (window.getSize() != winSizePrev) {
      glViewport(0, 0, window.getSize().x, window.getSize().y);

      projection(projectionMatrix, window.getAspect());

      // Set the projection matrix as a uniform
      glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
    }
    winSizePrev = window.getSize();

    glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Mesh &mesh : meshes) {
      mesh.draw();
    }

    context.swapBuffers();
    eventDelay.end();
  }

  return 0;
}