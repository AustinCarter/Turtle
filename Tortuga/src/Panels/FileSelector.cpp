#if 0
#include "FileSelector.h"

#include "Turtle/Core/Log.h"
#include <iostream>


namespace Turtle {

	FileSelector::FileSelector()
	{
		m_CurrentDirectory = std::filesystem::current_path();
	}

	void FileSelector::Display()
	{
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
			if (ImGui::Button(name.c_str()))
			{
				//NOTE: DOES NOT WORK FOR REDUNDANTLY NAMED DIRECTOREIS:
				//	EG: directory/directory/dir - The first directory button will work as will dir, however the second directory will not
				changeDir = true;
				m_SelectedFile = ""; 
				break;
			}	
			strcpy_s(&buffer[i], sizeof(buffer) - i, "\\");	
			i += 1;
			
		}

		if(changeDir)
			m_CurrentDirectory = std::filesystem::path(std::string(buffer));

		// File/Directory selection ------------------------------------------------
		ImGui::BeginChild("Directory view", ImVec2(ImGui::GetWindowContentRegionWidth(), 260), true);
		int index = 0;
		
		for (auto& file : std::filesystem::directory_iterator(m_CurrentDirectory.c_str()))
		{
			if (m_Filter.PassFilter(file.path().u8string().c_str()) || std::filesystem::is_directory(file))
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
							m_HasSelected = true; 		
					}
				}
				index++;
			}
		}
		
		ImGui::EndChild();


		// selection dialogue -----------------------------------------------------
		if(!m_SelectedFile.empty())
			strcpy_s(&buffer[i], sizeof(buffer) - i, m_SelectedFile.c_str());

		if(ImGui::InputText("", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			auto& newPath = std::filesystem::path(std::string(buffer));
			if(std::filesystem::exists(newPath))
			{
				m_CurrentDirectory = newPath;
				m_SelectedFile = "";
			}
		}
		ImGui::SameLine();
		m_Filter.Draw("filter", ImGui::GetContentRegionAvailWidth());
		if(ImGui::Button("Open"))
		{
			if(!m_SelectedFile.empty())
				m_HasSelected = true;
			else
				TURT_CORE_ERROR("Directory selection not supported");
		}
		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::SameLine();
		if(ImGui::Button("Close"))
			Close();
		ImGui::End();
	}

	std::string FileSelector::GetSelection()
	{
		char buffer[1024];
		// uint32_t index = std::filesystem::current_path().
		std::filesystem::path returnDir = std::filesystem::relative(m_CurrentDirectory);
		sprintf_s(buffer, 1024, "%s/%s", returnDir.generic_u8string().c_str(), m_SelectedFile.c_str());
		return std::string(buffer);
	}

	void FileSelector::SetFilter(const std::string& filter)
	{
		strcpy(m_Filter.InputBuf, filter.c_str());
		m_Filter.Build();
	}

	const std::string& FileSelector::SelectFileBlocking()
	{
		m_Active = true;
		while(m_Active)
		{
			Display();
			if(m_HasSelected)
			{
				Close();
				return m_SelectedFile;
			}
		}
		return "";
	}
}
#endif
