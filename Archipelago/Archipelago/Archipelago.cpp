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

//Key tracking
bool keys[1024];

//Mouse tracking
bool initializeMouse = true;
GLfloat lastX;
GLfloat lastY;

float t = 0;

//RANDOM NUMBER

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
	renderInitIslandSample();

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
		for (vector<Terrain>::iterator terrain = terrains.begin(); terrain != terrains.end(); terrain++) {
			drawObj(&(*terrain), &terrainShader, terrain->getModel());
		}

		//Draw water instance
		drawObj(water, waterShader, waterModel);

		glfwSwapBuffers(window);

		t += 0.001f;
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
		renderMoreIslands();
		return true;
	}

	return false;
}

void renderInitIslandSample() {
	srand(time(0));

	int randomNum1 = rand();
	int randomNum2 = rand();
	int randomNum3 = rand();
	int randomNum4 = rand();
	int randomNum5 = rand();



	cout << "Random number is: " << randomNum1<<endl;
	Terrain t(randomNum1);

	cout << "Random number is: " << randomNum2 << endl;
	Terrain t1(randomNum2);

	cout << "Random number is: " << randomNum3 << endl;
	Terrain t2(randomNum3);

	cout << "Random number is: " << randomNum4 << endl;
	Terrain t3(randomNum4);

	cout << "Random number is: " << randomNum5 << endl;
	Terrain t4(randomNum5);

	mat4 mod, mod1, mod2, mod3;
	mod =  translate(mod, vec3(t.getWidth(), 0, t.getWidth()));
	mod1 = translate(mod1, vec3(t.getWidth(), 0, -t.getWidth()));
	mod2 = translate(mod2, vec3(-t.getWidth(), 0, t.getWidth()));
	mod3 = translate(mod3, vec3(-t.getWidth(), 0, -t.getWidth()));

	t1.setModel(mod1);
	t2.setModel(mod2);
	t3.setModel(mod3);
	t4.setModel(mod);

	terrains.push_back(t);
	terrains.push_back(t1);
	terrains.push_back(t2);
	terrains.push_back(t3);
	terrains.push_back(t4);
}

void renderMoreIslands() {
	srand(time(0));
	Terrain t(rand());
	float xOffset = camera.getPosition().x + (camera.getPosition().x/4);
	float zOffset = camera.getPosition().z + (camera.getPosition().z / 4);
	mat4 mod = translate(mod,vec3(xOffset,0, zOffset));
	t.setModel(mod);
	terrains.push_back(t);
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
	vec3 terrainPosition(terrain->getModel()[3]);

	// Determine if cam will be in terrain
	bool camIsInTerrain = (camera.getPosition().x <= (terrainPosition.x + (terrain->getWidth() / 2)) && camera.getPosition().x >= (terrainPosition.x - (terrain->getWidth() / 2))) &&
		(camera.getPosition().z <= (terrainPosition.z + (terrain->getLength() / 2)) && camera.getPosition().z >= (terrainPosition.z - (terrain->getLength() / 2)));

	return camIsInTerrain;
}

void detectTerrainCollision() {
	if (currentTerrain != nullptr) {
		Terrain * terrain = currentTerrain;

		if (isCamInTerrain(terrain)) {
			calculateTerrainCollision(currentTerrain);
			return;
		}
	}

	for (vector<Terrain>::iterator terrain = terrains.begin(); terrain != terrains.end(); terrain++) {
		calculateTerrainCollision(&(*terrain));
	}
}

void calculateTerrainCollision(Terrain* terrain) {
	vec3 terrainPosition(terrain->getModel()[3]);
	vec3 nexPosition = camera.getPosition() + camera.getNextPosition();

	if (!isCamInTerrain(terrain)) {
		currentTerrain = nullptr;
		return; 
	}
	currentTerrain = terrain;

	// Remove translation effect and find location vertices vector
	// current position
	int lineOne = ((int)(camera.getPosition().z < 0 ? camera.getPosition().z + abs(terrainPosition.z) : camera.getPosition().z - terrainPosition.z) + (int)(terrain->getLength() / 2)) *  (int)terrain->getWidth();
	int currentLoc = (camera.getPosition().x < 0 ? camera.getPosition().x + abs(terrainPosition.x) : camera.getPosition().x - terrainPosition.x) + ((int)terrain->getWidth() / 2) + lineOne;

	// next position
	int lineTwo = ((int)(nexPosition.z < 0 ? nexPosition.z + abs(terrainPosition.z) : nexPosition.z - terrainPosition.z) + (int)(terrain->getLength() / 2)) *  (int)terrain->getWidth();
	int nextLoc = (nexPosition.x < 0 ? nexPosition.x + abs(terrainPosition.x) : nexPosition.x - terrainPosition.x) + ((int)terrain->getWidth() / 2) + lineTwo;

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
		currentTerrain = nullptr;
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