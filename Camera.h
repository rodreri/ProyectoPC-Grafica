#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 worldUp;
	GLfloat yaw;
	GLfloat pitch;


	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void keyControl2(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	void      setPosicion(glm::vec3 pos);
	void	  setDireccion(glm::vec3 front);
	void	  setGiro(float x, float y);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();



	~Camera();

private:
	glm::vec3 up;
	glm::vec3 right;
	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

