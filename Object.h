#pragma once
#include "Shader.h"
#include <memory>
#include  "Graphics.h"

class Object {
protected:

public:
    std::shared_ptr<Shader> ObjectShader; // Shared shader program
    bool MarkedForDelete = false;
    virtual void initializeFromRenderData(const RenderData& data) {};
    Object(std::shared_ptr<Shader> shaderProgram) : ObjectShader(shaderProgram) {}
    virtual ~Object() {}

    virtual void draw() = 0;
};