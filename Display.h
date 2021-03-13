#pragma once
#include"Header.h"

class WindowManager{
private: 
	GLFWwindow* window;
	int framebufferWidth = 1920;
	int framebufferHeight = 1080;


public: 
	WindowManager(const char* title, bool resizable, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR) {
		initWindow(title, resizable, GL_VERSION_MAJOR, GL_VERSION_MINOR);
	}
	void closeWindow(){

		
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}
	void update(){
		glfwPollEvents();
	}

	void initWindow(const char* title, bool resizable,const int GL_VERSION_MAJOR,const int GL_VERSION_MINOR) {

		//INIT GLFW
		if (glfwInit() == GLFW_FALSE) {
			std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
			glfwTerminate();
		}
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
   		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, resizable);



		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MAC OS

		this->window = glfwCreateWindow(framebufferWidth, framebufferHeight, title, NULL, NULL);
		if (this->window == nullptr) {
			std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
			glfwTerminate();
		}


		glfwMakeContextCurrent(this->window); //IMPORTANT!!
		//glViewport(0, 0, framebufferWidth, framebufferHeight);


		glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
		glfwSetFramebufferSizeCallback(window, WindowManager::framebuffer_resize_callback);
		//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
			glfwTerminate();
		}

		//Input
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}
	//Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
	{
		glViewport(0, 0, fbW, fbH);
	}
	GLFWwindow* getWindow() {
		return this->window;
	}

	int getFBW() {
		return this->framebufferWidth;
	}
	int getFBH() {
		return this->framebufferHeight;
	}
};