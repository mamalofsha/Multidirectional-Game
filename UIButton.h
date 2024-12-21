#pragma once
#include <functional>
#include <GLFW/glfw3.h>
#include "UIElement.h"
class UIButton :public  UIElement {
public:
	bool isHovered = false;
	std::function<void()> onClick;

	UIButton(float x, float y, float width, float height, std::function<void()> onClick)
		: UIElement(x, y, width, height), onClick(onClick) {
	}

	void updateHoverState(double mouseX, double mouseY) {
		isHovered = isInside(mouseX, mouseY); // Check if the mouse is within the button bounds
	}

	void cllicked() {
		std::cout << "Wwo";
	}

	void draw() {
		ObjectShader->use();
			ObjectShader->setBool("isHovered", isHovered);
			ObjectShader->setBool("isHidden", isHidden);

		glBindTexture(GL_TEXTURE_2D, ObjectShader->Texture);
		glBindVertexArray(ObjectShader->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}
};
