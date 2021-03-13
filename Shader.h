#pragma once
#include"Header.h"

class Shader
{
private:

	std::string loadShaderSource(const char* fileName)
	{
		std::string temp = "";
		std::string src = "";

		std::ifstream in_file(fileName, std::ios::in);

		if ( in_file.is_open())
		{
			while (std::getline(in_file, temp))
				src += temp + "\n";
		}
		else
		{
			//std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << strerror(errno) << "\n";
		}

		in_file.close();
		/*
		std::string versionNr =
			std::to_string(this->versionMajor) +
			std::to_string(this->versionMinor) +
			"0";

		src.replace(src.find("#version"), 12, ("#version " + versionNr));
	*/
		return src;
	}

	GLuint loadShader(GLenum type, const char* fileName)
	{
		char infoLog[512];
		GLint success;

		GLuint shader = glCreateShader(type);
		std::string str_src = this->loadShaderSource(fileName);
		const GLchar* src = str_src.c_str();
		const GLint  csucks = str_src.length();
		glShaderSource(shader, 1, &src,&csucks);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
			std::cout << infoLog << "\n";
		}

		return shader;
	}

	void linkProgram(GLuint vertexShader, GLuint fragmentShader)
	{
		char infoLog[512];
		GLint success;

		this->id = glCreateProgram();

		glAttachShader(this->id, vertexShader);

		glAttachShader(this->id, fragmentShader);

		glLinkProgram(this->id);

		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n";
		}

		glUseProgram(0);
	}

public:
	GLuint id;
	const int versionMajor;
	const int versionMinor;

	//Constructors/Destructors
	Shader(const char* vertexFile,const  char* fragmentFile)
		: versionMajor(0), versionMinor(0)
	{
		GLuint vertexShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);

		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

		this->linkProgram(vertexShader, fragmentShader);
		//End
		//glDeleteShader(vertexShader);
		//glDeleteShader(fragmentShader);
	}


	~Shader()
	{
		glDeleteProgram(this->id);
	}

	//Set uniform functions
	void use()
	{
		glUseProgram(this->id);
	}

	void unuse()
	{
		glUseProgram(0);
	}

	void load1i(GLint value, const GLchar* name)
	{
		this->use();

		glUniform1i(glGetUniformLocation(this->id, name), value);

		this->unuse();
	}

	void load1f(GLfloat value, const GLchar* name)
	{
		this->use();

		glUniform1f(glGetUniformLocation(this->id, name), value);

		this->unuse();
	}

	void loadVec2f(glm::fvec2 value, const GLchar* name)
	{
		this->use();

		glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

		this->unuse();
	}

	void loadVec3f(glm::fvec3 value, const GLchar* name)
	{
		this->use();

		glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

		this->unuse();
	}

	void loadVec4f(glm::fvec4 value, const GLchar* name)
	{
		this->use();

		glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

		this->unuse();
	}

	void loadMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE)
	{
		this->use();

		glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

		this->unuse();
	}

	void loadMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE)
	{
		this->use();

		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

		this->unuse();
	}
	void loadMaterial(Material * material) {

		this->use();
		this->loadVec3f(material->ambient, "material.ambient");
		this->loadVec3f(material->diffuse, "material.diffuse");
		this->loadVec3f(material->specular, "material.specular");

		this->unuse();
	}
	
	void loadTexturedMaterial(TexturedMaterial * material) {

		this->use();
		this->loadVec3f(material->ambient, "material.ambient");
		this->loadVec3f(material->diffuse, "material.diffuse"); 
		this->loadVec3f(material->specular, "material.specular");
		this->load1i(material->texture_diffuse, "material.diffuseTex");
		this->load1i(material->texture_specular, "material.specularTex");

		this->unuse();
	}
	
};