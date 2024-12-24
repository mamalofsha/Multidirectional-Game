#pragma once
#include <functional>
#include "Graphics.h"

struct MouseState {
    double X = 0.0;
    double Y = 0.0;
    int GridX = 0;
    int GridY = 0; 
    bool LeftPressed = false;
    bool RightPressed = false;
};

class MouseInteractionAPI {
private:
    MouseState CurrentMouseState;
    float TileWidth;
    float Zoom;
    float PanX;
    float PanY;
    float TileHeight;
    float GridOffsetX;
    float GridOffsetY;
    float LevelWidth;
    float LevelHeight;
public:
    using MouseEventCallback = std::function<void(int InGridX, int InGridY, float InScreenX, float InScreenY)>;

    MouseInteractionAPI(GLFWwindow* InWindow, GridConfig InConfig, float InLevelWidth, float InLevelHeight);
    ~MouseInteractionAPI();
    void SetMouseState(MouseState InState);
    MouseState GetMouseState();
    void SetTile(float InTileWidth, float InTileHeight);
    static void CursorCallback(GLFWwindow* InWindow, double InXPos, double InYPos);
    static void ClickCallback(GLFWwindow* InWindow, int InButton, int InAction, int InMods);
    void SetHoverCallback(MouseEventCallback InCallback) { OnHover = InCallback; }
    void SetClickCallback(MouseEventCallback InCallback) { OnClick = InCallback; }
    void HandleMouseMove(double InXPos, double InYPos, int InWindowWidth, int InWindowHeight);
    void HandleMouseClick(double InXPos, double InYPos, int InWindowWidth, int InWindowHeight);
    void SetPanZoom(float InPanX, float InPanY, float InZoom);

public:
    MouseEventCallback OnHover = nullptr;
    MouseEventCallback OnClick = nullptr;

    std::pair<int, int> ScreenToGrid(double InScreenX, double InScreenY, float InTileWidth, float InTileHeight,
        float InOffsetX, float InOffsetY, float InZoom, float InPanX, float InPanY,
        int InWindowWidth, int InWindowHeight, float InLevelWidth, float InLevelHeight);
    std::pair<float, float> ScreenToNDC(float InScreenX, float InScreenY, int InWindowWidth, int InWindowHeight) {
        float NdcX = (InScreenX / InWindowWidth) * 2.0f - 1.0f;
        float NdcY = 1.0f - (InScreenY / InWindowHeight) * 2.0f; 
        return { NdcX, NdcY };
    }
};