#pragma once
#include <functional>
#include "Graphics.h"


struct MouseState {
    double x, y = 0.0;
    int GridX, GridY = 0;// Current mouse position
    bool leftPressed=false;  // Is the left button pressed?
    bool rightPressed=false; // Is the right button pressed?
};

class MouseInteractionAPI {
private:
    MouseState CurrentMouseState;
    float TileWidth;
    float Zoom;
    float PanX;
    float PanY;
    float TileHeight;
    float gridoffsetX;
    float gridoffsetY;
    float levelWidth;
    float levelHeight;
public:
    using MouseEventCallback = std::function<void(int gridX, int gridY,float screenX,float screenY)>;
    
    MouseInteractionAPI(GLFWwindow* InWindow, GridConfig InConfig, float inLevelwidth, float inLevelheight);
    ~MouseInteractionAPI();
    void SetMouseState(MouseState InState);
    MouseState GetMouseState();
    void SetTile(float InTileWidth, float InTileHeight);
    static void CursorCallback( GLFWwindow* window, double xpos, double ypos);
    static void ClickCallback(GLFWwindow* window, int button, int action, int mods);
    void SetHoverCallback(MouseEventCallback callback) { OnHover = callback; }
    void SetClickCallback(MouseEventCallback callback) { OnClick = callback; }
    void HandleMouseMove(double xpos, double ypos, int windowWidth, int windowHeight);
    void HandleMouseClick(double xpos, double ypos, int windowWidth, int windowHeight);
    void SetPanZoom(float Panx, float pany,float zoom);

public:
    MouseEventCallback OnHover = nullptr;
    MouseEventCallback OnClick = nullptr;

    // Convert screen coordinates to grid coordinates
    std::pair<int, int> ScreenToGrid(double screenX, double screenY, float tileWidth, float tileHeight, float offsetX, float offsetY,float zoom , float panX, float panY ,int windowWidth, int windowHeight,float levelWidth,float levelHeight);

    std::pair<float, float> screenToNDC(float screenX, float screenY, int windowWidth, int windowHeight) {
        float ndcX = (screenX / windowWidth) * 2.0f - 1.0f;
        float ndcY = 1.0f - (screenY / windowHeight) * 2.0f; // Inversion for Y
        return { ndcX, ndcY };
    }
};