#pragma once
#include "TexturedObject.h"

// the thing that follows the mouse as an item
class MouseObject : public TexturedObject
{
private:
    bool IsAttachedToGrid;
    bool IsOverlapping = false;
    // For passing it to the actual building that goes up 
    std::string ItemID;
public:
    MouseObject(std::shared_ptr<Shader> InShaderProgram,
        const std::vector<float>& InVertices,
        const std::vector<unsigned int>& InIndices,
        const char* InTexturePath,
        const VertexAttribute& InVertexData,
        World* InWorldPtr)
        : TexturedObject(InShaderProgram, InVertices, InIndices, InTexturePath, InVertexData, InWorldPtr) {
        IsHidden = true;
    }
    void Draw() override;
    void SetItemID(const std::string& InItemID) { ItemID = InItemID; }
    void ReloadTexture(const char* InFilePath);
    bool GetIsAttachedToGrid() { return IsAttachedToGrid; }
    void ExecuteAction();
};