#include "pch.h"

#include "CameraScript.h"

CameraScript::CameraScript(Entity in_owner)
    : Script(in_owner)
{
}

CameraScript::~CameraScript()
{
    GetAppWindow().EnableMouseRelativeMode(false);
}

void CameraScript::OnUpdate(TimeStamp& in_ts)
{
    TransformComponent& t = GetEntity().GetTransform();

    if (Input::IsKeyDown(eKey::W))
        t.position += t.Forward() * _GetMoveSpeed() * in_ts.deltaTime;

    if (Input::IsKeyDown(eKey::S))
        t.position += t.Backward() * _GetMoveSpeed() * in_ts.deltaTime;

    if (Input::IsKeyDown(eKey::A))
        t.position += t.Left() * _GetMoveSpeed() * in_ts.deltaTime;

    if (Input::IsKeyDown(eKey::D))
        t.position += t.Right() * _GetMoveSpeed() * in_ts.deltaTime;

    if (Input::IsKeyDown(eKey::Q))
        t.position += t.Down() * _GetMoveSpeed() * in_ts.deltaTime;

    if (Input::IsKeyDown(eKey::E))
        t.position += t.Up() * _GetMoveSpeed() * in_ts.deltaTime;

    if (Input::IsKeyPressed(eKey::Space))
    {
        GetAppWindow().EnableMouseRelativeMode(true);
        m_isRotating = true;
    }

    if (Input::IsKeyReleased(eKey::Space))
    {
        GetAppWindow().EnableMouseRelativeMode(false);
        Input::SetMousePosToCenter();
        m_isRotating = false;
    }

    if (m_isRotating)
    {
        auto [dx, dy] = Input::GetMouseRelativeDeltaPos();
        Vec3 euler    = t.rotate.ToEuler();
        euler.x += dy * _GetRotateSpeed() * in_ts.deltaTime;
        euler.y += dx * _GetRotateSpeed() * in_ts.deltaTime;
        t.rotate = Quat::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
    }
}

float CameraScript::_GetMoveSpeed()
{
    return GetEntity().GetComponent<CameraControlComponent>().moveSpeed;
}

float CameraScript::_GetRotateSpeed()
{
    return GetEntity().GetComponent<CameraControlComponent>().rotSpeed;
}