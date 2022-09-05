#include "iWindow.h"

iStyle_Window* iWindow::GetWindowStyle()
{
	if(style != nullptr)
		return style->Window();
	else
		return iGUI::GlobalStyle()->Window();

	return nullptr;
}

void iWindow::UI_SetStyleParams()
{	
	iStyle_Window* getWindowStyle = GetWindowStyle();

	if (!getWindowStyle->IsResizable()) _window_flags |= ImGuiWindowFlags_NoResize;
	if (!getWindowStyle->ShowTitle()) _window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (!getWindowStyle->CanScroll()) _window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!getWindowStyle->IsDraggable()) _window_flags |= ImGuiWindowFlags_NoMove;

	iStyle* currentStyle = CurrentStyle();

	ImGui::GetStyle().WindowPadding = ImVec2(0, 0);

	currentStyle->UI_PushColors();

	if(currentStyle->_backgroundGradient != nullptr)
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0);
}

void iWindow::UI_PopStyleParams()
{
	iStyle* currentStyle = CurrentStyle();

	currentStyle->UI_PopColors();
}

void iWindow::UI_SetAndGetTransform()
{
	iStyle_Window* getWindowStyle = GetWindowStyle();

	int alignW, alignH;

	if(!getWindowStyle->IsResizable())
		ImGui::SetNextWindowSize(ImVec2(GetTransform().scale.x, GetTransform().scale.y), ImGuiCond_Always);

	if(getWindowStyle->_screenPosition != Vector2(-1, -1))
	{
		alignW = Graphics::GetMainWindow().width * getWindowStyle->_screenPosition.x - (GetTransform().scale.x / (1 / getWindowStyle->_screenPosition.x)) + (_imGuiScreenPos.x / Graphics::GetMainWindow().width) + GetTransform().position.x;
		alignH = Graphics::GetMainWindow().height * getWindowStyle->_screenPosition.y - (GetTransform().scale.y / (1 / getWindowStyle->_screenPosition.y)) + (_imGuiScreenPos.y / Graphics::GetMainWindow().height) - GetTransform().position.y;
	}
	else
	{
		alignW = GetTransform().position.x;
		alignH = GetTransform().position.y;
	}

	if(!getWindowStyle->IsDraggable())
		ImGui::SetNextWindowPos(ImVec2(alignW, alignH), ImGuiCond_Always);
}

