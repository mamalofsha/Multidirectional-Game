#pragma once
#include "Shader.h"
#include <memory>
#include  "Graphics.h"

class Object {
protected:

public:
    std::shared_ptr<Shader> ObjectShader; // Shared shader program
    bool MarkedForDelete = false;
    bool isHidden = false; // Visibility flag
    virtual void initializeFromRenderData(const RenderData& data) {};
    Object(std::shared_ptr<Shader> shaderProgram) : ObjectShader(shaderProgram) {}
    virtual ~Object() {}
    virtual void SetHidden(bool newHidden)
    {
        isHidden = newHidden;
    }
    virtual void draw() = 0;
};