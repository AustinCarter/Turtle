#include <imgui/imgui.h>

#include <string>
#include <filesystem>

namespace Turtle {

	class FileSelector
	{
	public:
		FileSelector();
		FileSelector(const std::string& dir) : m_CurrentDirectory(dir) {}

		// const std::string& SelectFile() const;
		void SelectFile();
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		ImGuiTextFilter m_filter;
		std::string m_SelectedFile = "";



		int32_t m_Selected = -1;
	};

}