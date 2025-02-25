#include "pch.h"

#include "GUIs.h"

#include "tinyfiledialogs.h"

std::filesystem::path GetPathFromFileDialog()
{
    std::vector<std::filesystem::path> filterPatterns = {
        L"*.png",
        L"*.exr",
        L"*.dds",
        L"*.hdr",
    };

    std::wstring path = FileSystem::OpenFileDialog(
        L"Load Image",
        "",
        filterPatterns,
        L"Image Files");

    return path;
}