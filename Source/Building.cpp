#include "Building.h"
#pragma once

void Building::Draw()
{
	if (IsHidden)return;
	ObjectShader->use();
	ObjectShader->setBool("isOverlapping", false);
	float NdcX = 0.0f;
	float NdcY = 0.0f;
	float ScreenX = 0.0f;
	float ScreenY = 0.0f;
	auto [WinX, WinY] = WorldPtr->GetWindowSize();
	MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));

	std::tie(ScreenX, ScreenY) = Graphics::GridToWorldPosition(GridX, GridY,
		WorldPtr->GetGridConfig().TileWidth, WorldPtr->GetGridConfig().TileHeight,
		WorldPtr->GetGridConfig().StartOffsetX, WorldPtr->GetGridConfig().StartOffsetY, WorldPtr->GetPan().first, WorldPtr->GetPan().second, Size, WorldPtr->GetZoom(), WinX, WinY, WorldPtr->GetLevelSize().first, WorldPtr->GetLevelSize().second);
	std::tie(NdcX, NdcY) = api->screenToNDC(ScreenX, ScreenY, WinX, WinY);
	float ScaleX = WorldPtr->GetLevelSize().first / WinX;
	float ScaleY = WorldPtr->GetLevelSize().second / WinY;
	glm::mat4 transform = glm::mat4(1.0f);
	GLuint  transformLoc;
	transform = glm::translate(transform, glm::vec3(NdcX, NdcY, 0.0f));
	transform = glm::scale(transform, glm::vec3(ScaleX * WorldPtr->GetZoom() * Size, ScaleY * WorldPtr->GetZoom() * Size, 1.0f));
	ObjectShader->setMat4("transform", transform);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
