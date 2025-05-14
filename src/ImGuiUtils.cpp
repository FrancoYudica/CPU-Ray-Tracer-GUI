#include "ImGuiUtils.hpp"

bool ImGuiUtils::color_edit(const char* label, RT::RGBColor& color)
{
    float temp_color[3];
    temp_color[0] = color.r;
    temp_color[1] = color.g;
    temp_color[2] = color.b;

    bool dragged = ImGui::ColorEdit3(label, temp_color);
    if (dragged) {
        color.r = temp_color[0];
        color.g = temp_color[1];
        color.b = temp_color[2];
    }
    return dragged;
}

bool ImGuiUtils::input(const char* label, RT::Vec3& vec3, float speed, float reset_value)
{
    bool modified = false;

    float x = vec3.x;
    float y = vec3.y;
    float z = vec3.z;

    ImGui::PushID(label);

    // 2 Columns. 1. Label and 2. for the sliders and buttons
    ImGui::Columns(2, "DragColumn", false);
    ImGui::SetColumnWidth(0, 100);
    ImGui::Text(label);
    ImGui::NextColumn();

    float widthLeft = ImGui::CalcItemWidth();
    int item_width = static_cast<int>(widthLeft / 3);
    ImGui::PushItemWidth(item_width);
    ImGui::PushItemWidth(item_width);
    ImGui::PushItemWidth(item_width);

    // X Button and drag
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));

    if (ImGui::Button("X", { 20, 20 })) {
        vec3.x = reset_value;
        modified = true;
    }

    ImGui::PopStyleColor();

    ImGui::SameLine();
    if (ImGui::DragFloat("##drag x", &x, speed)) {
        vec3.x = x;
        modified = true;
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    // Y Button and drag
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));

    if (ImGui::Button("Y", { 20, 20 })) {
        vec3.y = reset_value;
        modified = true;
    }

    ImGui::PopStyleColor();

    ImGui::SameLine();
    if (ImGui::DragFloat("##drag y", &y, speed)) {
        vec3.y = y;
        modified = true;
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    // Z Button and drag
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 1, 1));

    if (ImGui::Button("Z", { 20, 20 })) {
        vec3.z = reset_value;
        modified = true;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    if (ImGui::DragFloat("##drag z", &z, speed)) {
        vec3.z = z;
        modified = true;
    }
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    return modified;
}

bool ImGuiUtils::input_normal(const char* label, RT::Vec3& vec3, float speed)
{
    bool modified = ImGuiUtils::input(label, vec3, speed);

    bool is_normalized = RT::Math::magnitude(vec3) == 1.0;
    if (!is_normalized) {
        ImGui::SameLine();
        if (ImGui::Button("Normalize")) {
            vec3 = RT::Math::normalize(vec3);
            modified = true;
        }
    }

    return modified;
}

bool ImGuiUtils::input(const char* label, RT::Vec3f& vec3, float speed)
{
    float temp_color[3];
    temp_color[0] = vec3.x;
    temp_color[1] = vec3.y;
    temp_color[2] = vec3.z;

    bool dragged = ImGui::DragFloat3(label, temp_color, speed);

    if (dragged) {
        vec3.x = temp_color[0];
        vec3.y = temp_color[1];
        vec3.z = temp_color[2];
    }
    return dragged;
}

bool ImGuiUtils::input(const char* label, RT::Vec2f& vec2, float speed)
{
    float temp_color[2];
    temp_color[0] = vec2.x;
    temp_color[1] = vec2.y;
    bool dragged = ImGui::DragFloat2(label, temp_color, speed);
    if (dragged) {
        vec2.x = temp_color[0];
        vec2.y = temp_color[1];
    }
    return dragged;
}

bool ImGuiUtils::input(const char* label, float& v, float speed, float min, float max)
{
    bool dragged = ImGui::DragFloat(label, &v, speed, min, max);
    return dragged;
}

bool ImGuiUtils::input_degrees(const char* label, float& angle_radians, float min, float max)
{
    float angle_degrees = RT::Constants::OVER_180_PI * angle_radians;
    bool dragged = ImGui::DragFloat(
        label,
        &angle_degrees,
        1.0f,
        RT::Constants::OVER_180_PI * min,
        RT::Constants::OVER_180_PI * max);
    if (dragged)
        angle_radians = RT::Constants::PI_OVER_180 * angle_degrees;

    return dragged;
}

bool ImGuiUtils::input_degrees(const char* label, RT::Vec3& angle_radians, float min, float max)
{
    float temp_degrees[3];
    temp_degrees[0] = RT::Constants::OVER_180_PI * angle_radians.x;
    temp_degrees[1] = RT::Constants::OVER_180_PI * angle_radians.y;
    temp_degrees[2] = RT::Constants::OVER_180_PI * angle_radians.z;

    bool dragged = ImGui::DragFloat3(
        label,
        temp_degrees, 0.5f,
        min * RT::Constants::OVER_180_PI,
        max * RT::Constants::OVER_180_PI);

    if (dragged) {
        angle_radians.x = RT::Constants::PI_OVER_180 * temp_degrees[0];
        angle_radians.y = RT::Constants::PI_OVER_180 * temp_degrees[1];
        angle_radians.z = RT::Constants::PI_OVER_180 * temp_degrees[2];
    }

    return dragged;
}
