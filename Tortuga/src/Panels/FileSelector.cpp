#include "FileSelector.h"

#include <iostream>
#include "Turtle/Core/Log.h"


namespace Turtle {

	FileSelector::FileSelector()
	{
		m_CurrentDirectory = std::filesystem::current_path();
	}

	void FileSelector::SelectFile()
	{
		// ImGui::Begin("File Selector");
		ImGui::Begin("File Selector");
		char buffer[1024];
		int i = 0;
		bool changeDir = false;
		// path at top, navigate to parent directories -------------------------------
		for(auto& dir : m_CurrentDirectory)
		{
			std::string name = dir.u8string();
			if(name[0] == '\\')
				continue;
			ImGui::SameLine();  

			strcpy_s(&buffer[i], sizeof(buffer) - i, name.c_str());
			i += name.size();
			strcpy_s(&buffer[i], sizeof(buffer) - i, "\\");	
			i += 1;

			if (ImGui::Button(name.c_str()))
			{
				changeDir = true;
				m_SelectedFile = ""; 
				buffer[i] == '\0';
				break;
			}
			
		}

		if(changeDir)
			m_CurrentDirectory = std::filesystem::path(std::string(buffer));

		// File/Directory selection ------------------------------------------------
		ImGui::BeginChild("Directory view", ImVec2(ImGui::GetWindowContentRegionWidth(), 260), true);
		int index = 0;
		
		for (auto& file : std::filesystem::directory_iterator(m_CurrentDirectory.c_str()))
		{
			if (ImGui::Selectable(file.path().u8string().c_str(), index == m_Selected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				m_Selected = index;
				bool isDir = std::filesystem::is_directory(file.path());

				m_SelectedFile = isDir ? "" : file.path().filename().string();

				if (ImGui::IsMouseDoubleClicked(0))
				{
					if(isDir)
					{
						m_SelectedFile = "";
						m_CurrentDirectory = file.path();
					}		
					else
						TURT_WARN("{0}\\{1}", m_CurrentDirectory.u8string().c_str(), m_SelectedFile.c_str()); //return path		
				}
			}
			index++;
		}
		
		ImGui::EndChild();


		// selection dialogue -----------------------------------------------------
		if(!m_SelectedFile.empty())
			strcpy_s(&buffer[i], sizeof(buffer) - i, m_SelectedFile.c_str());

		if(ImGui::InputText("filename", buffer, sizeof(buffer)))
		{
			m_CurrentDirectory = std::filesystem::path(std::string(buffer));
		}
		if(ImGui::Button("Open"))
		{
			if(!m_SelectedFile.empty())
				TURT_WARN("{0}\\{1}", m_CurrentDirectory.u8string().c_str(), m_SelectedFile.c_str()); //return path
			else
				TURT_ERROR("Please select a valid file. Directory selected.	");
		}
		ImGui::End();

		//return m_CurrentDirectory;
	}
}