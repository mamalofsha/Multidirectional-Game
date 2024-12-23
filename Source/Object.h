#pragma once
#include "Shader.h"
#include <memory>
#include  "Graphics.h"

class Object {
protected:
    glm::vec2 Position;
    float Size;
    class World* WorldPtr;
    std::shared_ptr<Shader> ObjectShader; // Shared shader program
    bool IsMarkedForDelete = false;
    bool IsHidden = false; // Visibility flag
public:
    virtual void InitializeFromRenderData(const RenderData& data) {};
    Object(std::shared_ptr<Shader> shaderProgram) : ObjectShader(shaderProgram) {}
    virtual ~Object() {}
    virtual void SetHidden(bool newHidden){ IsHidden = newHidden;}
    bool GetHidden() { return IsHidden; };
    bool GetMarkedForDelete() { return IsMarkedForDelete; };
    virtual void Draw() = 0;
    void SetSize(float s) {Size = s;}
    std::shared_ptr<Shader> GetShader() { return ObjectShader; };
};