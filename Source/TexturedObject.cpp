#include "TexturedObject.h"

void TexturedObject::Draw()
{
	auto [WinX, WinY] = WorldPtr->GetWindowSize();
	float ScaleX = WorldPtr->GetLevelSize().first / WinX;
	float ScaleY = WorldPtr->GetLevelSize().second / WinY;
	glm::mat4 Transform = glm::mat4(1.0f);
	Transform = glm::scale(Transform, glm::vec3(ScaleX * WorldPtr->GetZoom(), ScaleY * WorldPtr->GetZoom(), 1.0f));
	Transform = glm::translate(Transform, glm::vec3(WorldPtr->GetPan().first, WorldPtr->GetPan().second, 0.0f));
	auto SharedObjectShader = ObjectShader.lock();
	if (!SharedObjectShader) return;
	SharedObjectShader->setMat4("Transform", Transform);
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
