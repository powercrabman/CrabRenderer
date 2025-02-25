#pragma once

namespace crab
{

class FileSystem
{
public:
    static std::filesystem::path OpenFileDialog(
        std::wstring_view                         in_dialogTitle,
        const std::filesystem::path&              in_defaultPath,
        const std::vector<std::filesystem::path>& in_filterPatterns,
        std ::wstring_view                        in_filterDescription);
};

}   // namespace crab