#pragma once
#include "Object.h"
#include "World.h"
#include "Graphics.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

class GridObject : public Object
{
private:
	unsigned int VAO, VBO;
	size_t GridVerticesSize;
public:
	GridObject(std::weak_ptr<Shader> InShaderProgram, GridConfig InGridConfig, World* InWorldPtr)
		: Object(InShaderProgram)
	{
		WorldPtr = InWorldPtr;
		InitializeFromRenderData(Graphics::DrawGrid(InShaderProgram,InGridConfig, WorldPtr->GetWindowSize().first, WorldPtr->GetWindowSize().second));
	}

	~GridObject() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void Draw() override;
	void InitializeFromRenderData(const RenderData& InData) override;
};