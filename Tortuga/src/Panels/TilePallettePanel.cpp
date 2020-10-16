#include "TilePallettePanel.h"

#include <imgui/imgui.h>

namespace Turtle {
	void TilePallettePanel::Display()
	{
		ImGui::Begin("TilePallette"); 
		ImGui::BeginChild("TileSet", ImVec2{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 50 }, false);
		ImGui::Image((void*)m_Texture.get()->GetRendererID(), ImVec2{ (float)m_Texture.get()->GetWidth(), (float)m_Texture.get()->GetHeight()},ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::EndChild();
		if(ImGui::Button("Close"))
			Close();
		ImGui::End();
	}

	void TilePallettePanel::Open()
	{
		m_Active = true;
	}

	void TilePallettePanel::Close()
	{
		m_Active = false;
	}
}