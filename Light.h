#pragma once
#include"Header.h"
class Light
{
protected:

public:
	float intensity;
	glm::vec3 color;
	Light(float intensity, glm::vec3 color)
	{
		this->intensity = intensity;
		this->color = color;
	}

	~Light()
	{

	}

};

class PointLight : public Light
{
protected:

public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	PointLight(glm::vec3 position, float intensity = 1.f, glm::vec3 color = glm::vec3(1.f),
		float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f)
		: Light(intensity, color)
	{
		this->position = position;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
	}

	~PointLight()
	{

	}

	void setPosition(const glm::vec3 position)
	{
		this->position = position;
	}

};