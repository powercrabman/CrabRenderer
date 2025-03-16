struct VSInput
{
    float2 position : POSITION;
    float2 texCoord : TEXCOORD;
};

cbuffer CB_PostProcessing : register(b0)
{
    float g_resolutionX; // resolution
    float g_resolutionY; // resolution
    float g_deltaTime; // deltaTime
    float g_pad;
    float4 g_userValues; // iMouse
};

// 채널 텍스처 & 샘플러
Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float frac(float x)
{
    return x - floor(x);
}
float2 frac(float2 x)
{
    return float2(frac(x.x), frac(x.y));
}

float hash1D(float x)
{
    return frac(sin(x) * 152754.742f);
}
float hash2D(float2 x)
{
    return hash1D(x.x + hash1D(x.y));
}

// -------------------- Value Noise --------------------
float valueNoise(float2 p, float f)
{
    float2 floorP = floor(p * f);
    float bl = hash2D(floorP + float2(0, 0));
    float br = hash2D(floorP + float2(1, 0));
    float tl = hash2D(floorP + float2(0, 1));
    float tr = hash2D(floorP + float2(1, 1));

    float2 fr = frac(p * f);
    fr = fr * fr * (3 - 2 * fr);

    float b = lerp(bl, br, fr.x);
    float t = lerp(tl, tr, fr.x);
    return lerp(b, t, fr.y);
}

// ----- 매크로/상수 정의 -----
#define AA      2
#define _Speed  3.0f
#define STEPS   12   // 원래 _Steps = 12.0f
#define _Size   0.3f

// -------------------- Background --------------------
float4 background(float3 ray)
{
    float2 uv = ray.xy;
    if (abs(ray.x) > 0.5f)
        uv.x = ray.z;
    else if (abs(ray.y) > 0.5f)
        uv.y = ray.z;

    float brightness = valueNoise(uv * 3.0f, 100.0f);
    float c = valueNoise(uv * 2.0f, 20.0f);
    brightness = pow(brightness, 256.0f);
    brightness = clamp(brightness * 100.0f, 0.0f, 1.0f);

    float3 stars = brightness * lerp(float3(1, 0.6f, 0.2f), float3(0.2f, 0.6f, 1), c);

    float4 nebulae = g_texture.SampleLevel(g_sampler, uv * 1.5f, 0.0f);
    nebulae.xyz += (nebulae.xxx + nebulae.yyy + nebulae.zzz);
    nebulae.xyz *= 0.25f;
    // 여러번 곱
    nebulae *= nebulae;
    nebulae *= nebulae;
    nebulae *= nebulae;
    nebulae *= nebulae;

    nebulae.xyz += stars;
    return nebulae;
}

// -------------------- Rotate Helper --------------------
void Rotate(inout float3 v, float2 angle)
{
    // rotY
    float2 csy = float2(cos(angle.y), sin(angle.y));
    float2 yz = float2(v.y, v.z);
    yz = csy.x * yz + csy.y * float2(-v.z, v.y);
    v.y = yz.x;
    v.z = yz.y;

    // rotX
    float2 csx = float2(cos(angle.x), sin(angle.x));
    float2 xz = float2(v.x, v.z);
    xz = csx.x * xz + csx.y * float2(-v.z, v.x);
    v.x = xz.x;
    v.z = xz.y;
}

