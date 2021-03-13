#include "Header.h"

int main()
{

	WindowManager windowManager("My game", true, 3, 3);

	GLLoader glloader;

	Camera camera(glm::vec3(0,100,0)); //();


	
	glloader.loadTexture("res/img/grass.png", &Texture::GRASS);
	glloader.loadTexture("res/img/dirt.png", &Texture::DIRT);
	glloader.loadTexture("res/img/stone.png", &Texture::STONE);
	glloader.loadTexture("res/img/smooth_stone.png", &Texture::SMOOTH_STONE);
	glloader.loadTexture("res/img/sand.png", &Texture::SAND);
	glloader.loadTexture("res/img/tall_grass.png", &Texture::TALL_GRASS);
	glloader.loadTexture("res/img/water.png", &Texture::WATER);
	glloader.loadTexture("res/img/maple_bark.png", &Texture::BARK);
	glloader.loadTexture("res/img/maple_leaf.png", &Texture::LEAVES);

	
	Shader * shader_entity = new Shader("vertex_core.glsl", "fragment_core.glsl");
	Shader * shader_terrain = new Shader("vertex_terrain.glsl", "fragment_terrain.glsl");

	shader_terrain->load1i(Texture::DIRT, "textures.diffuse_dirt");
	shader_terrain->load1i(Texture::GRASS, "textures.diffuse_grass");
	shader_terrain->load1i(Texture::STONE, "textures.diffuse_stone");
	shader_terrain->load1i(Texture::SAND, "textures.diffuse_sand");
	shader_terrain->load1i(Texture::WATER, "textures.diffuse_water");

	
	PointLight pointLight(glm::vec3(0,1000,0));

	RenderManager renderManager(windowManager.getWindow(), &camera, shader_terrain, shader_entity, windowManager.getFBW(), windowManager.getFBH(), &glloader);
	
	World world(&glloader);

	world.loadChunks(camera.getPosition());

	renderManager.loadWorld(&world);

	renderManager.loadLight(&pointLight);

	static double limitFPS = 1.0 / 60.0;
	double lastTime = glfwGetTime();
    double deltaTime = 0, nowTime = 0;
    int frames = 0 , ticks = 0;


	while (!glfwWindowShouldClose(windowManager.getWindow()))
	{

		glfwPollEvents();
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / limitFPS;
        lastTime = nowTime;

        while (deltaTime >= 1.0){// 60 ticks per second (dt in secons)

			glfwPollEvents();
			
			glm::vec3 pos = camera.getPosition() ;
			camera.updateInput(windowManager.getWindow(), .1);
			
			world.updateSkyBox(pos);
			pos.y += 100;
			pointLight.setPosition(pos);
			
			if (glfwGetMouseButton(windowManager.getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			{
				world.reLoadChunks(camera.getPosition());
			}

			
			
            ticks++;
            deltaTime--;
        
		}
		renderManager.render();
		frames++;

	}

    windowManager.closeWindow();

	//delete shader_terrain;
	//delete shader_entity;
}