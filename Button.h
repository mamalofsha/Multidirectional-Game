#pragma once
#include <functional>
#include <GLFW/glfw3.h>

class Button {
public:
	std::unique_ptr<Shader> ObjectShader;
	float x, y, width, height;
	bool isHovered = false;
	std::function<void()> onClick;
	bool isHidden = false;

	Button(float x, float y, float width, float height, std::function<void()> onClick)
		: x(x), y(y), width(width), height(height), onClick(onClick) {
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

	bool isInside(float mouseX, float mouseY) {
		float halfWidth = width / 2.0f;
		float halfHeight = height / 2.0f;
		std::cout << x - halfWidth << "," << x + halfWidth << std::endl;
		std::cout << mouseX << ".xz,zx." << mouseY << std::endl;

		return mouseX >= x - halfWidth && mouseX <= x + halfWidth &&
			mouseY <= y + halfHeight && mouseY >= y - halfHeight;
	}
};
