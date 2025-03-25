#pragma once

namespace crab
{

class GlobalConstants
{
public:
    static void Init();

    // Camera
    static void UpdateCamera(const Mat4& in_view,
                             const Mat4& in_viewProj,
                             const Mat4& in_invViewProj,
                             const Vec3& in_cameraPos);

    // Post Process
    static void UpdatePostProcessToneMapping(float in_exposure, float in_gamma);
    static void UpdatePostProcessBlurDown(float in_dx, float in_dy);
    static void UpdatePostProcessBlurUp(float in_blurRadius);
    static void UpdatePostProcessCombine(float in_combineStrength);

    // Getters
    static Ref<ConstantBuffer<CB_CAMERA>>&       GetCameraConstant();
    static Ref<ConstantBuffer<CB_TONE_MAPPING>>& GetToneMappingConstant();
    static Ref<ConstantBuffer<CB_BLUR_DOWN>>&    GetBlurDownConstant();
    static Ref<ConstantBuffer<CB_BLUR_UP>>&      GetBlurUpConstant();
    static Ref<ConstantBuffer<CB_COMBINE>>&      GetCombineConstant();

private:
    struct Impl;
    static Scope<Impl> s_impl;
};

}   // namespace crab