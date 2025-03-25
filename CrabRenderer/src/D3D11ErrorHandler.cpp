#include "InternalPCH.h"

#include "D3D11ErrorHandler.h"

#include "BasicPipeline.h"

#include <debugapi.h>

namespace crab
{
namespace d3d11
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

    bool CheckOK(HRESULT hr, std::string_view in_hintIfFailed)
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

            log::Error(message.c_str());

            OutputDebugStringA(const_cast<LPCSTR>(message.c_str()));
            __debugbreak();

            return false;
        }
        else
        {
            return true;
        }
    }

    bool CheckOK(HRESULT hr)
    {
        return CheckOK(hr, {});
    }

    bool CheckFail(HRESULT hr, std::string_view in_hintIfFailed)
    {
        return !CheckOK(hr, in_hintIfFailed);
    }

    bool CheckFail(HRESULT hr)
    {
        return !CheckOK(hr);
    }
}
}