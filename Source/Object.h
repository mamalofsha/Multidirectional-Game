#pragma once
#include "Shader.h"
#include <memory>
#include  "Graphics.h"

class Object {
protected:
    glm::vec2 position;
    float size;
    class World* WorldPtr;
    std::shared_ptr<Shader> ObjectShader; // Shared shader program
    bool MarkedForDelete = false;
    bool isHidden = false; // Visibility flag
public:
    virtual void initializeFromRenderData(const RenderData& data) {};
    Object(std::shared_ptr<Shader> shaderProgram) : ObjectShader(shaderProgram) {}
    virtual ~Object() {}
    virtual void SetHidden(bool newHidden){ isHidden = newHidden;}
    bool GetHidden() { return isHidden; };
    bool GetMarkedForDelete() { return MarkedForDelete; };
    virtual void draw() = 0;
    void setSize(float s) {size = s;}
    std::shared_ptr<Shader> GetShader() { return ObjectShader; };
};