#pragma once

namespace crab
{
namespace d3d11
{
    std::string TranslateHRESULT(HRESULT hr);
    
    // Check Succeeded HRESULT
    bool CheckOK(HRESULT hr, std::string_view in_hintIfFailed);
    bool CheckOK(HRESULT hr);

    // Check Failed HRESULT
    bool CheckFail(HRESULT hr, std::string_view in_hintIfFailed);
    bool CheckFail(HRESULT hr);

}   // namespace d3d11
}   // namespace crab