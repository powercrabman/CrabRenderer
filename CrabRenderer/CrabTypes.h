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

using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Mat4 = DirectX::SimpleMath::Matrix;
using Quat = DirectX::SimpleMath::Quaternion;

using AABB           = DirectX::BoundingBox;
using OBB            = DirectX::BoundingOrientedBox;
using BoundingShpere = DirectX::BoundingSphere;
using Ray            = DirectX::SimpleMath::Ray;
using Plane          = DirectX::SimpleMath::Plane;

using Color    = DirectX::SimpleMath::Color;
using Viewport = DirectX::SimpleMath::Viewport;
using Bool     = int32;   // for constant buffer

//===================================================
// Data Limit
//===================================================

constexpr int32  CRAB_INT32_MAX  = std::numeric_limits<int32>::max();
constexpr int32  CRAB_INT32_MIN  = std::numeric_limits<int32>::min();
constexpr uint32 CRAB_UINT32_MAX = std::numeric_limits<uint32>::max();
constexpr uint32 CRAB_UINT32_MIN = std::numeric_limits<uint32>::min();
constexpr int64  CRAB_INT64_MAX  = std::numeric_limits<int64>::max();
constexpr int64  CRAB_INT64_MIN  = std::numeric_limits<int64>::min();
constexpr uint64 CRAB_UINT64_MAX = std::numeric_limits<uint64>::max();
constexpr uint64 CRAB_UINT64_MIN = std::numeric_limits<uint64>::min();
constexpr float  CRAB_FLOAT_MAX  = std::numeric_limits<float>::max();
constexpr float  CRAB_FLOAT_MIN  = std::numeric_limits<float>::min();

//===================================================
// Smart Pointer
//===================================================

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
Scope<T> CreateScope(Args... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

}   // namespace crab