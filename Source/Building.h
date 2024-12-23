#pragma once
#include "TexturedObject.h"

class Building : public TexturedObject
{
protected:
    int GridX,GridY;

public:
	Building(std::shared_ptr<Shader> shaderProgram,
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices,
		const char* texturePath,
		const VertexAttribute& vertexData,
		World* InWorldPtr, int InGridX, int InGridY)
		: TexturedObject(shaderProgram, vertices, indices, texturePath, vertexData, InWorldPtr), GridX(InGridX), GridY(InGridY){
		// Additional MouseObject-specific initialization here
		SetSize(0.15f);
		/// fix it , it breaks the ui but everyone here counts from 0
	}
	void Draw() override;
};