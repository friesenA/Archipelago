#include "Shadows.h"



Shadows::Shadows()
{
}

void Shadows::initializeShadowMap()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFB);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadows::drawObj(Obj *mesh, glm::vec3 lightDir)
{
	Shader shadowShader("Shaders/shadowVertex.shader", "Shaders/shadowFragment.shader");  //need to move to attributes of class
	
	shadowShader.Use();
	setupLightSpaceMatrix(lightDir);
	
	lightLoc = glGetUniformLocation(shadowShader.Program, "lightSpaceMatrix");
	glUniformMatrix4fv(lightLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	mesh->draw();
}

void Shadows::endShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Shadows::getShadowMapTexture()
{
	return shadowMapTexture;
}

glm::mat4 Shadows::getLightSpaceMatrix()
{
	return lightSpaceMatrix;
}

Shadows::~Shadows()
{
}

void Shadows::setupFrameBuffer()
{
	glGenFramebuffers(1, &shadowFB);

	this->setupDepthTexture();

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
	}
}

void Shadows::setupDepthTexture()
{
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Shadows::setupLightSpaceMatrix(glm::vec3 lightDir)
{
	//Ortho Projection
	GLfloat near_plane = 0.1f, far_plane = 1000.0f;
	glm::mat4 lightProjection = glm::ortho(-800.0f, 800.0f, -800.0f, 800.0f, near_plane, far_plane);

	//View from Light Source
	float factor = 100.0f;//needs testing
	glm::mat4 lightView = glm::lookAt(factor * lightDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	lightSpaceMatrix = lightProjection * lightView;
}
