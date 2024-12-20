#include "World.h"

int main() {
	// init world
	World* CurrentWorld = new World(800.f, 800.f);
	// render loop
	float DeltaTime = 0.0f;
	float LastFrame = 0.0f;
	while (CurrentWorld->IsRunning())
	{
		float CurrentFrame = glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = glfwGetTime();
		CurrentWorld->Update(DeltaTime);
	}
	delete CurrentWorld;
	return 0;
}