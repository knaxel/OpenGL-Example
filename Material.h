#include "Header.h"

class Material{
public:

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	Material(
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular
	)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
	Material(
	float ambient,
	float diffuse,
	float specular
	)
	{
		this->ambient = glm::vec3(ambient);
		this->diffuse = glm::vec3(diffuse);
		this->specular = glm::vec3(specular);
	}
	Material(Material * mat
	)
	{
		this->ambient = mat->ambient;
		this->diffuse = mat->diffuse;
		this->specular = mat->specular;
	}

	~Material(){}

	//Function
};
class TexturedMaterial : public Material{
private:
public:
	GLuint texture_diffuse;
	GLuint texture_specular;


	TexturedMaterial(
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	GLuint  td,
	GLuint  ts
	) : Material(ambient,diffuse,specular)
	{
		texture_diffuse = td;
		texture_specular = ts;
	}
	TexturedMaterial(
	float ambient,
	float diffuse,
	float specular,
	GLuint  td,
	GLuint  ts
	) : Material(ambient,diffuse,specular)
	{
		texture_diffuse = td;
		texture_specular = ts;
	}
	TexturedMaterial(
	Material * mat,
	GLuint  td,
	GLuint  ts
	) : Material(mat)
	{
		texture_diffuse = td;
		texture_specular = ts;
	}

	~TexturedMaterial(){}

	//Function
};