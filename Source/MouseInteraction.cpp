#include "MouseInteraction.h"
#include "World.h"

MouseInteractionAPI::MouseInteractionAPI(GLFWwindow* InWindow, GridConfig InConfig, float InLevelWidth, float InLevelHeight) {
    glfwSetCursorPosCallback(InWindow, CursorCallback);
    glfwSetMouseButtonCallback(InWindow, ClickCallback);
    SetTile(InConfig.TileWidth, InConfig.TileHeight);
    GridOffsetX = InConfig.StartOffsetX;
    GridOffsetY = InConfig.StartOffsetY;
    LevelWidth = InLevelWidth;
    LevelHeight = InLevelHeight;
    glfwSetWindowUserPointer(InWindow, this);
}

MouseInteractionAPI::~MouseInteractionAPI() {
}

void MouseInteractionAPI::SetMouseState(MouseState InState) {
    CurrentMouseState = InState;
}

MouseState MouseInteractionAPI::GetMouseState() {
    return CurrentMouseState;
}

void MouseInteractionAPI::SetTile(float InTileWidth, float InTileHeight) {
    TileWidth = InTileWidth;
    TileHeight = InTileHeight;
}

void MouseInteractionAPI::CursorCallback(GLFWwindow* InWindow, double InXPos, double InYPos) {
    int WindowWidth, WindowHeight;
    glfwGetWindowSize(InWindow, &WindowWidth, &WindowHeight);
    MouseInteractionAPI* Api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(InWindow));
    if (Api) {
        Api->HandleMouseMove(InXPos, InYPos, WindowWidth, WindowHeight);
    }
}

void MouseInteractionAPI::ClickCallback(GLFWwindow* InWindow, int InButton, int InAction, int InMods) {
    if (InButton == GLFW_MOUSE_BUTTON_LEFT && InAction == GLFW_PRESS) {
        double XPos, YPos;
        glfwGetCursorPos(InWindow, &XPos, &YPos);
        int WindowWidth, WindowHeight;
        glfwGetWindowSize(InWindow, &WindowWidth, &WindowHeight);

        MouseInteractionAPI* Api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(InWindow));
        if (Api) {
            Api->HandleMouseClick(XPos, YPos, WindowWidth, WindowHeight);
        }
    }
}

void MouseInteractionAPI::HandleMouseMove(double InXPos, double InYPos, int InWindowWidth, int InWindowHeight) {
    auto [GridX, GridY] = ScreenToGrid(InXPos, InYPos, TileWidth, TileHeight, GridOffsetX, GridOffsetY, Zoom, PanX, PanY, InWindowWidth, InWindowHeight, LevelWidth, LevelHeight);
    auto [X, Y] = ScreenToNDC(InXPos, InYPos, InWindowWidth, InWindowHeight);
    CurrentMouseState.X = InXPos;
    CurrentMouseState.Y = InYPos;
    CurrentMouseState.GridX = GridX;
    CurrentMouseState.GridY = GridY;
    if (OnHover) {
        OnHover(GridX, GridY, X, Y);
    }
}

void MouseInteractionAPI::HandleMouseClick(double InXPos, double InYPos, int InWindowWidth, int InWindowHeight) {
    auto [GridX, GridY] = ScreenToGrid(InXPos, InYPos, TileWidth, TileHeight, GridOffsetX, GridOffsetY, PanX, PanY, Zoom, InWindowWidth, InWindowHeight, LevelWidth, LevelHeight);
    if (OnClick) {
        OnClick(GridX, GridY, InXPos, InYPos);
    }
}

void MouseInteractionAPI::SetPanZoom(float InPanX, float InPanY, float InZoom) {
    Zoom = InZoom;
    PanX = InPanX;
    PanY = InPanY;
}

std::pair<int, int> MouseInteractionAPI::ScreenToGrid(double InScreenX, double InScreenY, float InTileWidth, float InTileHeight, float InOffsetX, float InOffsetY, float InZoom, float InPanX, float InPanY, int InWindowWidth, int InWindowHeight, float InLevelWidth, float InLevelHeight) {
    float WindowAspectRatio = static_cast<float>(InWindowWidth) / static_cast<float>(InWindowHeight);
    float BgAspectRatio = InLevelWidth / InLevelHeight;

    float ScaleX = InLevelWidth / static_cast<float>(InWindowWidth);
    float ScaleY = InLevelHeight / static_cast<float>(InWindowHeight);

    float NdcX = ((((InScreenX) / InWindowWidth) * 2.0f - 1.0f) / (InZoom * ScaleX));
    float NdcY = ((1.0f - ((InScreenY) / InWindowHeight) * 2.0f) / (InZoom * ScaleY));
    NdcX -= InPanX;
    NdcY -= InPanY;

    float ApproxGridX = (NdcY / InTileHeight) + (NdcX / InTileWidth) - (InOffsetX);
    float ApproxGridY = (NdcY / InTileHeight) - (NdcX / InTileWidth) - (InOffsetY);

    int GridX = static_cast<int>(std::floor(ApproxGridX));
    int GridY = static_cast<int>(std::floor(ApproxGridY));

    return { GridX, GridY };
}