// -------------------- Accretion Disk --------------------
float4 raymarchDisk(float3 ray, float3 zeroPos)
{
    float3 position = zeroPos;
    float lengthPos = length(position.xz);

    // _Steps -> STEPS 로 바꾸고, min(1.0f, lengthPos...) 부분 동일
    float dist = min(1.0f, lengthPos * (1.0f / _Size) * 0.5f) * _Size * 0.4f * (1.0f / STEPS) / (abs(ray.y));
    position += dist * STEPS * ray * 0.5f;

    float2 deltaPos;
    deltaPos.x = -zeroPos.z * 0.01f + zeroPos.x;
    deltaPos.y = zeroPos.x * 0.01f + zeroPos.z;
    deltaPos = normalize(deltaPos - zeroPos.xz);

    float parallel = dot(ray.xz, deltaPos) / (sqrt(lengthPos)) * 0.5f;
    float redShift = parallel + 0.3f;
    redShift *= redShift;
    redShift = clamp(redShift, 0.0f, 1.0f);

    float disMix = clamp((lengthPos - _Size * 2.0f) * (1.0f / _Size) * 0.24f, 0.0f, 1.0f);
    float3 insideCol = lerp(float3(1, 0.8f, 0), float3(0.5f, 0.13f, 0.02f) * 0.2f, disMix);
    insideCol *= lerp(float3(0.4f, 0.2f, 0.1f), float3(1.6f, 2.4f, 4.0f), redShift) * 1.25f;

    redShift += 0.12f;
    redShift *= redShift;

    float4 o = float4(0, 0, 0, 0);

    for (int i2 = 0; i2 < STEPS; i2++)
    {
        float i = float(i2); // 필요하면 i값을 float로 사용
        position -= dist * ray;

        float intensity = clamp(1.0f - abs((i - 0.8f) * (1.0f / STEPS) * 2.0f), 0.0f, 1.0f);
        float lengthPos2 = length(position.xz);

        float distMult = 1.0f;
        distMult *= clamp((lengthPos2 - _Size * 0.75f) * (1.0f / _Size) * 1.5f, 0.0f, 1.0f);
        distMult *= clamp((_Size * 10.0f - lengthPos2) * (1.0f / _Size) * 0.20f, 0.0f, 1.0f);
        distMult *= distMult;

        float u = lengthPos2 + (g_deltaTime * _Size * 0.3f) + intensity * (_Size * 0.2f);

        float rot = fmod(g_deltaTime * _Speed, 8192.0f);
        float2 xy;
        xy.x = -position.z * sin(rot) + position.x * cos(rot);
        xy.y = position.x * sin(rot) + position.z * cos(rot);

        float xval = abs(xy.x / xy.y);
        float angle = 0.02f * atan(xval);

        float freq = 70.0f;
        float n1 = valueNoise(float2(angle, u * (1.0f / _Size) * 0.05f), freq);
        float n2 = valueNoise(float2(angle, u * (1.0f / _Size) * 0.05f), freq * 2.0f);
        float noise = n1 * 0.66f + n2 * 0.33f;

        float extraWidth = noise * (1.0f) * (1.0f - clamp(i * (1.0f / STEPS) * 2.0f - 1.0f, 0.0f, 1.0f));
        float alpha = clamp(noise * (intensity + extraWidth) * ((1.0f / _Size) * 10.0f + 0.01f) * dist * distMult, 0.0f, 1.0f);

        float3 col = 2.0f * lerp(float3(0.3f, 0.2f, 0.15f) * insideCol, insideCol, min(1.0f, intensity * 2.0f));
        float3 newColor = col * alpha + o.rgb * (1.0f - alpha);
        float newAlpha = o.a * (1.0f - alpha) + alpha;

        o.rgb = saturate(newColor);
        o.a = saturate(newAlpha);

        float lengthPos3 = lengthPos2 * (1.0f / _Size);
        o.rgb += redShift * (intensity + 0.5f) * (1.0f / STEPS) * 100.0f * distMult / (lengthPos3 * lengthPos3);
    }

    o.rgb = clamp(o.rgb - 0.005f, 0.0f, 1.0f);
    return o;
}

