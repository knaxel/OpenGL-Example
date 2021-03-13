#pragma once
#include"Header.h"


 struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};
class Mesh{
private:
public:
	//Vertex * vertexArray;
	//GLuint* indexArray;
	//unsigned nrOfVertices;
	unsigned index_count;

	GLuint VAO;


	Mesh(
		GLuint id,
		const unsigned& index_count)
	{

		this->VAO = id;
		this->index_count = index_count;

	}


	Mesh(const Mesh& obj)
	{
		this->VAO = obj.VAO;
		this->index_count = obj.index_count;


	}

	~Mesh()
	{

	}


	GLuint getVAO()
	{
		return this->VAO;
	}
	GLuint getIndiceCount()
	{
		return this->index_count;
	}
};
class Model {
private:

public:

	Mesh* mesh;
	TexturedMaterial* material;

	Model(Mesh* mesh, TexturedMaterial* material) {
		this->mesh = mesh;
		this->material = material;
	}

};