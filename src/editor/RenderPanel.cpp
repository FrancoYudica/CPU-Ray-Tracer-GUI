#include "RenderPanel.hpp"
#include <imgui/imgui.h>

using namespace Wolf;

void Editor::RenderPanel::_pre_render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    _window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration;
}

void Editor::RenderPanel::_on_render()
{
    if (!_texture)
        return;
    float aspect_ratio = static_cast<float>(_render_width) / _render_height;

    ImVec2 window_size = get_size();
    ImVec2 size = { static_cast<float>(window_size.x), static_cast<float>(window_size.x) / aspect_ratio };

    ImGui::Image(reinterpret_cast<ImTextureID>(_texture->get_id()), size, { 0, 1 }, { 1, 0 });
}

void Editor::RenderPanel::_post_render()
{
    ImGui::PopStyleVar(1);
}

void Editor::RenderPanel::render_scene(World& world, const RenderSettings::Settings& settings)
{
    RenderSettings::load_settings(world, settings);
    // Rendering
    {
        // Measures render time
        auto timer = Wolf::ScopeTimer([&](Wolf::Time elapsed) {
            _render_time = elapsed.seconds();
        });

        // Scene rendering
        bool render_success = world.render_scene();
        if (!render_success)
            return;
    }
    // Texture configuration
    Rendering::TextureConfig texture_config;
    texture_config.pixel_format = Rendering::TextureTypes::PixelFormat::RGB;
    texture_config.internal_pixel_format = Rendering::TextureTypes::PixelInternalFormat::RGB_8;
    texture_config.pixel_type = Rendering::TextureTypes::PixelType::FLOAT;
    texture_config.min_filter = texture_config.mag_filter = Rendering::TextureTypes::Filter::NEAREST;

    // Stores output buffer in bitmap
    _render_width = world.render_buffer->width;
    _render_height = world.render_buffer->height;

    // Creates a bitmap and copies buffer
    auto bitmap = std::make_shared<Rendering::BitMap<RGBColor>>(
        _render_width,
        _render_height);
    bitmap->copy_buffer(world.render_buffer->buffer_raw_ptr());
    _texture = Rendering::Texture::from_bitmap(bitmap, texture_config);
}