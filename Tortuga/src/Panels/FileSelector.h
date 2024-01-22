#if 0
#pragma once
#include <imgui/imgui.h>

#include <filesystem>
#include <string>

namespace Turtle {

	class FileSelector
	{
	public:
		FileSelector();
		FileSelector(const std::string& dir) : m_CurrentDirectory(dir) {}


		// const std::string& SelectFile() const;
		void Display();

		std::string GetSelection();

		bool HasSelected() {return m_HasSelected;}
		bool Active() {return m_Active;}

		void Open() {m_Active = true; m_HasSelected = false;}
		void Close() {m_Active = false;}

		const std::string& SelectFileBlocking();

		void SetFilter(const std::string& filter);
	private:
		std::filesystem::path m_CurrentDirectory;
		ImGuiTextFilter m_Filter;
		std::string m_SelectedFile = "";

		bool m_Active = false;
		bool m_HasSelected = false;



		int32_t m_Selected = -1;
	};

}
#endif
