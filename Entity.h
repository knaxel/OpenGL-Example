#pragma once
#include"Header.h"

class Entity {
public:
	Model* model;

	bool fakeLighting;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 matrix_transformation;

	Entity(Model* model)  {
		this->position = glm::vec3(0);
		this->origin = glm::vec3(0);
		this->rotation = glm::vec3(0);
		this->scale = glm::vec3(1);
		this->model = model;
		fakeLighting = false;
		this->updateModelMatrix();
	}
	Entity(Model* model, glm::vec3 scale )  {
		this->position = glm::vec3(0);
		this->origin = glm::vec3(0);
		this->rotation = glm::vec3(0);
		this->scale = scale;
		this->model = model;
		fakeLighting = false;
		this->updateModelMatrix();
	}
	Entity()  {
		this->position = glm::vec3(0);
		this->origin = glm::vec3(0);
		this->rotation = glm::vec3(0);
		this->scale = glm::vec3(1);
		this->model = NULL;
		fakeLighting = false;
		this->updateModelMatrix();
	}
	Entity(glm::vec3 position,
		glm::vec3 origin,
		glm::vec3 rotation,
		glm::vec3 scale,
		Model* model) {

		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;

		fakeLighting = false;
		this->model = model;
		this->updateModelMatrix();
	}
	Entity(glm::vec3 position,
		glm::vec3 origin,
		glm::vec3 rotation,
		glm::vec3 scale,
		Model* model, bool f) {

		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;

		fakeLighting = f;
		this->model = model;
		this->updateModelMatrix();
	}
	void updateModelMatrix()
	{
		this->matrix_transformation = glm::mat4(1.f);
		this->matrix_transformation = glm::translate(this->matrix_transformation, this->origin);
		this->matrix_transformation = glm::rotate(this->matrix_transformation, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->matrix_transformation = glm::rotate(this->matrix_transformation, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->matrix_transformation = glm::rotate(this->matrix_transformation, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->matrix_transformation = glm::translate(this->matrix_transformation, this->position - this->origin);
		this->matrix_transformation = glm::scale(this->matrix_transformation, this->scale);
	}
	void setPosition(float x, float y, float z)
	{
		this->position = glm::vec3(x,y,z);
		updateModelMatrix();
	}
	void setPosition(glm::vec3 v)
	{
		this->position = v-origin;
		updateModelMatrix();
	}

	void setOrigin(const glm::vec3 origin)
	{
		this->origin = origin;
		updateModelMatrix();
	}

	void setRotation(const glm::vec3 rotation)
	{
		this->rotation = rotation;
		updateModelMatrix();
	}

	void setScale(const glm::vec3 setScale)
	{
		this->scale = scale;
		updateModelMatrix();
	}

	//Functions

	void move(const glm::vec3 position)
	{
		this->position += position;
		updateModelMatrix();
	}

	void rotate(const glm::vec3 rotation)
	{
		this->rotation += rotation;
		updateModelMatrix();
	}

	void scaleUp(const glm::vec3 scale)
	{
		this->scale += scale;
		updateModelMatrix();
	}


};
