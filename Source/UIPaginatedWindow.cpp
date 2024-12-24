#include "UIPaginatedWindow.h"
#include "UIButton.h"

void UIPaginatedWindow::Draw()
{
	if (IsHidden) return;
	// Render the window background
	auto SharedObjectShader = ObjectShader.lock();
	if (SharedObjectShader)
	{
		SharedObjectShader->use();
		SharedObjectShader->setBool("IsHidden", IsHidden);
		SharedObjectShader->setBool("IsHovered", false);
		SharedObjectShader->setFloat("Alpha", Alpha);

		glBindTexture(GL_TEXTURE_2D, Texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		SharedObjectShader->setFloat("Alpha", 1.0f);

	}
	int StartIndex = CurrentPage * Rows * Columns;
	int EndIndex = StartIndex + (Rows * Columns);
	for (auto It = PageControls.begin(); It != PageControls.end(); ++It) {
		(*It)->Draw();
	}
	for (const auto& [TabName, Buttons] : Tabs) {
		if (TabName != ActiveTab) continue;
		for (int i = StartIndex; i < EndIndex; ++i) {
			if (i >= Buttons.size()) break;
			Buttons[i]->Draw();
		}
	}
}

void UIPaginatedWindow::UpdateChildrenButtons(float InX, float InY)
{
	for (auto& Child : PageControls)
	{
		if (Child)
		{
			if (IsHidden)
			{
				Child->IsHovered = false;
				continue;
			}
			Child->UpdateHoverState(InX, InY);
		}
	}
	int StartIndex = CurrentPage * Rows * Columns;
	int EndIndex;
	if (static_cast<int>((Tabs[ActiveTab].size()) - StartIndex >= (Rows * Columns)))
		EndIndex = StartIndex + (Rows * Columns);
	else
		EndIndex = (Tabs[ActiveTab].size());
	for (const auto& [TabName, Buttons] : Tabs) {
		if (TabName != ActiveTab) continue;
		for (int i = StartIndex; i < EndIndex; ++i) {
			if (IsHidden)
			{
				Buttons[i]->IsHovered = false;
				continue;
			}
			Buttons[i]->UpdateHoverState(InX, InY);
		}
	}
}

void UIPaginatedWindow::NextPage()
{
	if ((CurrentPage + 1) * (Rows * Columns) < Tabs[ActiveTab].size()) {
		CurrentPage++;
	}
}

void UIPaginatedWindow::PreviousPage()
{
	if ((CurrentPage - 1) >= 0) {
		CurrentPage--;
	}
}

void UIPaginatedWindow::UpdateAlpha(float InNewValue)
{
	Alpha += InNewValue ;
}

void UIPaginatedWindow::AddStaticTab(const std::string& InUITabName)
{
	UIText TextData;
	TextData.ActualText = "Hacky";
	TextData.OffSetX = 80.0f;
	TextData.OffSetY = 95.f;
	TextData.Scale = 0.7f;
	std::vector<UIText> OutTextData;
	OutTextData.push_back(TextData);
	TextData.ActualText = "Level " + std::to_string(Tabs[InUITabName].size() + 1);
	TextData.OffSetX = 80.0f;
	TextData.OffSetY = 115.f;
	TextData.Scale = 0.5f;
	OutTextData.push_back(TextData);
	int X = Tabs[InUITabName].size() % Columns;
	int Y = std::floor((Tabs[InUITabName].size() / Columns) % Rows);
	std::unique_ptr<UIButton> Button = std::make_unique<UIButton>(HudPtr->GetUIShader(), -0.7f + (X * SlotSpaceX / 1.2), -0.775f + (Y * SlotSpaceY), 0.2f, 0.2f, []() {
		}, "Assets/Images/profile.png", HudPtr, OutTextData);
	Tabs[InUITabName].push_back(std::move(Button));
}

std::vector<UIButton*>UIPaginatedWindow::GetCatButtons()
{
	std::vector<UIButton*> OutRawPtrVector;
	for (const auto& ButtonPtr : Tabs[ActiveTab])
	{
		OutRawPtrVector.push_back(ButtonPtr.get()); // Retrieve raw pointer from unique_ptr
	}
	return OutRawPtrVector;
}

void UIPaginatedWindow::SetContentSize(std::pair<int, int> InIntPair)
{
	Rows = InIntPair.first;
	Columns = InIntPair.second;
}

void UIPaginatedWindow::SetHidden(bool NewHidden)
{
	if (!NewHidden)
		IsChanging = true;
	else{
		IsChanging = false;
		Alpha = 0.f;
	}Object::SetHidden(NewHidden);
}
