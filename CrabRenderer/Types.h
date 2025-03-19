#pragma once

namespace crab
{

//===================================================
// Data Types
//===================================================

using int8  = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8  = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using wchar = wchar_t;
using Index = uint32;

using Vec2    = DirectX::SimpleMath::Vector2;
using Vec3    = DirectX::SimpleMath::Vector3;
using Vec4    = DirectX::SimpleMath::Vector4;
using Mat4    = DirectX::SimpleMath::Matrix;
using Quat    = DirectX::SimpleMath::Quaternion;
using Frustum = DirectX::BoundingFrustum;

using AABB           = DirectX::BoundingBox;
using OBB            = DirectX::BoundingOrientedBox;
using BoundingSphere = DirectX::BoundingSphere;
using Ray            = DirectX::SimpleMath::Ray;
using Plane          = DirectX::SimpleMath::Plane;

using Color4   = DirectX::SimpleMath::Vector4;
using Color3   = DirectX::SimpleMath::Vector3;
using Viewport = DirectX::SimpleMath::Viewport;
using Bool     = int32;   // for constant texture

//===================================================
// Smart Pointer
//===================================================

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
Scope<T> CreateScope(Args&&... args)
{
    return Scope<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
Ref<T> CreateRef(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

}   // namespace crab