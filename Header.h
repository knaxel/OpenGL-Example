#pragma once

#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>



#include<GLFW/glfw3.h>

#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include<FastNoiseLite.h>
#include<SOIL2.h>

#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map> 
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<functional>
#include<chrono>
#include<sstream>


#include"Display.h"
#include"Light.h"
#include"Material.h"
#include"Model.h"
#include"Camera.h"
#include"Shader.h"
#include"Entity.h"
#include"Loaders.h"
#include"World.h"
#include"RenderManager.h"
