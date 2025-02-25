#include "CrabPch.h"

#include "ErrorHandler.h"

namespace crab
{

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