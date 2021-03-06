/**
* COMP371
* Archipelago
* Team 7
*/

#include "Archipelago.h"
#include <thread>
#include <ctime>

//Camera facing forward z = -1;
float camStartingYLoc = 17.0f;
Camera camera(glm::vec3(0.0f, camStartingYLoc, 0.0f));
vector<Terrain> islandBuffer;
Shadows shadows;

int width, height;

//Key tracking
bool keys[1024];

//Mouse tracking
bool initializeMouse = true;
GLfloat lastX;
GLfloat lastY;

float t = 0;

//RANDOM NUMBER

int main(void) {

	//Console Request
	///////////////////////////////////////////////////////////////////////////
	unsigned int seed;
	std::cout << "Please enter a seed: ";
	std::cin >> seed;

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
	terrain = new Terrain(seed);

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
	shadows.drawObj(terrain , SUNLIGHT_DIR);

	shadows.endShadowMap();

	// Viewport / needs to be set after the shadow map created
	//////////////////////////////////////////////////////////////////////////
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// Game loop
	//////////////////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		moveCamera();
		clearScreenAndColor();

		projection = perspective(radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f); //global for all draws
		drawSkyBox(skybox);//Skybox must be drawn first

		//Setup view used for the rest of the scene
		view = camera.getViewMatrix();
		glDepthMask(GL_TRUE);

		// Draw terrain instance
		drawObj(terrain, &terrainShader, glm::mat4(1.0f));

		//Draw water instance
		drawObj(water, waterShader, waterModel);

		glfwSwapBuffers(window);

		t += 0.001;
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
	GLint waterTime = glGetUniformLocation(shader->Program, "time");
	 glUniform1f(waterTime, t);
	transformViewProj(shader, modelIn);
	lightingSetup(shader);
	mesh->draw();
}

bool incrementWaterSurface() {
	if (water->getLength() - abs(camera.getPosition().x) <= CAM_DIST_TO_EDGE || water->getLength() - abs(camera.getPosition().z) <= CAM_DIST_TO_EDGE) {
		waterModel = glm::scale(waterModel, glm::vec3(1.3f, 1.0f, 1.3f));
		water->incrementSurface(1.3);
		return true;
	}

	return false;
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
		cout << camera.getPosition().x << " , " << camera.getPosition().y << " , " << camera.getPosition().z << endl;
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

// Collision
//////////////////////////////////////////////////////////////////////////
bool isCamInTerrain(Terrain* terrain) {

	// Determine if cam will be in terrain
	bool camIsInTerrain = (camera.getPosition().x <= (terrain->getWidth() / 2)) && (camera.getPosition().x >= -(terrain->getWidth() / 2)) &&
		(camera.getPosition().z <= (terrain->getLength() / 2)) && (camera.getPosition().z >= -(terrain->getLength() / 2));

	return camIsInTerrain;
}

void detectTerrainCollision() {
	if (isCamInTerrain(terrain)) {
		calculateTerrainCollision(terrain);
		return;
	}
		calculateTerrainCollision(terrain);
}

void calculateTerrainCollision(Terrain* terrain) {
	vec3 nexPosition = camera.getPosition() + camera.getNextPosition();

	if (!isCamInTerrain(terrain)) {
		return; 
	}

	// Remove translation effect and find location vertices vector
	// current position
	int lineOne = ((int)(camera.getPosition().z < 0 ? camera.getPosition().z : camera.getPosition().z) + (int)(terrain->getLength() / 2)) *  (int)terrain->getWidth();
	int currentLoc = (camera.getPosition().x < 0 ? camera.getPosition().x : camera.getPosition().x) + ((int)terrain->getWidth() / 2) + lineOne;

	// next position
	int lineTwo = ((int)(nexPosition.z < 0 ? nexPosition.z : nexPosition.z) + (int)(terrain->getLength() / 2)) *  (int)terrain->getWidth();
	int nextLoc = (nexPosition.x < 0 ? nexPosition.x : nexPosition.x) + ((int)terrain->getWidth() / 2) + lineTwo;

	try {
		float offset = 2.0f;
		float pos = terrain->getVertices()->at(abs(currentLoc)).y;
		float next = terrain->getVertices()->at(abs(nextLoc)).y;
		pos = (next + pos) / 2;
		if (pos > camStartingYLoc) {
			camera.climbAt(pos + offset);
		}
	}
	catch (exception e) {
		cout << "Location not found! cur" << currentLoc << " next "<< nextLoc  <<endl;
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

void framebuffer_size_callback(GLFWwindow * window, int w, int h) {
	glViewport(0, 0, w, h);
	projection = perspective(radians(45.0f), (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
	width = w;
	height = h;

}