#include "CrabPch.h"

#include "ErrorHandler.h"

#include "D11Utils.h"

namespace crab
{

std::string TranslateHRESULT(HRESULT hr)
{
    char* errorMsg = nullptr;

    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr,
                   hr,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   reinterpret_cast<LPSTR>(&errorMsg),
                   0,
                   nullptr);

    std::string message = (errorMsg) ? errorMsg : "Unknown error";
    LocalFree(errorMsg);
    return message;
}

bool CheckD3D11Result(HRESULT hr, std::string_view in_hintIfFailed)
{
    if (FAILED(hr))
    {
        std::string hrMessage = TranslateHRESULT(hr);
        std::string message   = fmt::format("\n"
                                            "**********\n"
                                            "D11 assert fail: HRESULT: {}\n"
                                            "Hint : {}\n"
                                            "**********\n",
                                          hrMessage.c_str(),
                                          in_hintIfFailed.data());

        Log::Error(message.c_str());

        OutputDebugStringA(message.c_str());
        __debugbreak();

        return false;
    }
    else
    {
        return true;
    }
}

bool CheckD3D11Result(HRESULT hr)
{
    return CheckD3D11Result(hr, {});
}

}   // namespace crab