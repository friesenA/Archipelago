/**
 * COMP371
 * Archipelago
 * Team 7
 */

#include "Archipelago.h"

//Camera facing down y = -1;
Camera camera(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


//Camera facing forward z = -1;
//Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

//Key tracking
bool keys[1024];

//Mouse tracking
bool initializeMouse = true;
bool clickedLeftButton = false;
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
	glfwSetMouseButtonCallback(window, mouse_button_callback);

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
	skyboxShader = new Shader("Shaders/skyBoxVertex.shader", "Shaders/skyBoxFragment.shader");

	// Object Creation
	//////////////////////////////////////////////////////////////////////////
	Water water(10.0f);

	// Skybox
	//////////////////////////////////////////////////////////////////////////
	generateSkybox();

	// Game loop
	//////////////////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		// Clear buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Camera
		projection = perspective(radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		view = camera.getViewMatrix();
		moveCamera();
		

		

		// Draw skybox first
		glEnable(GL_DEPTH_CLAMP);
		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader->Use();
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skyboxShader->Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		
		

		//Foo water instance
		shader.Use();
		transformViewProj(&shader);

		glBindVertexArray(water.getVAO());
		glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 20);
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

	if (clickedLeftButton) {
		camera.rotateCamera(xOffset, yOffset);
	}
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		clickedLeftButton = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		clickedLeftButton = false;
	}
}


void generateSkybox() {
	//skybox
	vector<const GLchar*> faces;
	faces.push_back("Images/right1.jpg");
	faces.push_back("Images/left1.jpg");
	faces.push_back("Images/top1.jpg");
	faces.push_back("Images/bottom1.jpg");
	faces.push_back("Images/back1.jpg");
	faces.push_back("Images/front1.jpg");
	cubemapTexture = loadCubemap(faces);


	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// Setup skybox VAO
	GLuint skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}


GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
