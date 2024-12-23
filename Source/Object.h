#pragma once
#include <memory>
#include "Graphics.h"

class Object {
protected:
    glm::vec2 Position;
    float Size;
    class World* WorldPtr;
    std::shared_ptr<Shader> ObjectShader; // Shared shader program
    bool IsMarkedForDelete = false;
    bool IsHidden = false; // Visibility flag
public:
    virtual void InitializeFromRenderData(const RenderData& InData) {};
    Object(std::shared_ptr<Shader> InShaderProgram) : ObjectShader(InShaderProgram) {}
    virtual ~Object() {}
    virtual void SetHidden(bool NewHidden){ IsHidden = NewHidden;}
    bool GetHidden() { return IsHidden; };
    bool GetMarkedForDelete() { return IsMarkedForDelete; };
    virtual void Draw() = 0;
    void SetSize(float s) {Size = s;}
    std::shared_ptr<Shader> GetShader() { return ObjectShader; };
};