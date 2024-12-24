#include "Building.h"
#pragma once

void Building::Draw()
{
	if (IsMarkedForDelete)
	{
		return;
	}
	if (IsHidden)return;
	ObjectShader->use();
	ObjectShader->setBool("IsOverlapping", false);
	ObjectShader->setBool("IsHidden", false);
	float NdcX = 0.0f;
	float NdcY = 0.0f;
	float ScreenX = 0.0f;
	float ScreenY = 0.0f;
	auto [WinX, WinY] = WorldPtr->GetWindowSize();
	MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(WorldPtr->GetWindow()));
	std::tie(ScreenX, ScreenY) = Graphics::GridToWorldPosition(GridX, GridY,
		WorldPtr->GetGridConfig().TileWidth, WorldPtr->GetGridConfig().TileHeight,
		WorldPtr->GetGridConfig().StartOffsetX, WorldPtr->GetGridConfig().StartOffsetY, WorldPtr->GetPan().first, WorldPtr->GetPan().second, Size, WorldPtr->GetZoom(), WinX, WinY, WorldPtr->GetLevelSize().first, WorldPtr->GetLevelSize().second);
	std::tie(NdcX, NdcY) = api->ScreenToNDC(ScreenX, ScreenY, WinX, WinY);
	float ScaleX = WorldPtr->GetLevelSize().first / WinX;
	float ScaleY = WorldPtr->GetLevelSize().second / WinY;
	glm::mat4 Transform = glm::mat4(1.0f);
	Transform = glm::translate(Transform, glm::vec3(NdcX, NdcY, 0.0f));
	Transform = glm::scale(Transform, glm::vec3(ScaleX * WorldPtr->GetZoom() * Size, ScaleY * WorldPtr->GetZoom() * Size, 1.0f));
	ObjectShader->setMat4("Transform", Transform);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::pair<int, int> Building::GetGridCoord()
{
	return {GridX,GridY};
}

void Building::MarkForDelete()
{
	ObjectShader->use();
	ObjectShader->setBool("IsOverlapping", false);
	ObjectShader->setBool("IsHidden", true);
	IsHidden = true;
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	IsMarkedForDelete = true;
}