void iWindow::OnStart()
{
	iStyle_Window* getWindowStyle = GetWindowStyle();

	if(getWindowStyle->SetTransformAtStart())
	{
		ImGui::SetNextWindowPos(ImVec2(GetTransform().position.x, GetTransform().position.y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(GetTransform().scale.x, GetTransform().scale.y), ImGuiCond_Always);
	}
}

void iWindow::UI_RenderBorder(iStyle_Window* windowStyle, bool focused)
{
	iStyle_Border* getCurrentBorder = windowStyle->CurrentBorder();

	if(getCurrentBorder->Size() > 0)
	{
		ImDrawList* draw_list = nullptr;

		draw_list = ImGui::GetForegroundDrawList();

		ImVec2 firstPoint = ImVec2(_imGuiScreenPos.x, _imGuiScreenPos.y);
		ImVec2 secondPoint = ImVec2(_imGuiScreenPos.x + GetTransform().scale.x, _imGuiScreenPos.y);
		ImVec2 thirdPoint = ImVec2(_imGuiScreenPos.x + GetTransform().scale.x, _imGuiScreenPos.y + GetTransform().scale.y);
		ImVec2 forthPoint = ImVec2(_imGuiScreenPos.x, _imGuiScreenPos.y + GetTransform().scale.y);

		ImVec4 getColor = ImVec4(
			getCurrentBorder->MainColor().r,
			getCurrentBorder->MainColor().g,
			getCurrentBorder->MainColor().b,
			getCurrentBorder->MainColor().a
		);

		ImU32 finalCol = ImColor(getColor);

		// 1st line
		draw_list->AddRectFilled(
			ImVec2(firstPoint.x - (getCurrentBorder->Size() / 2) + getCurrentBorder->Radius(), firstPoint.y - (getCurrentBorder->Size() / 2)), 
			ImVec2(secondPoint.x + (getCurrentBorder->Size() / 2) - getCurrentBorder->Radius(), secondPoint.y + (getCurrentBorder->Size() / 2)), 
			finalCol
			);

		if(getCurrentBorder->Radius() != 0)
			draw_list->AddBezierQuadratic(
				ImVec2(secondPoint.x - getCurrentBorder->Radius(), secondPoint.y),
				secondPoint,
				ImVec2(secondPoint.x, secondPoint.y + getCurrentBorder->Radius()), 
				finalCol, 
				getCurrentBorder->Size(), 
				12
				);

		// 2nd line
		draw_list->AddRectFilled(
			ImVec2(secondPoint.x - (getCurrentBorder->Size() / 2), secondPoint.y - (getCurrentBorder->Size() / 2) + getCurrentBorder->Radius()), 
			ImVec2(thirdPoint.x + (getCurrentBorder->Size() / 2), thirdPoint.y + (getCurrentBorder->Size() / 2) - getCurrentBorder->Radius()), 
			finalCol
			);

		if(getCurrentBorder->Radius() != 0)
			draw_list->AddBezierQuadratic(
				ImVec2(thirdPoint.x, thirdPoint.y - getCurrentBorder->Radius()),
				thirdPoint,
				ImVec2(thirdPoint.x - getCurrentBorder->Radius(), thirdPoint.y), 
				finalCol, 
				getCurrentBorder->Size(), 
				12
				);

		// 3rd line
		draw_list->AddRectFilled(
			ImVec2(thirdPoint.x + (getCurrentBorder->Size() / 2) - getCurrentBorder->Radius(), thirdPoint.y - (getCurrentBorder->Size() / 2)), 
			ImVec2(forthPoint.x - (getCurrentBorder->Size() / 2) + getCurrentBorder->Radius(), forthPoint.y + (getCurrentBorder->Size() / 2)), 
			finalCol
			);

		if(getCurrentBorder->Radius() != 0)
			draw_list->AddBezierQuadratic(
				ImVec2(forthPoint.x + getCurrentBorder->Radius(), forthPoint.y),
				forthPoint,
				ImVec2(forthPoint.x, forthPoint.y - getCurrentBorder->Radius()), 
				finalCol, 
				getCurrentBorder->Size(), 
				12
				);

		// 4th line
		draw_list->AddRectFilled(
			ImVec2(forthPoint.x - (getCurrentBorder->Size() / 2), forthPoint.y + (getCurrentBorder->Size() / 2) - getCurrentBorder->Radius()), 
			ImVec2(firstPoint.x + (getCurrentBorder->Size() / 2), firstPoint.y - (getCurrentBorder->Size() / 2) + getCurrentBorder->Radius()), 
			finalCol
			);

		if(getCurrentBorder->Radius() != 0)
			draw_list->AddBezierQuadratic(
				ImVec2(firstPoint.x, firstPoint.y + getCurrentBorder->Radius()),
				firstPoint,
				ImVec2(firstPoint.x + getCurrentBorder->Radius(), firstPoint.y), 
				finalCol, 
				getCurrentBorder->Size(), 
				12
				);
	}
}

void iWindow::ShadeVertsLinearColorGradientSetAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1)
{
    ImVec2 gradient_extent = ImVec2(gradient_p1.x - gradient_p0.x, gradient_p1.y - gradient_p0.y);
    float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
    ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    ImDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
    const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
    const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
    const int col0_a = (int)(col0 >> IM_COL32_A_SHIFT) & 0xFF;
    const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
    const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
    const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
    const int col_delta_a = ((int)(col1 >> IM_COL32_A_SHIFT) & 0xFF) - col0_a;
    for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
    {
        float d = ImDot(ImVec2(vert->pos.x - gradient_p0.x, vert->pos.y - gradient_p0.y), gradient_extent);
        float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
        int r = (int)(col0_r + col_delta_r * t);
        int g = (int)(col0_g + col_delta_g * t);
        int b = (int)(col0_b + col_delta_b * t);
        int a = (int)(col0_a + col_delta_a * t);
        vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
    }
}

