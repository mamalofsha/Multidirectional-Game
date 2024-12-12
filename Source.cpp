#include "World.h"
#include "Graphics.h"

// global variables
World* CurrentWorld;
// function definitions
void processInput(GLFWwindow* window);

int main()
{
	//////////////
	// init window
	GLFWwindow* Window = Graphics::InitWindow();
	// init world
	CurrentWorld = new World();
	/////////////
	// ------------------------------------
	// render loop
	// -----------
	while (!glfwWindowShouldClose(Window))
	{
		// input
		processInput(Window);
		// World update ( draw , objects tick , etc) 
		CurrentWorld->Update(0.f);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	delete CurrentWorld;
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
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
	CurrentWorld->ProcessInput(Input);
}