#pragma once
#include <functional>


class MouseInteractionAPI {
public:
    using MouseEventCallback = std::function<void(int gridX, int gridY)>;

    void setHoverCallback(MouseEventCallback callback) { onHover = callback; }
    void setClickCallback(MouseEventCallback callback) { onClick = callback; }

    void handleMouseMove(double xpos, double ypos, int windowWidth, int windowHeight, float tileWidth, float tileHeight) {
        auto [gridX, gridY] = screenToGrid(xpos, ypos, tileWidth, tileHeight, windowWidth, windowHeight);
        if (onHover) {
            onHover(gridX, gridY); // Trigger hover callback
        }
    }

    void handleMouseClick(double xpos, double ypos, int windowWidth, int windowHeight, float tileWidth, float tileHeight) {
        auto [gridX, gridY] = screenToGrid(xpos, ypos, tileWidth, tileHeight, windowWidth, windowHeight);
        if (onClick) {
            onClick(gridX, gridY); // Trigger click callback
        }
    }

private:
    MouseEventCallback onHover = nullptr;
    MouseEventCallback onClick = nullptr;

    // Convert screen coordinates to grid coordinates
    std::pair<int, int> screenToGrid(double screenX, double screenY, float tileWidth, float tileHeight, int windowWidth, int windowHeight) {
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
    float approxGridX = (ndcY / tileHeight) + (ndcX / tileWidth) ;
    float approxGridY = (ndcY / tileHeight) - (ndcX / tileWidth) ;

    // Debugging approximate grid indices before rounding
   // std::cout << "Approx Grid Coordinates: (" << approxGridX << ", " << approxGridY << ")" << std::endl;

    // Step 3: Round to the nearest integer grid index
    int gridX = static_cast<int>(std::ceil(approxGridX));
    int gridY = static_cast<int>(std::ceil(approxGridY));

    // Debugging final grid indices
    //std::cout << "Final Grid Coordinates: (" << gridX << ", " << gridY << ")" << std::endl;

    return { gridX, gridY };
    
    }
};