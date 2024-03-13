#pragma once

// see https://64.github.io/tonemapping

#include <array>

#include "color.h"

float luminance(float3 v)
{
    return dot(v, float3(0.2126f, 0.7152f, 0.0722f));
}

float3 change_luminance(float3 v, float new_luminance)
{
    float old_luminance = luminance(v);
    return v * (new_luminance / old_luminance);
}

enum TonemapFunction
{
    NONE,
    REINHARD,
    EXTENDED_REINHARD,
    EXTENDED_REINHARD_LUMINANCE,
    REINHARD_JODIE,
    UNCHARTED2_FILMIC,
    ACES_FITTED,
    ACES_APPROXIMATED,

    _TM_FUNC_COUNT
};

const char *tonemap_function_name(int fn)
{
    switch (fn)
    {
    case NONE:
        return "none";
    case REINHARD:
        return "reinhard";
    case EXTENDED_REINHARD:
        return "extended-reinhard";
    case EXTENDED_REINHARD_LUMINANCE:
        return "extended-reinhard-lum";
    case REINHARD_JODIE:
        return "reinhard-jodie";
    case UNCHARTED2_FILMIC:
        return "hable-filmic";
    case ACES_FITTED:
        return "aces-fitted";
    case ACES_APPROXIMATED:
        return "aces-approximated";
    }
}

// ========== tonemap functions ==========

float3 tmo_reinhard(float3 v)
{
    return v / (1.0f + v);
}

float3 tmo_extended_reinhard(float3 v, float max_white)
{
    float3 numerator = v * (1.0f + (v / (max_white * max_white)));
    return numerator / (1.0f + v);
}

float3 tmo_extended_reinhard_lum(float3 v, float max_luminance)
{
    float l_old = luminance(v);
    float numerator = l_old * (1.0f + (l_old / (max_luminance * max_luminance)));
    float l_new = numerator / (1.0f + l_old);
    return change_luminance(v, l_new);
}

float3 tmo_reinhard_jodie(float3 v)
{
    float3 t = v / (1.0f + v);
    return lerp(v / (1.0f + luminance(v)), t, t);
}

float3 hable_tmo_partial(float3 x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float3 tmo_hable_filmic(float3 v)
{
    float exposure_bias = 1.5f;
    float3 curr = hable_tmo_partial(v * exposure_bias);

    float3 W = float3(11.2f);
    float3 white_scale = float3(1.0f) / hable_tmo_partial(W);
    return curr * white_scale;
}

static const std::array<float3, 3> aces_input_matrix =
    {
        float3(0.59719f, 0.35458f, 0.04823f),
        float3(0.07600f, 0.90834f, 0.01566f),
        float3(0.02840f, 0.13383f, 0.83777f)};

static const std::array<float3, 3> aces_output_matrix =
    {
        float3(1.60475f, -0.53108f, -0.07367f),
        float3(-0.10208f, 1.10813f, -0.00605f),
        float3(-0.00327f, -0.07276f, 1.07602f)};

float3 aces_mul(const std::array<float3, 3> &m, const float3 &v)
{
    float x = m[0].r * v.r + m[0].g * v.g + m[0].b * v.b;
    float y = m[1].r * v.r + m[1].g * v.g + m[1].b * v.b;
    float z = m[2].r * v.r + m[2].g * v.g + m[2].b * v.b;
    return float3(x, y, z);
}

float3 aces_rtt_and_odt_fit(float3 v)
{
    float3 a = v * (v + 0.0245786f) - 0.000090537f;
    float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

float3 tmo_aces_fitted(float3 v)
{
    v = aces_mul(aces_input_matrix, v);
    v = aces_rtt_and_odt_fit(v);
    return aces_mul(aces_output_matrix, v);
}

float3 tmo_aces_approximated(float3 v)
{
    v = v * 0.6f;
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((v * (a * v + b)) / (v * (c * v + d) + e), 0.0f, 1.0f);
}