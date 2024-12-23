#pragma once
#include "Building.h"
#include "HUD.h"

class Decoration : public Building {
private:
    DecorationData Data;
public:
    Decoration(std::shared_ptr<Shader> InShader,
        const std::vector<float>& InVertices,
        const std::vector<unsigned int>& InIndices,
        const VertexAttribute& InVertexData,
        World* InWorld,
        int InGridX,
        int InGridY,
        const DecorationData& InDecorationData)
        : Building(InShader, InVertices, InIndices, InDecorationData.ImageFile.c_str(), InVertexData, InWorld, InGridX, InGridY),
        Data(InDecorationData)
    {
    }
    void Draw() override {
        Building::Draw();
    }
};