#pragma once
#include "Header.h"

namespace Texture
{
	static GLuint GRASS, STONE,BARK,LEAVES, DIRT, SAND, TALL_GRASS, WATER,SMOOTH_STONE;
};

class GLLoader
{
private:
	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos; 
	std::vector<Mesh * > meshs;

public:
	const std::vector<std::string> explode(const std::string& s, const char& c)
	{
		std::string buff{""};
		std::vector<std::string> v;
		
		for(auto n:s)
		{
			if(n != c) buff+=n; else
			if(n == c && buff != "") { v.push_back(buff); buff = ""; }
		}
		if(buff != "") v.push_back(buff);
		
		return v;
	}
	GLLoader()
	{
	}
	~GLLoader()
	{

		for (GLuint i : vaos)
		{
			glDeleteVertexArrays(1, &i);
		}
		for (GLuint i : vbos)
		{
			glDeleteBuffers(1, &i);
		}
	}
	Mesh *loadToMesh(Vertex *vertices, GLuint *indices, int vertex_count, int index_count)
	{

		GLuint vaoID;
		glCreateVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		GLuint vbo_vertices, vbo_indices;
		glGenBuffers(1, &vbo_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &vbo_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(GLuint), indices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);

		vaos.push_back(vaoID);
		vbos.push_back(vbo_vertices);
		vbos.push_back(vbo_indices);
		return new Mesh(vaoID, index_count);
	}
	Mesh *loadToMesh(const char * path)
	{

		Vertex *vertices; GLuint *indices;
		 int vertex_count=0;
		 int index_count=0;
		std::ifstream file;
		std::string   line;

		std::vector<Vertex> vert;

		std::vector<glm::vec3> raw_ver,raw_norm;
		std::vector<glm::vec2> raw_vt;
		std::vector<GLuint> raw_ind;
		int count = 0; 
		
		file.open( ( path) );

			if ( file.is_open())
			{
				while ( getline ( file, line ))
				{
					std::stringstream ss ( line );

					if ( getline ( ss, line, ','))
					{
						if(line[0] == 'v' && line[1] == ' '){
							std::vector<std::string> v{explode(line, ' ')};
							float x = stof(v.at(1));
							float y = stof(v.at(2));
							float z = stof(v.at(3));
							raw_ver.push_back(glm::vec3(x,y,z));
						}else if(line[0] == 'v' && line[1] == 't'){
							std::vector<std::string> v{explode(line, ' ')};
							float x = stof(v.at(1));
							float y = stof(v.at(2));
							raw_vt.push_back(glm::vec2(x,y));
						}else if(line[0] == 'v' && line[1] == 'n'){
							std::vector<std::string> v{explode(line, ' ')};
							float x = stof(v.at(1));
							float y = stof(v.at(2));
							float z = stof(v.at(3));
							raw_norm.push_back(glm::vec3(x,y,z));
						}else if(line[0] == 'f' && line[1] == ' '){
							



							std::vector<std::string> v{explode(line, ' ')};


							if(v.size() > 4){
								std::vector<std::string> first {explode(v.at(1), '/')};
								std::vector<std::string> second {explode(v.at(2), '/')};
								std::vector<std::string> third {explode(v.at(3), '/')};
								std::vector<std::string> fourth {explode(v.at(4), '/')};


								//std::cout<< v.at(1) << std::endl;
								//std::cout<< v.at(2) << std::endl;
								//std::cout<< v.at(3) << std::endl;
								//std::cout<< first.at(0) << std::endl;
								//std::cout<< first.at(1) << std::endl;
								//std::cout<< first.at(2) << std::endl;


								struct Vertex v1{raw_ver.at(stoi(first.at(0))-1), 
														glm::vec3(1,1,1),
														raw_vt.at(stoi(first.at(1))-1),
														raw_norm.at(stoi(first.at(2))-1)};
								struct Vertex v2{raw_ver.at(stoi(second.at(0))-1), 
														glm::vec3(1,1,1),
														raw_vt.at(stoi(second.at(1))-1),
														raw_norm.at(stoi(second.at(2))-1)};
								struct Vertex v3{raw_ver.at(stoi(third.at(0))-1), 
														glm::vec3(1,1,1),
														raw_vt.at(stoi(third.at(1))-1),
														raw_norm.at(stoi(third.at(2))-1)};
								struct Vertex v4{raw_ver.at(stoi(fourth.at(0))-1), 
														glm::vec3(1,1,1),
														raw_vt.at(stoi(fourth.at(1))-1),
														raw_norm.at(stoi(fourth.at(2))-1)};

								raw_ind.push_back(count);
								raw_ind.push_back(count+1);
								raw_ind.push_back(count+2);

								raw_ind.push_back(count+3);
								raw_ind.push_back(count);
								raw_ind.push_back(count+2);

								count+=4;

								vert.push_back(v1);

								vert.push_back(v2);

								vert.push_back(v3 );
								vert.push_back(v4 );



								continue;
							}
							std::vector<std::string> first {explode(v.at(1), '/')};
							std::vector<std::string> second {explode(v.at(2), '/')};
							std::vector<std::string> third {explode(v.at(3), '/')};


							//std::cout<< v.at(1) << std::endl;
							//std::cout<< v.at(2) << std::endl;
							//std::cout<< v.at(3) << std::endl;
							//std::cout<< first.at(0) << std::endl;
							//std::cout<< first.at(1) << std::endl;
							//std::cout<< first.at(2) << std::endl;

							raw_ind.push_back(count++);
							raw_ind.push_back(count++);
							raw_ind.push_back(count++);

							struct Vertex v1{raw_ver.at(stoi(first.at(0))-1), 
													glm::vec3(1,1,1),
													raw_vt.at(stoi(first.at(1))-1),
													raw_norm.at(stoi(first.at(2))-1)};
							struct Vertex v2{raw_ver.at(stoi(second.at(0))-1), 
													glm::vec3(1,1,1),
													raw_vt.at(stoi(second.at(1))-1),
													raw_norm.at(stoi(second.at(2))-1)};
							struct Vertex v3{raw_ver.at(stoi(third.at(0))-1), 
													glm::vec3(1,1,1),
													raw_vt.at(stoi(third.at(1))-1),
													raw_norm.at(stoi(third.at(2))-1)};

							vert.push_back(v1);

							vert.push_back(v2);

							vert.push_back(v3 );


						}
					}
				}

			}

		vertices = new Vertex[vert.size()];
		vertex_count = vert.size();
		for(int i = 0; i< (int)vert.size(); i++){
			vertices[i] =vert.at(i);
		}

		indices = new GLuint[raw_ind.size()];
		index_count = raw_ind.size();
		for(int i = 0; i< (int)raw_ind.size(); i++){
			indices[i] =raw_ind.at(i);
		}

		GLuint vaoID;
		glCreateVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		GLuint vbo_vertices, vbo_indices;
		glGenBuffers(1, &vbo_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &vbo_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(GLuint), indices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);

		vaos.push_back(vaoID);
		vbos.push_back(vbo_vertices);
		vbos.push_back(vbo_indices);
		return new Mesh(vaoID, index_count);
		return NULL;
	}


	

	void loadTexture(const char *fileName, GLuint *tex)
	{
		if (tex)
		{
			glDeleteTextures(1, tex);
		}
		int width, height;
		unsigned char *image = SOIL_load_image(fileName, &width, &height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, tex);
		glBindTexture(GL_TEXTURE_2D, *tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image!=NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED:" << fileName << "\n";
		}

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);

		return;
	}
};
