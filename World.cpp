#include "World.h"
#include "Graphics.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "PlayerObject.h"
#include "Math.h"
#include "XMLParser.h"
#include <algorithm>


World::World(std::vector<std::shared_ptr<GameObject>>& GameObjects)
{

}

World::World(const unsigned int Width, const unsigned int Height)
{
	StartUpData Temp = XMLParser::LoadLeveL("TestLevel.xml");
	Window = Graphics::InitWindow(Temp.WindowWidth, Temp.WindowHeight);
	InitBackground();
	InitGrid(Temp.GridFileName);
	SetupMouseCallbacks();

}

World::~World()
{
	for (auto it = Shaders.begin(); it < Shaders.end(); it++)
	{
		glDeleteVertexArrays(1, &it->shader.VAO);
		glDeleteBuffers(1, &it->shader.VBO);
		glDeleteBuffers(1, &it->shader.EBO);
	}
	MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(Window));
	if (api) {
		delete api; // Free the dynamically allocated memory
	}
	GameObjects.clear();
	Player = nullptr;
	Shaders.clear();
	glfwTerminate();
}

void World::InitBackground()
{
	Shaders.push_back({Graphics::DrawTexture("Map1.jpg"), ShaderType::Background });
}

void World::InitGrid(const std::string& InFileName)
{
	gridConfig = XMLParser::ParseGridDataFromXML(InFileName);
	Shaders.push_back({ Graphics::DrawGrid(gridConfig), ShaderType::Grid });
}

void World::onHoverFunction(int gridX, int gridY)
{
	std::cout << "Hovereddead over tile: (" << gridX << ", " << gridY << ")" << std::endl;
	//mouseState.GridX = gridX;
	//mouseState.GridY = gridY;
}

void World::onClickFunction(int gridX, int gridY)
{
	std::cout << "Clicked on tile: (" << gridX << ", " << gridY << ")" << std::endl;
	// Additional logic for click behavior
}

void World::CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(window));
	if (api) {
		// Use the API
		api->HandleMouseMove(xpos, ypos, windowWidth, windowHeight);
	}
}

void World::ProcessInputGL(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	float panSpeed = 0.01f; // Speed of panning


	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		panX -= panSpeed;

	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		panX += panSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		panY += panSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		panY -= panSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if(Zoom < 3.5f)
		Zoom +=  0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Zoom -= 0.05f;

	}

	Zoom = std::clamp(Zoom, 1.f, 3.5f);
	float minPanX = -((2000.0f * Zoom) - windowWidth) / 2000.0f;
	float maxPanX = ((2000.0f * Zoom) - windowWidth) / 2000.0f;

	float minPanY = -((1404.0f * 1) - (windowHeight * Zoom)) / 2000.0f;
	float maxPanY = ((1404.0f * Zoom) - windowHeight) / 2000.0f;
	panX = std::clamp(panX, minPanX, maxPanX);
	panY = std::clamp(panY, minPanY, maxPanY);
	std::cout << "pany: " << panY << std::endl;
	std::cout << "panY lim: " << minPanY << ": " << maxPanY << std::endl;



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
	ProcessInputGL(Window);
}

void World::SetupMouseCallbacks()
{
	// Mouse Interaction API setup
	MouseInteractionAPI* mouseAPI = new MouseInteractionAPI(Window,gridConfig,onHoverFunction,onClickFunction);
	// Set the user pointer
}

void World::RenderUpdate()
{
	//
	glClearColor(0.2f, 0.3f, 0.76f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// static stuff , right now only BG
	for (auto it = Shaders.begin(); it < Shaders.end(); it++)
	{
		float scaleX = 2000.0f / 800.0f;
		float scaleY = 1404.0f / 800.0f;
		// bind Texture
		// render container
		GLuint  transformLoc;
		switch (it->type)
		{
		case ShaderType::Background:

			// bind Texture
			glBindTexture(GL_TEXTURE_2D, it->shader.Texture);
			// render container
			it->shader.use();
			//it->shader.setUniform3f("panOffset", panX, panY,0.f);
			// Scale and translate the background
			glm::mat4 transform = glm::mat4(1.0f);

			transform = glm::scale(transform, glm::vec3(scaleX*Zoom, scaleY*Zoom, 1.0f));
			transform = glm::translate(transform, glm::vec3(panX, panY, 0.0f));

			  transformLoc = glGetUniformLocation((it)->shader.ID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
			//it->shader.setFloat("zoom", Zoom);
			glBindVertexArray(it->shader.VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			break;
		case ShaderType::Grid:
			it->shader.use();
			it->shader.setUniform2i("tileCoor", mouseState.GridX, mouseState.GridY);
			it->shader.setUniform2f("panOffset", panX, panY);
			it->shader.setFloat("zoom", Zoom);

			glBindVertexArray(it->shader.VAO);
			//// todoooo donmt forgetteettete it shoudl be grid vertices size
			glDrawArrays(GL_LINES, 0, 200 / 2);
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
		GLuint   transformLoc = glGetUniformLocation((*it)->ObjectShader->ID, "transform");
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





void World::Update(float DeltaSeconds)
{
	GarbageCollection();
	InputUpdate();
	RenderUpdate();
	glfwPollEvents();
	//CollisionUpdate();

}

bool World::IsRunning()
{
	return !glfwWindowShouldClose(Window);
}


