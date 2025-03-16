#include "pch.h"

#include "GUIs.h"

#include "tinyfiledialogs.h"

#include <SDL3/SDL_dialog.h>

std::filesystem::path GetPathFromFileDialog()
{

    std::wstring path = FileSystem::OpenFileDialog(
        L"Load Image",
        "",
        {},
        L"");

    return path;
}