#include "World.h"
#include "Graphics.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>


void World::InitBackground()
{
	Shaders.push_back(Graphics::DrawTexture("grass2.jpg"));
}

void World::SpawnPlayer()
{
	Player = std::make_shared<GameObject>();
	GameObjects.push_back(Player);
	Graphics::DrawShape(*Player);
}

void World::SpawnObjects()
{
}

void World::ProcessInputGL(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	std::vector<float> Input = { 0.f,0.f };
	/// player input for movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Input[1] = 1.f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Input[0] = -1.f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Input[0] = 1.f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Input[1] = -1.f;
	PassInput(Input);
}

void World::InputUpdate()
{
	glfwPollEvents();
	ProcessInputGL(Window);
}

void World::RenderUpdate()
{
	//
	glClearColor(0.2f, 0.3f, 0.76f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// static stuff , right now only BG
	for (auto it = Shaders.begin(); it < Shaders.end(); it++)
	{
		// bind Texture
		glBindTexture(GL_TEXTURE_2D, it->Texture);
		// render container
		it->use();
		glBindVertexArray(it->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	// position update
	for (auto it = GameObjects.begin(); it != GameObjects.end(); ++it)
	{
		// create transformations
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3((*it)->GetTransform().Location[0], (*it)->GetTransform().Location[1], 0.0f));
		transform = glm::rotate(transform, (*it)->GetTransform().Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		(*it)->ObjectShader->use();
		unsigned int transformLoc = glGetUniformLocation((*it)->ObjectShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		glBindVertexArray((*it)->ObjectShader->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	glfwSwapBuffers(Window);
}

std::weak_ptr<GameObject> World::GetPlayer()
{
	return Player;
}

World::World(std::vector<std::shared_ptr<GameObject>>& GameObjects)
{

}

World::World(const unsigned int Width, const unsigned int Height)
{
	Window = Graphics::InitWindow(Width, Height);
	InitBackground();
	SpawnPlayer();
}

World::~World()
{
	for (auto it = Shaders.begin(); it < Shaders.end(); it++)
	{
		glDeleteVertexArrays(1, &it->VAO);
		glDeleteBuffers(1, &it->VBO);
		glDeleteBuffers(1, &it->EBO);
	}
	GameObjects.clear();
	Player = nullptr;
	Shaders.clear();
	glfwTerminate();
}


void World::Update(float DeltaSeconds)
{
	InputUpdate();
	RenderUpdate();
}

void World::PassInput(std::vector<float> InDir)
{
	Player->ConsumeInput(InDir);
}

bool World::IsRunning()
{
	return !glfwWindowShouldClose(Window);
}
