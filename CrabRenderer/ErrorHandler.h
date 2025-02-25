#pragma once

namespace crab
{

// if failed, return false
bool CheckD3D11Result(HRESULT hr, std::string_view in_hintIfFailed);
bool CheckD3D11Result(HRESULT hr);

}