void iWindow::UI_RenderBackgroundGradient(iStyle_Window* windowStyle)
{
	iStyle* currentStyle = CurrentStyle();

	iStyle_Border* getCurrentBorder = windowStyle->CurrentBorder();

	if(currentStyle->_backgroundGradient != nullptr)
	{
		std::vector<std::pair<Color, float>> finalB;
	
		if (currentStyle->_backgroundGradient->gradients[0].second != 0)
			finalB.push_back(std::make_pair(currentStyle->_backgroundGradient->gradients[0].first, 0));
	
		for (auto i : currentStyle->_backgroundGradient->gradients)
			finalB.push_back(i);
	
		if(currentStyle->_backgroundGradient->gradients[currentStyle->_backgroundGradient->gradients.size() - 1].second != 100)
			finalB.push_back(std::make_pair(currentStyle->_backgroundGradient->gradients[currentStyle->_backgroundGradient->gradients.size() - 1].first, 100));
		
		ImVec2 topL = ImVec2(_imGuiScreenPos.x, _imGuiScreenPos.y);
		ImVec2 botR = ImVec2(_imGuiScreenPos.x + GetTransform().scale.x, _imGuiScreenPos.y + GetTransform().scale.y);

		for (int i = 1; i < finalB.size(); i++)
		{
			ImVec2 topLFinal = topL, botRFinal = botR;
	
			topLFinal.y = (GetTransform().scale.y * finalB[i - 1].second / 100) + topL.y;
			botRFinal.y = (GetTransform().scale.y * finalB[i].second / 100) + topL.y;
	
			ImColor col2(finalB[i].first.r, finalB[i].first.g, finalB[i].first.b, finalB[i].first.a);
			ImColor col1(finalB[i - 1].first.r, finalB[i - 1].first.g, finalB[i - 1].first.b, finalB[i - 1].first.a);
	
			float finalBorderRadius = 0;
			ImDrawFlags flags;
			if (i == 1)
			{
				if (finalB.size() == 2)
					flags = ImDrawCornerFlags_All;
				else
					flags = ImDrawCornerFlags_Top;
	
				finalBorderRadius = getCurrentBorder->Radius();
			}
			else if (i == finalB.size() - 1)
			{
				flags = ImDrawCornerFlags_Bot;
				finalBorderRadius = getCurrentBorder->Radius();
			}
	
			ImDrawList* final_drawList = nullptr;
	
			//if (final_drawList == nullptr)
				final_drawList = ImGui::GetWindowDrawList();
	
			const int vtx_idx_0 = final_drawList->VtxBuffer.Size;
			final_drawList->AddRectFilled(topLFinal, botRFinal, ImColor(1, 1, 1, 1), finalBorderRadius, flags);
			const int vtx_idx_1 = final_drawList->VtxBuffer.Size;
	
			ShadeVertsLinearColorGradientSetAlpha(final_drawList, vtx_idx_0, vtx_idx_1, topLFinal, ImVec2(topLFinal.x, botRFinal.y), col1, col2);
		}
	}
}

void iWindow::OnUpdate()
{
	UI_SetStyleParams();

	UI_SetAndGetTransform();

	iStyle_Window* getWindowStyle = GetWindowStyle();

	ImGui::GetStyle().WindowRounding = getWindowStyle->CurrentBorder()->Radius();

	if(ImGui::Begin(title.c_str(), nullptr, _window_flags))
	{
		_imGuiScreenPos = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

		if(getWindowStyle->IsDraggable())
			GetTransform().position = _imGuiScreenPos;

		if(getWindowStyle->IsResizable())
			GetTransform().scale = Vector2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		UI_RenderBackgroundGradient(getWindowStyle);

		for(auto i : allElements)
			i->OnUpdate();

		UI_RenderBorder(getWindowStyle, ImGui::IsWindowFocused());
		
		ImGui::End();
	}

	UI_PopStyleParams();
}