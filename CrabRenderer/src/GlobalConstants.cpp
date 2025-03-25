#include "InternalPCH.h"

#include "GlobalConstants.h"

#include "RenderFactory.h"
#include "ShaderSharedData.h"

namespace crab
{

struct GlobalConstants::Impl
{
    // camera
    Ref<ConstantBuffer<CB_CAMERA>> cameraConstant;

    // post process
    Ref<ConstantBuffer<CB_TONE_MAPPING>> toneMappingConstant;
    Ref<ConstantBuffer<CB_BLUR_DOWN>>    blurDownConstant;
    Ref<ConstantBuffer<CB_BLUR_UP>>      blurUpConstant;
    Ref<ConstantBuffer<CB_COMBINE>>      combineConstant;
};

Scope<GlobalConstants::Impl> GlobalConstants::s_impl = nullptr;

//===================================================

void GlobalConstants::Init()
{
    using namespace factory;
    s_impl = CreateScope<Impl>();

    // camera
    s_impl->cameraConstant = CreateConstantBuffer<CB_CAMERA>();

    // post process
    s_impl->toneMappingConstant = CreateRef<ConstantBuffer<CB_TONE_MAPPING>>();
    s_impl->blurDownConstant    = CreateRef<ConstantBuffer<CB_BLUR_DOWN>>();
    s_impl->blurUpConstant      = CreateRef<ConstantBuffer<CB_BLUR_UP>>();
    s_impl->combineConstant     = CreateRef<ConstantBuffer<CB_COMBINE>>();
}

void GlobalConstants::UpdateCamera(const Mat4& in_view,
                                   const Mat4& in_viewProj,
                                   const Mat4& in_invViewProj,
                                   const Vec3& in_cameraPos)
{
    s_impl->cameraConstant->WriteToBuffer(CB_CAMERA {
        .cb_cameraView        = in_view,
        .cb_cameraViewProj    = in_viewProj,
        .cb_cameraInvViewProj = in_invViewProj,
        .cb_cameraPosition    = in_cameraPos });
}

void GlobalConstants::UpdatePostProcessToneMapping(float in_exposure, float in_gamma)
{
    s_impl->toneMappingConstant->WriteToBuffer(CB_TONE_MAPPING {
        .cb_toneMappingExposure = in_exposure,
        .cb_toneMappingGamma    = in_gamma });
}

void GlobalConstants::UpdatePostProcessBlurDown(float in_dx, float in_dy)
{
    s_impl->blurDownConstant->WriteToBuffer(CB_BLUR_DOWN {
        .cb_blurDownDeltaX = in_dx,
        .cb_blurDownDeltaY = in_dy });
}

void GlobalConstants::UpdatePostProcessBlurUp(float in_blurRadius)
{
    s_impl->blurUpConstant->WriteToBuffer(CB_BLUR_UP {
        .cb_blurUpRadius = in_blurRadius });
}

void GlobalConstants::UpdatePostProcessCombine(float in_combineStrength)
{
    s_impl->combineConstant->WriteToBuffer(CB_COMBINE {
        .cb_combineStrength = in_combineStrength });
}

Ref<ConstantBuffer<CB_TONE_MAPPING>>& GlobalConstants::GetToneMappingConstant()
{
    return s_impl->toneMappingConstant;
}

Ref<ConstantBuffer<CB_BLUR_DOWN>>& GlobalConstants::GetBlurDownConstant()
{
    return s_impl->blurDownConstant;
}

Ref<ConstantBuffer<CB_BLUR_UP>>& GlobalConstants::GetBlurUpConstant()
{
    return s_impl->blurUpConstant;
}

Ref<ConstantBuffer<CB_COMBINE>>& GlobalConstants::GetCombineConstant()
{
    return s_impl->combineConstant;
}

Ref<ConstantBuffer<CB_CAMERA>>& GlobalConstants::GetCameraConstant()
{
    return s_impl->cameraConstant;
}

};   // namespace crab