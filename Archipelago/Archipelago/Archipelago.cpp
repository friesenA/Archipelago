/**
 * COMP371
 * Archipelago
 * Team 7
 */

#include "Archipelago.h"
#include "SkyBox.h"

//Camera facing down y = -1;
Camera camera(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


//Camera facing forward z = -1;
//Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

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
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

	// Shader
	//////////////////////////////////////////////////////////////////////////
	Shader shader("Shaders/vertex.shader", "Shaders/fragment.shader");

	// Object Creation
	//////////////////////////////////////////////////////////////////////////
	Water water(2.5f);

	// Skybox
	//////////////////////////////////////////////////////////////////////////
	SkyBox skybox;
	skybox.generate();

	// Game loop
	//////////////////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		moveCamera();

		// Clear buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Camera
		projection = perspective(radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
		view = camera.getViewMatrix();
		

		skybox.draw(view, projection);

		//Foo water instance
		shader.Use();
		view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getForward(), glm::vec3(0.0f, 1.0f, 0.0f));
		transformViewProj(&shader);

		glBindVertexArray(water.getVAO());
		glDrawElements(GL_TRIANGLES, water.getNumIndices(), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 20);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	// Clean up
	//////////////////////////////////////////////////////////////////////////
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


// Transform
//////////////////////////////////////////////////////////////////////////
void transformViewProj(Shader *shaders) {
	projLoc = glGetUniformLocation(shaders->Program, "projection");
	viewLoc = glGetUniformLocation(shaders->Program, "view");
	modelLoc = glGetUniformLocation(shaders->Program, "model");

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void drawSkyBox() {

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
	if (keys[GLFW_KEY_W]) {
		camera.translateCamera(FORWARD);
		cout << camera.getPosition().x  << " , " << camera.getPosition().z << endl;
	}
	if (keys[GLFW_KEY_S]) {
		camera.translateCamera(BACKWARD);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
	}
	if (keys[GLFW_KEY_A]) {
		camera.translateCamera(LEFT);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
	}
	if (keys[GLFW_KEY_D]) {
		camera.translateCamera(RIGHT);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
	}

	//included for debugging purposes
	if (keys[GLFW_KEY_UP]) {
		camera.translateCamera(UP);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
	}

	if (keys[GLFW_KEY_DOWN]) {
		camera.translateCamera(DOWN);

		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
	}
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

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		camera.rotateCamera(xOffset, yOffset);
	}
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height){
	glViewport(0, 0, width, height);
}
