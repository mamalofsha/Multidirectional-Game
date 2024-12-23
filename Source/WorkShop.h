#pragma once
#include "Building.h"
#include "HUD.h"

class Workshop : public Building {
private:
    WorkshopData Data;

public:
    Workshop(std::shared_ptr<Shader> shader,
        const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices,
        const VertexAttribute& vertexData,
        World* world,
        int gridX,
        int gridY,
        const WorkshopData& data)
        : Building(shader, vertices, indices, data.ImageFile.c_str(), vertexData, world, gridX, gridY) // Pass to base class constructor
    {
    }

    void draw() override {
        // Draw the workshop...
        Building::draw();
    }
};