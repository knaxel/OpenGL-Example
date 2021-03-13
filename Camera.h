#pragma once
#include"Header.h"
class Inputable {
private:

	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	bool firstMouse;

protected:
	GLfloat movementSpeed;
	GLfloat sensitivity;
	double mouseOffsetX;
	double mouseOffsetY;

	 void super(GLFWwindow* window) {

		
		glfwGetCursorPos(window, &mouseX, &this->mouseY);

		if (this->firstMouse)
		{
			this->lastMouseX = this->mouseX;
			this->lastMouseY = this->mouseY;
			this->firstMouse = false;
		}

		//Calc offset
		this->mouseOffsetX = this->mouseX - this->lastMouseX;
		this->mouseOffsetY = this->lastMouseY - this->mouseY;

		//Set last X and Y
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;

		//Move light
	}
};

enum direction {FORWARD = 0, BACKWARD, LEFT, RIGHT,UP,DOWN};

class Camera : Inputable
{
private:

	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	float fov;
	float nearPlane;
	float farPlane;

	void updateCameraVectors()
	{
		this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.y = sin(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
	{

		movementSpeed = 10.f;
		sensitivity = 1.f;
		this->worldUp = worldUp;
		this->position = position;
		this->right = glm::vec3(0.f);
		this->up = worldUp;

		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;


		this->fov = 90.f;
		this->nearPlane = 0.1f;
		this->farPlane = 10000.f;



		this->updateCameraVectors();
	}
	Camera()
	{


		movementSpeed = 100.f;
		sensitivity = 1.0f;
		this->worldUp = glm::vec3(0.f,1.f,0.f);
		this->position = glm::vec3(0.f);
		this->right = glm::vec3(0.f);
		this->up = worldUp;

		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;


		this->fov = 90.f;
		this->nearPlane = 0.1f;
		this->farPlane = 10000000.f;



		this->updateCameraVectors();
	}

	Camera(glm::vec3 position)
	{


		movementSpeed = 100.f;
		sensitivity = 1.0f;
		this->worldUp = glm::vec3(0.f,1.f,0.f);
		this->position = position;
		this->right = glm::vec3(0.f);
		this->up = worldUp;

		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;


		this->fov = 90.f;
		this->nearPlane = 0.1f;
		this->farPlane = 10000000.f;



		this->updateCameraVectors();
	}

	~Camera() {}

	//Accessors
	const glm::mat4 getViewMatrix()
	{
		this->updateCameraVectors();

		return glm::lookAt(this->position, this->position + this->front, this->up);
	}
	const glm::mat4 getProjectionMatrix(int framebufferWidth, int framebufferHeight)
	{
		return glm::perspective(
			glm::radians(this->fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			this->nearPlane,
			this->farPlane
		);
	}

	const glm::vec3 getPosition() const
	{
		return this->position;
	}

	//Functions
	void move(const float& dt, const int direction)
	{
		//Update position vector
		switch (direction)
		{
		case FORWARD:
			this->position += this->front * this->movementSpeed * dt;
			break;
		case BACKWARD:
			this->position -= this->front * this->movementSpeed * dt;
			break;
		case LEFT:
			this->position -= this->right * this->movementSpeed * dt;
			break;
		case RIGHT:
			this->position += this->right * this->movementSpeed * dt;
			break;
		case UP:
			this->position += this->up * this->movementSpeed * dt;
			break;
		case DOWN:
			this->position += -this->up * this->movementSpeed * dt;
			break;
		default:
			break;
		}
	}

	void updateInput(GLFWwindow * w, const float& dt) {
		super(w);
		//Update pitch yaw and roll
		this->pitch += (mouseOffsetY) * this->sensitivity * dt;
		this->yaw += (mouseOffsetX) * this->sensitivity * dt;

		if (this->pitch > 80.f)
			this->pitch = 80.f;
		else if (this->pitch < -80.f)
			this->pitch = -80.f;

		if (this->yaw > 360.f || this->yaw < -360.f)
			this->yaw = 0.f;


		//Program
		if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(w, GLFW_TRUE);
		}

		//Camera
		if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
		{
			move(dt, FORWARD);
		}
		if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
		{
			move(dt, BACKWARD);
		}
		if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
		{
			move(dt, LEFT);
		}
		if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
		{
			move(dt, RIGHT);
		}
		if (glfwGetKey(w, GLFW_KEY_C) == GLFW_PRESS)
		{
			move(dt, DOWN);
		}
		if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			move(dt, UP);
		}

	}
};