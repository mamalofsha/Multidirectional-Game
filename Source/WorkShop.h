#pragma once
#include "Building.h"

class Workshop : public Building {
private:
    WorkshopData Data;
public:
    Workshop(std::weak_ptr<Shader> InShader,
        const std::vector<float>& InVertices,
        const std::vector<unsigned int>& InIndices,
        const VertexAttribute& InVertexData,
        World* InWorld,
        int InGridX,
        int InGridY,
        const WorkshopData& InData)
        : Building(InShader, InVertices, InIndices, InData.ImageFile.c_str(), InVertexData, InWorld, InGridX, InGridY)
    {
    }
    void Draw() override {
        Building::Draw();
    }
};