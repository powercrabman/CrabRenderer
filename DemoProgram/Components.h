#pragma once

// maybe MeshRenderer and ModelRenderer can be merged
struct CameraControlComponent
{
    float moveSpeed = 1.f;
    float rotSpeed  = 15.f * DEG2RAD;
};