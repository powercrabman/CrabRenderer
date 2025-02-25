#include "CrabPch.h"

#include "FileSystem.h"

#include "tinyfiledialogs.h"

namespace crab
{

std::filesystem::path FileSystem::OpenFileDialog(
    std::wstring_view                         in_dialogTitle,
    const std::filesystem::path&              in_defaultPath,
    const std::vector<std::filesystem::path>& in_filterPatterns,
    std::wstring_view                         in_filterDescription)
{
    std::vector<const wchar*> filterPatterns;
    for (auto& filter: in_filterPatterns)
        filterPatterns.push_back(const_cast<wchar*>(filter.c_str()));

    wchar* result = tinyfd_openFileDialogW(
        in_dialogTitle.data(),
        in_defaultPath.c_str(),
        static_cast<uint32>(filterPatterns.size()),
        filterPatterns.data(),
        in_filterDescription.data(),
        0);

    std::wstring path = result ? result : L"";
    return path;
}

}   // namespace crab