// -------------------- MAIN PIXEL SHADER --------------------
struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    float2 fragCoord = float2(input.uv.x * g_resolutionX,
                                input.uv.y * g_resolutionY);
    float2 iResolution = float2(g_resolutionX, g_resolutionY);

    float4 colOut = float4(0, 0, 0, 0);

    // AA=2 => 이중 루프
    for (int j = 0; j < AA; j++)
    {
        for (int i = 0; i < AA; i++)
        {
            // 회전
            float2 fragCoordRot;
            fragCoordRot.x = fragCoord.x * 0.985f + fragCoord.y * 0.174f;
            fragCoordRot.y = fragCoord.y * 0.985f - fragCoord.x * 0.174f;
            fragCoordRot += float2(-0.06f, 0.12f) * iResolution;

            float2 subPix = float2(i, j) / float2(AA, AA);
            float2 fc2 = fragCoordRot - iResolution * 0.5f + subPix;
            float3 ray = normalize(float3(fc2.x, fc2.y, iResolution.x));

            float3 pos = float3(
                0.0f,
                0.05f,
                -(20.0f * g_userValues.x / iResolution.y - 10.0f)
                 * (20.0f * g_userValues.x / iResolution.y - 10.0f) * 0.05f
            );
            float2 angle = float2(g_deltaTime * 0.1f, 0.2f);
            angle.y = (2.0f * g_userValues.y / iResolution.y) * 3.14159f + 0.1f + 3.14159f;

            float dist0 = length(pos);
            Rotate(pos, angle);
            angle.xy -= min(0.3f / dist0, 3.14159f) * float2(1, 0.5f);
            Rotate(ray, angle);

            float4 col = float4(0, 0, 0, 0);
            float4 glow = float4(0, 0, 0, 0);
            float4 outCol = float4(100, 0, 0, 0);

            // Black hole & disk steps
            for (int disks = 0; disks < 20; disks++)
            {
                for (int h = 0; h < 6; h++)
                {
                    float dotpos = dot(pos, pos);
                    float invDist = rsqrt(dotpos);
                    float centDist = dotpos * invDist;

                    float stepDist = 0.92f * abs(pos.y / (ray.y));
                    float farLimit = centDist * 0.5f;
                    float closeLimit = centDist * 0.1f + 0.05f * centDist * centDist * (1.0f / _Size);
                    stepDist = min(stepDist, min(farLimit, closeLimit));

                    float invDistSqr = invDist * invDist;
                    float bendForce = stepDist * invDistSqr * _Size * 0.625f;
                    ray = normalize(ray - (bendForce * invDist) * pos);
                    pos += stepDist * ray;

                    glow += float4(1.2f, 1.1f, 1.0f, 1.0f) * (
                              0.01f * stepDist * invDistSqr * invDistSqr
                              * clamp(centDist * 2.0f - 1.2f, 0.0f, 1.0f)
                            );
                }

                float dist2 = length(pos);
                if (dist2 < _Size * 0.1f)
                {
                    // BH 안으로 빨려들어감
                    outCol = float4(col.rgb * col.a + glow.rgb * (1.0f - col.a), 1.0f);
                    break;
                }
                else if (dist2 > _Size * 1000.0f)
                {
                    // 바깥으로 탈출
                    float4 bg = background(ray);
                    outCol = float4(col.rgb * col.a + bg.rgb * (1.0f - col.a) + glow.rgb * (1.0f - col.a), 1.0f);
                    break;
                }
                else if (abs(pos.y) <= _Size * 0.002f)
                {
                    float4 diskCol = raymarchDisk(ray, pos);
                    pos.y = 0.0f;
                    pos += abs(_Size * 0.001f / ray.y) * ray;
                    float4 merged = float4(diskCol.rgb * (1.0f - col.a) + col.rgb,
                                           col.a + diskCol.a * (1.0f - col.a));
                    col = merged;
                }
            }

            if (outCol.r == 100.0f)
            {
                outCol = float4(col.rgb + glow.rgb * (col.a + glow.a), 1.0f);
            }

            outCol.rgb = pow(outCol.rgb, float3(0.6f, 0.6f, 0.6f));
            colOut += outCol / float(AA * AA);
        }
    }

    return colOut;
}
