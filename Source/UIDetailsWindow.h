#pragma once
#include "UIWindow.h"

class UIButton;

class UIDetailsWindow : public UIWindow {
public:
    std::vector<std::shared_ptr<UIButton>> PageControls; // close/delete item
    int GridX, GridY;
    UIDetailsWindow(std::shared_ptr<Shader> InShaderProgram, float InPosX, float InPosY, float InWidth, 
        float InHeight, const std::string& InAssetPath ,HUD* InHud)
        : UIWindow(InShaderProgram, InPosX, InPosY, InWidth, InHeight){
        InitializeFromRenderData(Graphics::DrawUIElement(std::vector<float>{InPosX, InPosY}, std::vector<float>{InWidth, InHeight}, InAssetPath.c_str()));
        HudPtr = InHud;
    }
    void Draw() override;
    void AddText(UIText InText);
    void Override(std::vector<UIText> InText , const char* InFilePath,int InGridX,int InGridY);
    void ItemDelete();
    void UpdateChildrenButtons(float InX, float InY)override;

 };
