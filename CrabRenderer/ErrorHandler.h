#pragma once

namespace crab
{

std::string TranslateHRESULT(HRESULT hr);
bool        CheckD3D11Result(HRESULT hr, std::string_view in_hintIfFailed);
bool        CheckD3D11Result(HRESULT hr);

}   // namespace crab