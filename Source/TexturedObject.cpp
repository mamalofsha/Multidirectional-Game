#include "TexturedObject.h"

void TexturedObject::Draw()
{
	auto [winX, winY] = WorldPtr->GetWindowSize();
	float scaleX = WorldPtr->GetLevelSize().first / winX;
	float scaleY = WorldPtr->GetLevelSize().second / winY;
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, glm::vec3(scaleX * WorldPtr->GetZoom(), scaleY * WorldPtr->GetZoom(), 1.0f));
	transform = glm::translate(transform, glm::vec3(WorldPtr->GetPan().first, WorldPtr->GetPan().second, 0.0f));
	auto SharedObjectShader = ObjectShader.lock();
	if (!SharedObjectShader) return;
	SharedObjectShader->setMat4("Transform", transform);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TexturedObject::InitializeFromRenderData(const RenderData& InData)
{
	VAO = InData.VAO;
	VBO = InData.VBO;
	EBO = InData.EBO;
	Texture = InData.TextureID;
}
