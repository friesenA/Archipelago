#pragma once

#include "stdHeader.h"
#include "Camera.h"
#include "Water.h"
#include "Terrain.h"

using namespace std;
using namespace glm;

// Window dimensions
const GLuint WIDTH = 800;
const GLuint HEIGHT = 800;

// Transform
//////////////////////////////////////////////////////////////////////////
mat4 view;
mat4 model;
mat4 projection;
GLint modelLoc;
GLint viewLoc;
GLint projLoc;
void transformViewProj(Shader *shaders);

//////////////////////////////////////////////////////////////////////////////OpenGL
/**
* Initial GLFW window hints
*/
void initGLFWHints() {
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
};

/**
* Get a GLFW window instance
*/
GLFWwindow* getWindowInstance() {
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Archipelago", NULL, NULL);

	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	return window;
};

// Keyboard & Mouse
//////////////////////////////////////////////////////////////////////////
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void error_callback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void moveCamera();



