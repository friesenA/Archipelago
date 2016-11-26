/**
* COMP371
* Archipelago
* Team 7
*/

#include "Archipelago.h"

//Camera facing forward z = -1;
float camYLoc = 17.0f;
Camera camera(glm::vec3(0.0f, camYLoc, 0.0f));


Shadows shadows;

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

	// Shaders
	//////////////////////////////////////////////////////////////////////////
	waterShader = new Shader("Shaders/waterVertex.shader", "Shaders/waterFragment.shader");
	Shader terrainShader("Shaders/terrainVertex.shader", "Shaders/terrainFragment.shader");

	// Object Creation
	//////////////////////////////////////////////////////////////////////////
	water = new Water(15.0f);
	Terrain t(90);
	t.setModel(model);
	terrains.push_back(t);

	// Skybox
	//////////////////////////////////////////////////////////////////////////
	SkyBox skybox;
	skybox.generate();

	// OpenGL Settings
	//////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB); //gamma correction
								   // Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Register Shadows
	//////////////////////////////////////////////////////////////////////////

	shadows.setupFrameBuffer();
	shadows.initializeShadowMap();

	//Draw Obj instances
	shadows.drawObj(water, SUNLIGHT_DIR);
	shadows.drawObj(&terrains.front(), SUNLIGHT_DIR);

	shadows.endShadowMap();

	// Viewport / needs to be set after the shadow map created
	//////////////////////////////////////////////////////////////////////////
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Game loop
	//////////////////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		moveCamera();
		clearScreenAndColor();

		projection = perspective(radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f); //global for all draws
		drawSkyBox(skybox);//Skybox must be drawn first

		//Setup view used for the rest of the scene
		view = camera.getViewMatrix();
		glDepthMask(GL_TRUE);

		// Draw terrain instance
		drawObj(&terrains.front(), &terrainShader, model);
		//Draw water instance
		drawObj(water, waterShader, waterModel);

		glfwSwapBuffers(window);
	}

	// Clean up
	//////////////////////////////////////////////////////////////////////////
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

//Draw Obj
//////////////////////////////////////////////////////////////////////////
void drawObj(Obj *mesh, Shader* shader, mat4 modelIn) {
	shader->Use();
	transformViewProj(shader, modelIn);
	lightingSetup(shader);
	mesh->draw();
}

void incrementWaterSurface() {
	float distanceToEdge = 100;
	if (water->getLength() - abs(camera.getPosition().x) <= distanceToEdge || water->getLength() - abs(camera.getPosition().z) <= distanceToEdge) {
		float foo = water->getLength();
		waterModel = glm::scale(waterModel, glm::vec3(1.3f, 1.0f, 1.0f));
		waterModel = glm::scale(waterModel, glm::vec3(1.0f, 1.0f, 1.3f));
		water->incrementSurface(1.3);
	}
}

// Transform
//////////////////////////////////////////////////////////////////////////
void transformViewProj(Shader *shaders, mat4 modelIn) {
	projLoc = glGetUniformLocation(shaders->Program, "projection");
	viewLoc = glGetUniformLocation(shaders->Program, "view");
	modelLoc = glGetUniformLocation(shaders->Program, "model");

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelIn));
}

// Lighting
////////////////////////////////////////////////////////////////////////////
void lightingSetup(Shader *shaders) {
	glm::vec3 camPos = camera.getPosition();

	GLint lightDir = glGetUniformLocation(shaders->Program, "lightDirection");
	GLint lightCol = glGetUniformLocation(shaders->Program, "lightColor");
	GLint viewPos = glGetUniformLocation(shaders->Program, "viewerPos");

	//light attributes
	glUniform3f(lightDir, SUNLIGHT_DIR.x, SUNLIGHT_DIR.y, SUNLIGHT_DIR.z);
	glUniform3f(lightCol, 1.0f, 1.0f, 1.0f);
	glUniform3f(viewPos, camPos.x, camPos.y, camPos.z);

	//Shadow texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadows.getShadowMapTexture());
	glUniform1i(glGetUniformLocation(shaders->Program, "shadowTexture"), 0);

	GLint lightCTMLoc = glGetUniformLocation(shaders->Program, "lightSpaceMatrix");
	glUniformMatrix4fv(lightCTMLoc, 1, GL_FALSE, glm::value_ptr(shadows.getLightSpaceMatrix()));
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
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
		incrementWaterSurface();
	}
	if (keys[GLFW_KEY_S]) {
		camera.translateCamera(BACKWARD);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
		incrementWaterSurface();
	}
	if (keys[GLFW_KEY_A]) {
		camera.translateCamera(LEFT);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
		incrementWaterSurface();
	}
	if (keys[GLFW_KEY_D]) {
		camera.translateCamera(RIGHT);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
		incrementWaterSurface();
	}

	//included for debugging purposes
	if (keys[GLFW_KEY_UP]) {
		camera.translateCamera(UP);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
		incrementWaterSurface();
	}

	if (keys[GLFW_KEY_DOWN]) {
		camera.translateCamera(DOWN);
		cout << camera.getPosition().x << " , " << camera.getPosition().z << endl;
		incrementWaterSurface();
	}

	detectTerrainCollision();
}

void detectTerrainCollision() {
	for (vector<Terrain>::iterator terrain = terrains.begin(); terrain != terrains.end(); terrain++) {
		vec3 terrainPosition(terrain->getModel()[3]);
		vec3  nexPosition = camera.getPosition() + camera.getNextPosition();

		// Determine if cam is in terrain
		bool camIsInTerrain = abs(camera.getPosition().x) <= terrainPosition.x + (terrain->getWidth()/2) &&
							  abs(camera.getPosition().z)<= terrainPosition.z + (terrain->getLength()/2);

		if (camIsInTerrain) {
			cout <<"Cam: "<< camera.getPosition().x << " , " << camera.getPosition().z << endl;
			//// Get terrain height at location
			int a = (int)nexPosition.z;
			int b = (int)(terrain->getLength() / 2);
			int c = (int)terrain->getWidth();
			int d = (int)nexPosition.x;
			int e = ((int)terrain->getWidth() / 2);


			// current position
			int foobar = ((int)camera.getPosition().z + (int)(terrain->getLength() / 2)) *  (int)terrain->getWidth();
			int currentLoc = (int)camera.getPosition().x + ((int)terrain->getWidth() / 2) + foobar;

			// next position
			int foo = ((int)nexPosition.z + (int)(terrain->getLength() / 2)) *  (int)terrain->getWidth();
			int theLocation = (int)nexPosition.x + ((int)terrain->getWidth() / 2) + foo;

			try {
				float bar = terrain->getVertices().at(theLocation).y;
				float barfoo = terrain->getVertices().at(currentLoc).y;
				bar = (barfoo + bar) / 2;

				cout << "the at loc: " << theLocation << endl;
				cout<< "height at loc: "<< bar <<endl;
				if (bar > camYLoc) {
					float offset = 1.5f;
					camera.climbAt(bar + offset);
				}else {
					//camera.climbAt(camYLoc);
				}

			}catch (exception e) {
				cout << "not found!" << endl;
			}
			//cout << "cam is in terrain " << "x:" << nextCamPosition.x << " terrain y:" << nextCamPosition.y << " z:" <<nextCamPosition.z << endl;
		}

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

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}