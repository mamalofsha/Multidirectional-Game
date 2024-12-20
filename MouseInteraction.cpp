#include "MouseInteraction.h"

MouseInteractionAPI::MouseInteractionAPI(GLFWwindow* InWindow, GridConfig InConfig, MouseEventCallback Hovercallback,MouseEventCallback ClickCallBack)
{
    glfwSetCursorPosCallback(InWindow,CursorCallback);
    glfwSetMouseButtonCallback(InWindow,ClickCallback);
    SetTile(InConfig.tileWidth, InConfig.tileHeight);
    SetHoverCallback(Hovercallback);
    SetClickCallback(ClickCallBack);
    glfwSetWindowUserPointer(InWindow, this);
}

MouseInteractionAPI::~MouseInteractionAPI()
{
}

void MouseInteractionAPI::SetMouseState(MouseState InState)
{
    CurrentMouseState = InState;
}

MouseState MouseInteractionAPI::GetMouseState()
{
    return CurrentMouseState;
}

void MouseInteractionAPI::SetTile(float InTileWidth, float InTileHeight)
{
    TileWidth = InTileWidth;
    TileHeight = InTileHeight;
}

void MouseInteractionAPI::CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(window));
    if (api) {
        // Use the API
        api->HandleMouseMove(xpos, ypos, windowWidth, windowHeight);
    }
}

void MouseInteractionAPI::ClickCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(window));
        if (api) {
            	api->HandleMouseClick(xpos, ypos, windowWidth, windowHeight);
        }
    }
}

void MouseInteractionAPI::HandleMouseMove(double xpos, double ypos, int windowWidth, int windowHeight)
{

    auto [gridX, gridY] = ScreenToGrid(xpos, ypos, TileWidth, TileHeight, windowWidth, windowHeight);
    CurrentMouseState.x = xpos;
    CurrentMouseState.y = ypos;
    CurrentMouseState.GridX = gridX;
    CurrentMouseState.GridY = gridY;
    if (OnHover) {
        OnHover(gridX, gridY); // Trigger hover callback
    }
}

void MouseInteractionAPI::HandleMouseClick(double xpos, double ypos, int windowWidth, int windowHeight)
{
    auto [gridX, gridY] = ScreenToGrid(xpos, ypos, TileWidth, TileHeight, windowWidth, windowHeight);
    if (OnClick) {
        OnClick(gridX, gridY); // Trigger click callback
    }
}

std::pair<int, int> MouseInteractionAPI::ScreenToGrid(double screenX, double screenY, float tileWidth, float tileHeight, int windowWidth, int windowHeight)
{
    /*   float ndcX = (screenX / windowWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (screenY / windowHeight) * 2.0f;
    int gridX = static_cast<int>(round((ndcY / tileHeight) + (ndcX / tileWidth) ));
    int gridY = static_cast<int>(round((ndcY / tileHeight) - (ndcX / tileWidth) ));
    return { gridX, gridY };
   */
   // Step 1: Convert screen coordinates to Normalized Device Coordinates (NDC)
    float ndcX = (screenX / windowWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (screenY / windowHeight) * 2.0f;

    // Debugging NDC values
  //  std::cout << "NDC Coordinates: (" << ndcX << ", " << ndcY << ")" << std::endl;

    // Step 2: Reverse isometric transformation to calculate grid indices
    float approxGridX = (ndcY / tileHeight) + (ndcX / tileWidth);
    float approxGridY = (ndcY / tileHeight) - (ndcX / tileWidth);

    // Debugging approximate grid indices before rounding
   // std::cout << "Approx Grid Coordinates: (" << approxGridX << ", " << approxGridY << ")" << std::endl;

    // Step 3: Round to the nearest integer grid index
    int gridX = static_cast<int>(std::ceil(approxGridX));
    int gridY = static_cast<int>(std::ceil(approxGridY));

    // Debugging final grid indices
    //std::cout << "Final Grid Coordinates: (" << gridX << ", " << gridY << ")" << std::endl;

    return { gridX, gridY };
}
