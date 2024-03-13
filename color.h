#pragma once

#include <stdint.h>

struct float3
{
    float r, g, b;

    float3(float r, float g, float b) : r(r), g(g), b(b) {}
    float3(float x) : r(x), g(x), b(x) {}
};

float3 operator*(const float3 &a, const float3 &b) { return {a.r * b.r, a.g * b.g, a.b * b.b}; }
float3 operator*(const float3 &a, const float b) { return {a.r * b, a.g * b, a.b * b}; }
float3 operator*(const float a, const float3 &b) { return {a * b.r, a * b.g, a * b.b}; }
float3 operator/(const float3 &a, const float3 &b) { return {a.r / b.r, a.g / b.g, a.b / b.b}; }
float3 operator/(const float3 &a, const float b) { return {a.r / b, a.g / b, a.b / b}; }
float3 operator+(const float3 &a, const float3 &b) { return {a.r + b.r, a.g + b.g, a.b + b.b}; }
float3 operator+(const float a, const float3 &b) { return {a + b.r, a + b.g, a + b.b}; }
float3 operator+(const float3 &a, const float b) { return {a.r + b, a.g + b, a.b + b}; }
float3 operator-(const float3& a, const float b) { return {a.r - b, a.g - b, a.b - b}; }
float3 operator-(const float a, const float3& b) { return {a - b.r, a - b.g, a - b.b}; }

float dot(const float3 &a, const float3 &b) { return a.r * b.r + a.g * b.g + a.b * b.b; }
float3 lerp(const float3 &a, const float3 &b, const float3 &t) { return a * (1.0f - t) + b * t; }
float lerp(float a, float b, float t) { return a * (1.0f - t) + b * t; }

float clamp(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }
float3 clamp(const float3 &v, float a, float b) { return {clamp(v.r, a, b), clamp(v.g, a, b), clamp(v.b, a, b)}; }

float3 rgb_to_float3(uint8_t *rgb)
{
    return {rgb[0] / 255.0f, rgb[1] / 255.0f, rgb[2] / 255.0f};
}

void float3_to_rgb(float3 c, uint8_t *dest)
{
    dest[0] = (uint8_t)(c.r * 255.0f);
    dest[1] = (uint8_t)(c.g * 255.0f);
    dest[2] = (uint8_t)(c.b * 255.0f);
}