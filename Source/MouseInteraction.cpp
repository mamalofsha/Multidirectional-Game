#include "MouseInteraction.h"
#include "World.h"

MouseInteractionAPI::MouseInteractionAPI(GLFWwindow* InWindow, GridConfig InConfig,float inLevelwidth,float inLevelheight)
{
    glfwSetCursorPosCallback(InWindow,CursorCallback);
    glfwSetMouseButtonCallback(InWindow,ClickCallback);
    SetTile(InConfig.TileWidth, InConfig.TileHeight);
    gridoffsetX = InConfig.StartOffsetX;
    gridoffsetY = InConfig.StartOffsetY;
    levelWidth = inLevelwidth;
    levelHeight = inLevelheight;
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

    auto [gridX, gridY] = ScreenToGrid(xpos, ypos, TileWidth, TileHeight, gridoffsetX,gridoffsetY,Zoom,PanX,PanY,windowWidth, windowHeight, levelWidth,levelHeight);
    auto [x, y] = screenToNDC(xpos, ypos, windowWidth, windowHeight);
    CurrentMouseState.x = xpos;
    CurrentMouseState.y = ypos;
    CurrentMouseState.GridX = gridX;
    CurrentMouseState.GridY = gridY;
    if (OnHover) {
        OnHover(gridX, gridY,x,y); // Trigger hover callback
    }
}

void MouseInteractionAPI::HandleMouseClick(double xpos, double ypos, int windowWidth, int windowHeight)
{
    auto [gridX, gridY] = ScreenToGrid(xpos, ypos, TileWidth, TileHeight, gridoffsetX,gridoffsetY,PanX,PanY,Zoom,windowWidth, windowHeight,levelWidth,levelHeight);
    if (OnClick) {
        OnClick(gridX, gridY,xpos,ypos); // Trigger click callback
    }
}

void MouseInteractionAPI::SetPanZoom(float Panx, float pany, float zoom)
{
    Zoom = zoom;
    PanX = Panx;
    PanY = pany;
}

std::pair<int, int> MouseInteractionAPI::ScreenToGrid(double screenX, double screenY, float tileWidth, float tileHeight, float offsetX, float offsetY, float zoom, float panX, float panY, int windowWidth, int windowHeight, float levelWidth, float levelHeight)
{
    /*   float ndcX = (screenX / windowWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (screenY / windowHeight) * 2.0f;
    int gridX = static_cast<int>(round((ndcY / tileHeight) + (ndcX / tileWidth) ));
    int gridY = static_cast<int>(round((ndcY / tileHeight) - (ndcX / tileWidth) ));
    return { gridX, gridY };
   */
   // Step 1: Convert screen coordinates to Normalized Device Coordinates (NDC)
    float windowAspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    float bgAspectRatio = levelWidth / levelHeight;

    // Adjust background scaling to fit window aspect ratio
    float scaleX = levelWidth / static_cast<float>(windowWidth), scaleY = levelHeight /static_cast<float>(windowHeight);
    if (windowAspectRatio > bgAspectRatio) {
    //    scaleX = windowAspectRatio / bgAspectRatio;
    }
    else {
   //     scaleY = bgAspectRatio / windowAspectRatio;
    }

    float ndcX = ((((screenX) / windowWidth) * 2.0f - 1.0f)/ (zoom* scaleX));
    float ndcY = ((1.0f - ((screenY) / windowHeight) * 2.0f)/ (zoom* scaleY));
    ndcX -= panX; // Apply horizontal pan offset
    ndcY -= panY;
    //ndcX 
    //ndcY -= -1.5 * tileHeight;
    // Debugging NDC values
  //  std::cout << "NDC Coordinates: (" << ndcX << ", " << ndcY << ")" << std::endl;

    // Step 2: Reverse isometric transformation to calculate grid indices
    float approxGridX = (ndcY / tileHeight) + (ndcX / tileWidth) - (offsetX);
    float approxGridY = (ndcY / tileHeight) - (ndcX / tileWidth) - (offsetY);

    // Debugging approximate grid indices before rounding
   // std::cout << "Approx Grid Coordinates: (" << approxGridX << ", " << approxGridY << ")" << std::endl;

    // Step 3: Round to the nearest integer grid index
    int gridX = static_cast<int>(std::floor(approxGridX));
    int gridY = static_cast<int>(std::floor(approxGridY));

    // Debugging final grid indices
    //std::cout << "Final Grid Coordinates: (" << gridX << ", " << gridY << ")" << std::endl;

    return { gridX, gridY };
}

