#pragma once
#include "Source/Building.h"
#include "HUD.h"

class Decoration : public Building {
private:
    DecorationData Data;

public:
    Decoration(std::shared_ptr<Shader> shader,
        const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices,
        const VertexAttribute& vertexData,
        World* world,
        int gridX,
        int gridY,
        const DecorationData& data)
        : Building(shader, vertices, indices, data.ImageFile.c_str(), vertexData, world, gridX, gridY) // Pass to base class constructor
    {
    }

    void draw() override {
        // Draw the workshop...
        Building::draw();
    }
};