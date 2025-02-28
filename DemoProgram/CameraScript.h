#pragma once

class CameraScript : public Script
{
public:
    CameraScript(Entity in_owner);
    ~CameraScript() override;

    void OnUpdate(TimeStamp& in_ts) override;

private:
    float _GetMoveSpeed();
    float _GetRotateSpeed();

    bool m_isRotating = false;
};
