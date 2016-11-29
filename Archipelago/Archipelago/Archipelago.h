#pragma once

#include "stdHeader.h"
#include "Camera.h"
#include "Water.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Shadows.h"

using namespace std;
using namespace glm;

// Window dimensions
const GLuint WIDTH = 800;
const GLuint HEIGHT = 800;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Objects
Water* water;
const float CAM_DIST_TO_EDGE = 100;
Terrain* terrain;

// Shaders
//////////////////////////////////////////////////////////////////////////
Shader* waterShader;

// Transform
//////////////////////////////////////////////////////////////////////////
mat4 view;
mat4 model;
mat4 projection;

mat4 waterModel;
GLint modelLoc;
GLint viewLoc;
GLint projLoc;
int main(void);
void transformViewProj(Shader *shaders, mat4 modelIn);

//Lighting
///////////////////////////////////////////////////////////////////////////
void lightingSetup(Shader *shaders);
glm::vec3 const SUNLIGHT_DIR(glm::normalize(glm::vec3(sqrt(2.0f), 1.0f, -sqrt(2.0f))));

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

void clearScreenAndColor() {
	// Clear buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Keyboard & Mouse
//////////////////////////////////////////////////////////////////////////
void error_callback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void moveCamera();

// Draws
//////////////////////////////////////////////////////////////////////////
void drawSkyBox(SkyBox &skybox);
void drawObj(Obj *mesh, Shader *shader, mat4 modelIn);
bool incrementWaterSurface();
void renderInitIslandSample();
void renderMoreIslands();


// Collision
//////////////////////////////////////////////////////////////////////////
void detectTerrainCollision();
void calculateTerrainCollision(Terrain* terrain);
bool isCamInTerrain(Terrain* terrain);


