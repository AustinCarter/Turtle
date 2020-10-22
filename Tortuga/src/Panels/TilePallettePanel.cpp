#include "TilePallettePanel.h"

#include "Turtle/Core/Log.h"

#include <imgui/imgui.h>

namespace Turtle {
	void TilePallettePanel::Display()
	{
		ImGui::Begin("Tile Pallette"); 
		ImGuiIO& io = ImGui::GetIO();
		ImGui::BeginChild("Tile Set", ImVec2{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 50 }, false);
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)m_Texture.get()->GetRendererID(), ImVec2{ (float)m_Texture.get()->GetWidth(), (float)m_Texture.get()->GetHeight()},ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		if(!m_TileSize[0]) m_TileSize[0] = m_Texture->GetWidth();
		if(!m_TileSize[1]) m_TileSize[1] = m_Texture->GetHeight();

		for(int x = 1; x < m_Texture->GetWidth()/m_TileSize[0]; x++)
		{
			ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + m_TileSize[0] * x , p.y), ImVec2(p.x + m_TileSize[0] * x, p.y + m_Texture->GetHeight()), IM_COL32(0, 0, 0, 255), 1.0f);
		}
		for(int y = 1; y < m_Texture->GetHeight()/m_TileSize[1]; y++)
		{
			ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x, p.y + m_TileSize[1] * y), ImVec2(p.x +  m_Texture->GetWidth(), p.y + m_TileSize[1] * y), IM_COL32(0, 0, 0, 255), 1.0f);
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
		{
			m_Selected[0] = (int)(io.MousePos.x - p.x) / m_TileSize[0];
			m_Selected[1] = (int)(io.MousePos.y - p.y) / m_TileSize[1];
			// TURT_WARN("selected: {0}, {1}", (int)(io.MousePos.x - p.x)/128, (int)(io.MousePos.y - p.y)/128);
		}

		int xPos = m_TileSize[0] * m_Selected[0];
		int yPos = m_TileSize[1] * m_Selected[1];
		ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos), ImVec2(p.x + xPos + m_TileSize[0], p.y + yPos), IM_COL32(255, 0, 0, 255), 3.0f);
		ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos + m_TileSize[0] , p.y + yPos), ImVec2(p.x + xPos + m_TileSize[0], p.y + yPos + m_TileSize[1]), IM_COL32(255, 0, 0, 255), 3.0f);
		ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos ), ImVec2(p.x + xPos, p.y + yPos + m_TileSize[1]), IM_COL32(255, 0, 0, 255), 3.0f);
		ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + xPos , p.y + yPos + m_TileSize[1]), ImVec2(p.x + xPos + m_TileSize[0], p.y + yPos + m_TileSize[1]), IM_COL32(255, 0, 0, 255), 3.0f);

		ImGui::EndChild();
		if(ImGui::Button("Close"))
			Close();
		ImGui::End();
	}

	void TilePallettePanel::Open()
	{
		m_Active = true;
	}

	void TilePallettePanel::SetTileSize(int tileWidth, int tileHeight)
	{
		m_TileSize[0] = tileWidth;
		m_TileSize[1] = tileHeight;
	}

	void TilePallettePanel::Close()
	{
		m_Active = false;
	}
}