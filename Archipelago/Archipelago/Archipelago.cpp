/**
 * COMP371
 * Archipelago
 * Team 7
 */

#include "stdHeader.h"
#include "Archipelago.h"
#include "Camera.h"
#include "Water.h"
#include "Terrain.h"

using namespace std;

// Window dimensions
const GLuint WIDTH = 800;
const GLuint HEIGHT = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Key tracking
bool keys[1024];

//Mouse tracking
bool initializeMouse = true;
GLfloat lastX;
GLfloat lastY;

int main(void) {

	// OpenGL Spec
	initGLFWHints();

	// OpenGl window creation
	glfwInit();
	GLFWwindow* window = getWindowInstance();

	// Setup window
	//////////////////////////////////////////////////////////////////////////
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);

	// GLEW
	//////////////////////////////////////////////////////////////////////////
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}

	// Viewport
	//////////////////////////////////////////////////////////////////////////
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	// Game loop
	//////////////////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		moveCamera();

		glm::mat4 view_matrix;
		view_matrix = camera.getViewMatrix();
		
	
		glfwSwapBuffers(window);
	}

	// Clean up
	//////////////////////////////////////////////////////////////////////////
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


// GLFW
//////////////////////////////////////////////////////////////////////////

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
}

// Keyboard
//////////////////////////////////////////////////////////////////////////
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

//Seperates key event from callback to handle multiple key presses
void moveCamera() {
	if (keys[GLFW_KEY_W])
		camera.translateCamera(FORWARD);

	if (keys[GLFW_KEY_S])
		camera.translateCamera(BACKWARD);

	if (keys[GLFW_KEY_A])
		camera.translateCamera(LEFT);

	if (keys[GLFW_KEY_D])
		camera.translateCamera(RIGHT);

	//included for debugging purposes
	if (keys[GLFW_KEY_UP])
		camera.translateCamera(UP);

	if (keys[GLFW_KEY_DOWN])
		camera.translateCamera(DOWN);
}

// Mouse
//////////////////////////////////////////////////////////////////////////////
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	
	if (initializeMouse)
	{
		lastX = xpos;
		lastY = ypos;
		initializeMouse = false;
	}

	GLfloat xOffset = xpos - lastX;
	GLfloat yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.rotateCamera(xOffset, yOffset);
}
