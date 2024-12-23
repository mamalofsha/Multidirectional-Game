#pragma once
#include <functional>
#include <GLFW/glfw3.h>
#include "UIElement.h"
#include "HUD.h"
class UIButton :public  UIElement {
public:
	bool isHovered = false;
	std::function<void()> onClick;
	std::string Text;
	HUD* Hudptr;

	UIButton(std::shared_ptr<Shader> shaderProgram, float x, float y, float width, float height, std::function<void()> onClick, const std::string& inText, const std::string& inAssetPath, HUD* inHudptr)
		: UIElement(shaderProgram, x, y, width, height), onClick(onClick) {
		Hudptr = inHudptr;
		Text = inText;
		initializeFromRenderData(Graphics::DrawUIElement(std::vector<float>{x, y}, std::vector<float>{width, height}, inAssetPath.c_str()));
	}
	void updateHoverState(double mouseX, double mouseY) {
		isHovered = isInside(mouseX, mouseY); // Check if the mouse is within the button bounds
	}

	void cllicked() {
		std::cout << Text;
	}

	void draw() {
		ObjectShader->use();
		ObjectShader->setBool("isHovered", isHovered);
		ObjectShader->setBool("isHidden", isHidden);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		if (Text.empty() || !Hudptr)return;
		float screenX = (x + 1.0f) * 0.5f * Hudptr->GetWindowSize().first;
		float screenY = (1.0f - y) * 0.5f * Hudptr->GetWindowSize().second; // Inversion for Y
		Graphics::RenderText(Hudptr->GetFontShader(), Hudptr->GetFontData().first, Hudptr->GetFontData().second, Text, screenX - 100.0f, Hudptr->GetWindowSize().second - screenY, 1.0f, glm::vec3(0.0, 0.0f, 0.0f), Hudptr->GetCharacters());
	}

};
