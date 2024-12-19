#include "World.h"
#include "Graphics.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "PlayerObject.h"
#include "Math.h"


void World::InitBackground()
{
	Shaders.push_back({ Graphics::DrawTexture("Map1.jpg"), ShaderType::Background });
	Shaders.push_back({ Graphics::DrawGrid("grid_config.xml"), ShaderType::Grid });
}

void World::SpawnPlayer()
{
	Player = std::make_shared<PlayerObject>();
	GameObjects.push_back(Player);
	Graphics::DrawShape(*Player);
}

void World::SpawnObjects()
{
}

void World::onHoverFunction(int gridX, int gridY)
{
	std::cout << "Hovered over tile: (" << gridX << ", " << gridY << ")" << std::endl;
	mouseState.GridX = gridX;
	mouseState.GridY = gridY;
}

void World::onClickFunction(int gridX, int gridY)
{
	std::cout << "Clicked on tile: (" << gridX << ", " << gridY << ")" << std::endl;
	// Additional logic for click behavior
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

void World::GarbageCollection()
{
	for (auto it = GameObjects.begin(); it != GameObjects.end(); )
	{
		if ((*it)->MarkedForDelete)
		{
			it = GameObjects.erase(it);
		}
		else
			++it;
	}
}

void World::InputUpdate()
{
	if (Player)
	{
		ProcessInputGL(Window);
	}
}

void World::SetupMouseCallbacks()
{
	// Mouse Interaction API setup
	MouseInteractionAPI mouseAPI;
	//mouseAPI.setHoverCallback(onHoverFunction); // Use regular function
	//mouseAPI.setClickCallback(onClickFunction); // Use regular function
	glfwSetCursorPosCallback(Window, [](GLFWwindow* window, double xpos, double ypos) {
		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		//mouseState.x = xpos;
		//mouseState.y = ypos;
		MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(window));
		if (api) {
			//api->handleMouseMove(xpos, ypos, windowWidth, windowHeight, gridConfig.tileWidth, gridConfig.tileHeight);
		}
		});
	glfwSetMouseButtonCallback(Window, [](GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			int windowWidth, windowHeight;
			glfwGetWindowSize(window, &windowWidth, &windowHeight);

			MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(window));
			if (api) {
				//	api->handleMouseClick(xpos, ypos, windowWidth, windowHeight, gridConfig.tileWidth, gridConfig.tileHeight);
			}
		}
		});

	glfwSetWindowUserPointer(Window, &mouseAPI);

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
		// render container
		switch (it->type)
		{
		case ShaderType::Background:

			// bind Texture
			glBindTexture(GL_TEXTURE_2D, it->shader.Texture);
			// render container
			it->shader.use();
			glBindVertexArray(it->shader.VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			break;
		case ShaderType::Grid:
			it->shader.use();
			it->shader.setUniform2i("tileCoor", mouseState.GridX, mouseState.GridY);
			glBindVertexArray(it->shader.VAO);
			glDrawArrays(GL_POINTS, 0, 200 / 2);
			break;
		}
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

void World::CollisionUpdate()
{

	for (auto it = GameObjects.begin(); it != GameObjects.end(); ++it)
	{
		if (!Player || Player->MarkedForDelete)
		{
			break;
		}
		if (*it == Player)continue;
		std::vector<float> Raw;
		for (size_t i = 0; i < Player->GetTransform().Location.size(); i++)
		{
			Raw.push_back((*it)->GetTransform().Location[i] - Player->GetTransform().Location[i]);
		}
		if (Math::VectorSize(Raw) < Player->GetLength() * 1.5f)
		{
			bool collisionX = Player->GetTransform().Location[0] + Player->GetLength() >= (*it)->GetTransform().Location[0] &&
				(*it)->GetTransform().Location[0] + (*it)->GetLength() >= Player->GetTransform().Location[0];
			// collision y-axis?
			bool collisionY = Player->GetTransform().Location[1] + Player->GetLength() >= (*it)->GetTransform().Location[1] &&
				(*it)->GetTransform().Location[1] + (*it)->GetLength() >= Player->GetTransform().Location[1];
			// collision only if on both axes
			if (collisionX && collisionY)
			{
				if ((*it)->GetIsHazard())
				{
					std::cout << "Collision with hazard!" << std::endl;
					Player->MarkedForDelete = true;
					Player = nullptr;
				}
				else
				{
					std::cout << "Collision with item" << std::endl;
					(*it)->MarkedForDelete = true;
				}
			}
		}
	}
}

void World::HandleCollision(GameObject& GameObject1, GameObject& GameObject2)
{
	if (GameObject2.GetIsHazard())
	{
		Player = nullptr;
	}
	else
	{
		delete& GameObject2;
	}
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
	SetupMouseCallbacks();
	InitBackground();
	//SpawnPlayer();
}

World::~World()
{
	for (auto it = Shaders.begin(); it < Shaders.end(); it++)
	{
		glDeleteVertexArrays(1, &it->shader.VAO);
		glDeleteBuffers(1, &it->shader.VBO);
		glDeleteBuffers(1, &it->shader.EBO);
	}
	GameObjects.clear();
	Player = nullptr;
	Shaders.clear();
	glfwTerminate();
}


void World::Update(float DeltaSeconds)
{
	GarbageCollection();
//	InputUpdate();
	RenderUpdate();
	glfwPollEvents();

	//CollisionUpdate();
}

void World::PassInput(std::vector<float> InDir)
{
	Player->ConsumeInput(InDir);
}

bool World::IsRunning()
{
	return !glfwWindowShouldClose(Window);
}


