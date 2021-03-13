#pragma once
#include "Header.h"
class RenderManager
{
private:
	GLFWwindow *window;
	Shader *shader_terrain, *shader_entity;

	Camera *camera;

	World *rendered_world;
	PointLight *pl;
	void init()
	{
		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CCW);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

public:
	RenderManager(GLFWwindow *window, Camera *camera, Shader *shader_terrain, Shader *shader_entity, int frameBufferWidth, int frameBufferHeight, GLLoader *loader)
	{
		shader_terrain->loadMat4fv(camera->getProjectionMatrix(frameBufferWidth, frameBufferHeight), "ProjectionMatrix");
		shader_entity->loadMat4fv(camera->getProjectionMatrix(frameBufferWidth, frameBufferHeight), "ProjectionMatrix");
		this->shader_terrain = shader_terrain;
		this->shader_entity = shader_entity;
		this->window = window;
		this->camera = camera;
		init();
	}

	~RenderManager()
	{
	}
	void updateProjection(Camera *camera, int frameBufferWidth, int frameBufferHeight)
	{
		shader_terrain->loadMat4fv(camera->getProjectionMatrix(frameBufferWidth, frameBufferHeight), "ProjectionMatrix");
		shader_entity->loadMat4fv(camera->getProjectionMatrix(frameBufferWidth, frameBufferHeight), "ProjectionMatrix");
	}
	void render()
	{

		glClearColor(0,0, 1, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		///
		/// LOADING TERRAIN
		///

		shader_terrain->loadMat4fv(camera->getViewMatrix(), "ViewMatrix");
		shader_terrain->loadVec3f(camera->getPosition(), "cameraPos");

		//shader_entity->loadLight();
		shader_terrain->loadVec3f(pl->position, "pointLight.position");
		shader_terrain->load1f(pl->intensity, "pointLight.intensity");
		shader_terrain->loadVec3f(pl->color, "pointLight.color");
		shader_terrain->load1f(pl->constant, "pointLight.constant");
		shader_terrain->load1f(pl->linear, "pointLight.linear");
		shader_terrain->load1f(pl->quadratic, "pointLight.quadratic");

		glActiveTexture(GL_TEXTURE0 + Texture::GRASS);
		glBindTexture(GL_TEXTURE_2D, Texture::GRASS);
		glActiveTexture(GL_TEXTURE0 + Texture::DIRT);
		glBindTexture(GL_TEXTURE_2D, Texture::DIRT);
		glActiveTexture(GL_TEXTURE0 + Texture::STONE);
		glBindTexture(GL_TEXTURE_2D, Texture::STONE);
		glActiveTexture(GL_TEXTURE0 + Texture::SAND);
		glBindTexture(GL_TEXTURE_2D, Texture::SAND);

		for (int i = 0; i < (2 * World::RENDER_DISTANCE + 1) * (2 * World::RENDER_DISTANCE + 1); i++)
		{
			if (rendered_world->getChunks() == NULL || rendered_world->getChunks()[i] == NULL)
			{
				continue;
			}

			Chunk *chunk = rendered_world->getChunks()[i];

			shader_terrain->loadMat4fv(chunk->matrix_transformation, "ModelMatrix");
			shader_terrain->loadMaterial(chunk->material);
			shader_terrain->use();

			glBindVertexArray(chunk->mesh->VAO);

			glDrawElements(GL_TRIANGLES, chunk->mesh->getIndiceCount(), GL_UNSIGNED_INT, 0);
		}
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		shader_terrain->unuse();

		//
		//LOADING ENTITTIES
		//

		shader_entity->loadMat4fv(camera->getViewMatrix(), "ViewMatrix");
		shader_entity->loadVec3f(camera->getPosition(), "cameraPos");

		shader_entity->loadVec3f(pl->position, "pointLight.position");
		shader_entity->load1f(pl->intensity, "pointLight.intensity");
		shader_entity->loadVec3f(pl->color, "pointLight.color");
		shader_entity->load1f(pl->constant, "pointLight.constant");
		shader_entity->load1f(pl->linear, "pointLight.linear");
		shader_entity->load1f(pl->quadratic, "pointLight.quadratic");

		for (Entity *e : *rendered_world->getEntities())
		{
				//glActiveTexture(GL_TEXTURE0 + (GLint)1);
				//glBindTexture(GL_TEXTURE_2D, model->textureSpecularID);

				glActiveTexture(GL_TEXTURE0 + e->model->material->texture_diffuse);
				glBindTexture(GL_TEXTURE_2D, e->model->material->texture_diffuse);

				shader_entity->load1i(e->model->material->texture_diffuse, "material.diffuseTex");
				if(e->model->material->texture_diffuse!= 0 )
					shader_entity->load1i(1, "useTexture");
				else
					shader_entity->load1i(0, "useTexture");

				if(e->fakeLighting )
					shader_entity->load1i(1, "fakeLighting");
				else
					shader_entity->load1i(0, "fakeLighting");

				shader_entity->loadMat4fv(e->matrix_transformation, "ModelMatrix");
				shader_entity->loadTexturedMaterial(e->model->material);
				shader_entity->use();
				glBindVertexArray(e->model->mesh->getVAO());
				glDrawElements(GL_TRIANGLES, e->model->mesh->getIndiceCount(), GL_UNSIGNED_INT, 0);
		}
		

		glBindVertexArray(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		shader_entity->unuse();

		glfwSwapBuffers(window);
		glFlush();
	}

	void loadWorld(World *world)
	{
		this->rendered_world = world;
	}

	void loadLight(PointLight *l)
	{
		pl = l;
	}
};