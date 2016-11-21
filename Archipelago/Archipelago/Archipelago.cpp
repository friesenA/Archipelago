/**
 * COMP371
 * Archipelago
 * Team 7
 */

#include "Archipelago.h"

glm::vec3 const SUNLIGHT_DIR(glm::normalize(glm::vec3(1.0f, 2.0f*sqrt(2.0f), -1.0f)));

//Camera facing forward z = -1;
Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));

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

	// Shaders
	//////////////////////////////////////////////////////////////////////////
	Shader waterShader("Shaders/waterVertex.shader", "Shaders/waterFragment.shader");
	Shader terrainShader("Shaders/terrainVertex.shader", "Shaders/terrainFragment.shader");

	// Object Creation
	//////////////////////////////////////////////////////////////////////////
	
	Water water(2.0f);
	Terrain terrain(63);

	// Skybox
	//////////////////////////////////////////////////////////////////////////
	SkyBox skybox;
	skybox.generate();


	// OpenGL Settings
	//////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);

	// Game loop
	//////////////////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		clearScreenAndColor();
		moveCamera();

		projection = perspective(radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f); //global for all draws
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//Skybox must be drawn first
		drawSkyBox(skybox);

		//Setup view used for the rest of the scene
		view = camera.getViewMatrix();
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		//Foo water instance
		waterShader.Use();
		transformViewProj(&waterShader);
		lightingSetup(&waterShader);

		// Draw water instance
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(water.getVAO());
		glDrawElements(GL_TRIANGLES, water.getNumIndices(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Draw terrain instance
		terrainShader.Use();
		transformViewProj(&terrainShader);
		lightingSetup(&terrainShader);

//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(terrain.getVAO());
		glDrawElements(GL_TRIANGLES, terrain.getNumIndices(), GL_UNSIGNED_INT, 0);
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

// Lighting
////////////////////////////////////////////////////////////////////////////
void lightingSetup(Shader *shaders) {
	glm::vec3 camPos = camera.getPosition();

	GLint lightDir = glGetUniformLocation(shaders->Program, "lightDirection");
	GLint lightCol = glGetUniformLocation(shaders->Program, "lightColor");
	GLint viewPos = glGetUniformLocation(shaders->Program, "viewerPos");

	glUniform3f(lightDir, SUNLIGHT_DIR.x, SUNLIGHT_DIR.y, SUNLIGHT_DIR.z);
	glUniform3f(lightCol, 1.0f, 1.0f, 1.0f);
	glUniform3f(viewPos, camPos.x, camPos.y, camPos.z);

}

void drawSkyBox(SkyBox &skybox) {
	view = glm::mat4(glm::mat3(camera.getViewMatrix()));
	skybox.draw(view, projection);
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
