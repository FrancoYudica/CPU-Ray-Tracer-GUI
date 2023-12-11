
#ifndef __IMGUI_UTILS__
#define __IMGUI_UTILS__
#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include <array>
#include <imgui/imgui.h>

namespace ImGuiUtils {
static bool color_edit(const char* label, RT::RGBColor& color, bool& modified)
{
    float temp_color[3];
    temp_color[0] = color.r;
    temp_color[1] = color.g;
    temp_color[2] = color.b;

    bool dragged = ImGui::ColorEdit3(label, temp_color);
    modified |= dragged;
    if (dragged) {
        color.r = temp_color[0];
        color.g = temp_color[1];
        color.b = temp_color[2];
    }
    return dragged;
}
static bool input(const char* label, RT::Vec3& vec3, bool& modified, float speed = 0.25f)
{
    float temp_color[3];
    temp_color[0] = vec3.x;
    temp_color[1] = vec3.y;
    temp_color[2] = vec3.z;

    bool dragged = ImGui::DragFloat3(label, temp_color, speed);
    modified |= dragged;
    if (dragged) {
        vec3.x = temp_color[0];
        vec3.y = temp_color[1];
        vec3.z = temp_color[2];
    }
    return dragged;
}
static bool input(const char* label, RT::Vec3f& vec3, bool& modified, float speed = 0.25f)
{
    float temp_color[3];
    temp_color[0] = vec3.x;
    temp_color[1] = vec3.y;
    temp_color[2] = vec3.z;

    bool dragged = ImGui::DragFloat3(label, temp_color, speed);

    modified |= dragged;
    if (dragged) {
        vec3.x = temp_color[0];
        vec3.y = temp_color[1];
        vec3.z = temp_color[2];
    }
    return dragged;
}

static bool input(const char* label, RT::Vec2f& vec2, bool& modified, float speed = 0.25f)
{
    float temp_color[2];
    temp_color[0] = vec2.x;
    temp_color[1] = vec2.y;
    bool dragged = ImGui::DragFloat2(label, temp_color, speed);
    modified |= dragged;
    if (dragged) {
        vec2.x = temp_color[0];
        vec2.y = temp_color[1];
    }
    return dragged;
}
static bool input(const char* label, float& v, bool& modified, float speed = 0.25f, float min = 0.0f, float max = 0.0f)
{
    bool dragged = ImGui::DragFloat(label, &v, speed, min, max);
    modified |= dragged;
    return dragged;
}

/// @brief float input that shows UI as degrees, but makes internal conversion to radians
/// @param angle_radians Angle in radians
/// @return
static bool input_degrees(const char* label, float& angle_radians, bool& modified, float min = 0.0f, float max = Constants::PI_2)
{
    float angle_degrees = Constants::OVER_180_PI * angle_radians;
    bool dragged = ImGui::DragFloat(
        label,
        &angle_degrees,
        1.0f,
        Constants::OVER_180_PI * min,
        Constants::OVER_180_PI * max);
    if (dragged)
        angle_radians = Constants::PI_OVER_180 * angle_degrees;

    modified |= dragged;
    return dragged;
}


/// @brief float input that shows UI as degrees, but makes internal conversion to radians
/// @param angle_radians Angle in radians
/// @return
static bool input_degrees(const char* label, Vec3& angle_radians, bool& modified, float min = 0.0f, float max = Constants::PI_2)
{
    float temp_degrees[3];
    temp_degrees[0] = Constants::OVER_180_PI * angle_radians.x;
    temp_degrees[1] = Constants::OVER_180_PI * angle_radians.y;
    temp_degrees[2] = Constants::OVER_180_PI * angle_radians.z;

    bool dragged = ImGui::DragFloat3(
        label,
        temp_degrees, 0.5f,
        min * Constants::OVER_180_PI,
        max * Constants::OVER_180_PI);

    if (dragged) {
        angle_radians.x = Constants::PI_OVER_180 * temp_degrees[0];
        angle_radians.y = Constants::PI_OVER_180 * temp_degrees[1];
        angle_radians.z = Constants::PI_OVER_180 * temp_degrees[2];
    }

    modified |= dragged;
    return dragged;
}


template <std::size_t array_magnitude>
static bool combo_box(const char* label, std::array<const char*, array_magnitude> items, int& current_item, bool& modified)
{
    bool modified_combo = ImGui::Combo(label, &current_item, items.data(), array_magnitude);
    modified |= modified_combo;
    return modified_combo;
}
}; // namespace ImGuiUtils

#endif