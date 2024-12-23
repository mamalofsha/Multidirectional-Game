#pragma once
#include "TexturedObject.h"

class Building : public TexturedObject
{
protected:
    int GridX,GridY;
public:
    Building(std::shared_ptr<Shader> InShaderProgram,
        const std::vector<float>& InVertices,
        const std::vector<unsigned int>& InIndices,
        const char* InTexturePath,
        const VertexAttribute& InVertexData,
        World* InWorldPtr, int InGridX, int InGridY)
        : TexturedObject(InShaderProgram, InVertices, InIndices, InTexturePath, InVertexData, InWorldPtr),
        GridX(InGridX), GridY(InGridY) {
        SetSize(0.15f);
    }
	void Draw() override;
};