#include "GridObject.h"

void GridObject::Draw()
{
    if (!WorldPtr->GetGridDraw())return;
    auto [WindowWidth, WindowHeight] = WorldPtr->GetWindowSize();
    float ScaleX = (WorldPtr->GetLevelSize().first) / WindowWidth;
    float ScaleY = (WorldPtr->GetLevelSize().second) / WindowHeight;
    glm::mat4 Transform = glm::mat4(1.0f);
    GLuint TransformLoc;
    ObjectShader->setUniform2i("TileCoor", WorldPtr->GetMouseState().GridX, WorldPtr->GetMouseState().GridY);
    Transform = glm::scale(Transform, glm::vec3(ScaleX * WorldPtr->GetZoom(), ScaleY * WorldPtr->GetZoom(), 1.0f));
    Transform = glm::translate(Transform, glm::vec3(WorldPtr->GetPan().first, WorldPtr->GetPan().second, 0.0f));
    TransformLoc = glGetUniformLocation(ObjectShader->ID, "Transform");
    glUniformMatrix4fv(TransformLoc, 1, GL_FALSE, glm::value_ptr(Transform));
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, GridVerticesSize / 2);
}
