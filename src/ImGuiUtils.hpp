
#ifndef __IMGUI_UTILS__
#define __IMGUI_UTILS__
#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include <array>
#include <imgui/imgui.h>

namespace ImGuiUtils {
bool color_edit(const char* label, RT::RGBColor& color);

bool input(const char* label, RT::Vec3& vec3, float speed = 0.25f, float reset_value = 0.0f);

bool input_normal(const char* label, RT::Vec3& vec3, float speed = 0.25f);

bool input(const char* label, RT::Vec3f& vec3, float speed = 0.25f);

bool input(const char* label, RT::Vec2f& vec2, float speed = 0.25f);

bool input(const char* label, float& v, float speed = 0.25f, float min = 0.0f, float max = 0.0f);

/// @brief float input that shows UI as degrees, but makes internal conversion to radians
/// @param angle_radians Angle in radians
/// @return
bool input_degrees(const char* label, float& angle_radians, float min = 0.0f, float max = RT::Constants::PI_2);

/// @brief float input that shows UI as degrees, but makes internal conversion to radians
/// @param angle_radians Angle in radians
/// @return
bool input_degrees(const char* label, RT::Vec3& angle_radians, float min = 0.0f, float max = RT::Constants::PI_2);

template <std::size_t array_magnitude>
bool combo_box(const char* label, std::array<const char*, array_magnitude> items, int& current_item)
{
    bool modified_combo = ImGui::Combo(label, &current_item, items.data(), array_magnitude);
    return modified_combo;
}
}; // namespace ImGuiUtils

#endif