#include "MainLayer.hpp"
#include "ImGuiRT.hpp"
#include "ImGuiUtils.hpp"
#include <imgui/imgui.h>

#include <array>
#include <chrono>
#include <ctime>
#include <time.h>

using namespace Wolf;
using namespace RT;

static constexpr const char* s_imgui_init_relative_path = "../imgui.ini";

void MainLayer::on_start()
{
    auto& window = Application::get()->get_main_window();
    render_camera = Rendering::Camera(
        window->get_width(),
        window->get_height(),
        1.0f);

    world = World();
    world.set_build(BuildFunctions::build_instance_test_bvh);
    world.build();

    camera_controller.set_eye(world.camera->eye);
    camera_controller.set_look_at(world.camera->look_at);

    // Loads imgui layout
    ImGui::LoadIniSettingsFromDisk(s_imgui_init_relative_path);
    auto& io = ImGui::GetIO();

    // Disables imgui.ini auto saving
    io.IniFilename = NULL;
    _render_to_texture();

    Editor::SceneNodePtr scene_root = std::make_shared<Editor::SceneNode>(world.root_container);
    scene_root->set_name("Root");
    Editor::SceneNode::initialize_recursive(scene_root);

    scene_hierarchy_panel.set_root_container(scene_root);
    editor_ui.init();
}

void MainLayer::on_event(const Unique<Event>& event)
{
    EventDispatcher dispatcher(event);

    dispatcher.dispatch<WindowResizeEvent>(
        Wolf::EventType::WindowResize,
        [&](const Unique<WindowResizeEvent>& event) {
            render_camera.on_viewport_resize(event->width, event->height);
            return false;
        });
}

void MainLayer::on_update(const Wolf::Time& delta)
{

    bool using_camera = viewport_hovered && Wolf::Input::is_button_down(MouseButton::RIGHT);

    // Calculates mouse delta
    glm::vec2 current_mouse_pos = Wolf::Input::get_mouse_pos_norm();
    auto p_norm = Wolf::Input::get_mouse_pos_norm();
    glm::vec2 mouse_delta = current_mouse_pos - mouse_pos;

    // Sets mouse mode
    if (using_camera) {
        Wolf::Cursor::set_mode(Cursor::CursorMode::Disabled);
        Wolf::Input::set_mouse_pos_norm(mouse_pos);

    } else {
        Wolf::Cursor::set_mode(Cursor::CursorMode::Normal);
        mouse_pos = current_mouse_pos;
    }

    if (using_camera && camera_controller.on_update(mouse_delta, delta)) {
        world.camera->set_eye(camera_controller.get_eye());
        world.camera->set_look_at(camera_controller.get_look_at());
        world.camera->setup_camera();
        _render_to_texture();
    }
}

void MainLayer::_render_to_texture()
{

    {
        // Measures render time
        auto timer = Wolf::ScopeTimer([&](Wolf::Time elapsed) {
            render_scene_time = elapsed.seconds();
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
    texture_width = world.render_buffer->width;
    texture_height = world.render_buffer->height;

    // Creates a bitmap and copies buffer
    auto bitmap = std::make_shared<Rendering::BitMap<RGBColor>>(
        world.render_buffer->width,
        world.render_buffer->height);
    bitmap->copy_buffer(world.render_buffer->buffer_raw_ptr());
    texture = Rendering::Texture::from_bitmap(bitmap, texture_config);
}

void MainLayer::on_ui_render_start()
{
    ImGui::ShowDemoWindow();
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(2);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {

                if (ImGui::MenuItem("Exit", NULL, false, true)) {
                    Application::get()->quit();
                }
                if (ImGui::MenuItem("Save imgui layout", NULL, false, true)) {
                    ImGui::SaveIniSettingsToDisk(s_imgui_init_relative_path);
                }
                if (ImGui::MenuItem("Save render", NULL, false, true)) {
                    _save_render();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    scene_hierarchy_panel.show();

    inspector_panel.set_scene_node(scene_hierarchy_panel.get_selected());
    inspector_panel.show();
    // Renderer configs ---------------------------------------------------------------------
    {
        ImGui::Begin("Renderer");

        ImGui::Checkbox("Show out of gamut", &world.show_out_of_gamut);
        ImGui::Separator();

        ImGuiUtils::color_edit("Out of gamut color", world.out_of_gamut_color);
        ImGui::Separator();

        if (ImGui::Button("Render"))
            _render_to_texture();

        std::string render_time = "elapsed: ";
        ImGui::SameLine();
        render_time += std::to_string(render_scene_time);
        ImGui::Text("%s", render_time.c_str());

        ImGui::End();
    }

    ImGui::Begin("Ray Tracer configurations");

    // General configurations ---------------------------------------------------------------
    if (ImGui::TreeNodeEx("View plane", ImGuiTreeNodeFlags_Framed)) {

        ImGui::SliderFloat("Pixel size", &world.view_plane.pixel_size, 0.01f, 10.0f);

        {
            int horizontal_resolution = world.view_plane.h_res;
            bool modified_resolution = ImGui::InputInt("Horizontal resolution", (int*)&horizontal_resolution);
            int vertical_resolution = world.view_plane.v_res;
            modified_resolution |= ImGui::InputInt("Vertical resolution", (int*)&vertical_resolution);

            if (ImGui::Button("Resize to viewport")) {
                modified_resolution = true;
                horizontal_resolution = viewport_size.x;
                vertical_resolution = viewport_size.y;
            }

            if (modified_resolution) {
                world.view_plane.set_hres(horizontal_resolution);
                world.view_plane.set_vres(vertical_resolution);
            }
        }
        ImGuiRT::edit_sampler("View plane sampler", world.view_plane.sampler);
        world.view_plane.samples = world.view_plane.sampler->get_num_samples();

        ImGui::TreePop();
    }

    // Main camera configurations -----------------------------------------------------------
    if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_Framed)) {
        std::shared_ptr<Camera> cam = world.camera;
        if (ImGuiRT::camera_edit(cam, world))
            world.set_camera(cam);

        CameraType cam_type = world.camera->get_camera_type();

        // If any stereo camera is used
        if (
            cam_type == CameraType::StereoDual || cam_type == CameraType::StereoAnaglyph) {
            auto stereo_camera = std::dynamic_pointer_cast<Cameras::StereoCamera>(world.camera);
            std::shared_ptr<Camera> left_cam = stereo_camera->left_camera;
            std::shared_ptr<Camera> right_cam = stereo_camera->right_camera;

            ImGui::PushID("right");

            if (ImGui::TreeNodeEx("Stereo right camera")) {
                if (ImGuiRT::camera_edit(right_cam, world)) {
                    stereo_camera->set_right_camera(right_cam);
                    stereo_camera->setup_camera();
                }
                ImGui::TreePop();
            }

            ImGui::PushID("left");
            if (ImGui::TreeNodeEx("Stereo left camera")) {
                if (ImGuiRT::camera_edit(left_cam, world)) {
                    stereo_camera->set_left_camera(left_cam);
                    stereo_camera->setup_camera();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
            ImGui::PopID();
        }

        ImGui::TreePop();
    }
    // Tracer configuration -----------------------------------------------------------------
    if (ImGui::TreeNodeEx("Tracer", ImGuiTreeNodeFlags_Framed)) {
        int current_item = static_cast<int>(world.tracer->get_type());
        bool modified_tracer = ImGuiUtils::combo_box<4>(
            "Tracer",
            { "Ray cast",
                "Depth",
                "Normal",
                "Area lighting" },
            current_item);
        if (modified_tracer) {
            TracerType selected_type = static_cast<TracerType>(current_item);
            if (selected_type != world.tracer->get_type()) {
                std::shared_ptr<Tracer> new_tracer;
                switch (selected_type) {
                case TracerType::RayCast:
                    new_tracer = std::make_shared<Tracers::RayCastTracer>(world);
                    break;
                case TracerType::Depth:
                    new_tracer = std::make_shared<Tracers::DepthTracer>(world);
                    break;
                case TracerType::Normal:
                    new_tracer = std::make_shared<Tracers::NormalTracer>(world);
                    break;
                case TracerType::AreaLighting:
                    new_tracer = std::make_shared<Tracers::AreaLighting>(world);
                    break;
                }
                world.set_tracer(new_tracer);
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();
    // Scene configurations -----------------------------------------------------
    {
        ImGui::Begin("Scene");
        bool modified_scene = false;
        ImGuiUtils::color_edit("Background color", world.background_color);

        ImGuiRT::edit_light(world.ambient_light);

        ImGui::Separator();

        if (ImGui::TreeNodeEx("Lights", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
            // Displays editor for all lights
            int i = 0;
            for (auto& light : world.lights) {
                ImGui::PushID(i++);
                ImGuiRT::edit_light(light);
                ImGui::PopID();
            }
            ImGui::TreePop();
        }

        ImGui::End();
    }
    // Editor specific configurations -----------------------------------------------------------------------
    {
        ImGui::Begin("Editor configurations");
        bool modified_editor = false;
        if (ImGui::TreeNodeEx("Editor camera", ImGuiTreeNodeFlags_Framed)) {
            Vec2f sensibility = camera_controller.get_sensibility();
            float speed = camera_controller.get_speed();
            Vec3f eye = camera_controller.get_eye();
            Vec3f look_at = camera_controller.get_look_at();

            ImGuiUtils::input("Sensibility", sensibility, modified_editor);
            ImGui::InputFloat("Speed", &speed);
            if (ImGuiUtils::input("Eye", eye, modified_editor)) {
                camera_controller.set_eye(eye);
                world.camera->set_eye(eye);
            }
            if (ImGuiUtils::input("Look at", look_at, modified_editor)) {
                camera_controller.set_look_at(look_at);
                world.camera->set_look_at(look_at);
            }

            camera_controller.set_sensibility(sensibility);
            camera_controller.set_speed(speed);

            ImGui::TreePop();
        }
        ImGui::End();
    }
    // Render window -----------------------------------------------------------------------------
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::Begin("Render", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
        viewport_pos.x = ImGui::GetCursorScreenPos().x;
        viewport_pos.y = ImGui::GetCursorScreenPos().y;
        viewport_size.x = ImGui::GetContentRegionAvail().x;
        viewport_size.y = ImGui::GetContentRegionAvail().y;
        float aspect_ratio = static_cast<float>(world.render_buffer->width) / world.render_buffer->height;
        ImVec2 size = { static_cast<float>(viewport_size.x), static_cast<float>(viewport_size.x) / aspect_ratio };

        ImGui::Image(reinterpret_cast<ImTextureID>(texture->get_id()), size, { 0, 1 }, { 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar(1);

        auto p1 = viewport_pos + viewport_size;
        auto mouse = ImGui::GetMousePos();
        viewport_hovered = viewport_pos.x < mouse.x && viewport_pos.y < mouse.y && mouse.x < p1.x && mouse.y < p1.y;
    }
}

void MainLayer::_save_render()
{
    // Gets system date and time
    std::string relative_save_folder = "./../renders/screenshots/";
    std::string image_name = relative_save_folder;
    {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

        image_name += buf;
    }
    image_name += ".png";
    std::cout << "Saved image at: " << image_name << std::endl;

    // Creates temporary RGBA_8 buffer
    auto buffer = std::make_shared<Wolf::Rendering::BitMap<Wolf::RGBA8_UI>>(
        world.render_buffer->width,
        world.render_buffer->height);

    // Fills new buffer
    for (uint32_t x = 0; x < buffer->width; x++) {
        for (uint32_t y = 0; y < buffer->height; y++) {
            auto rendered_color = world.render_buffer->get_pixel(x, y);
            rendered_color = glm::clamp(rendered_color, RT::RGBColor(0), RT::RGBColor(1));
            RGBA8_UI color(
                static_cast<uint8_t>(rendered_color.r * 255),
                static_cast<uint8_t>(rendered_color.g * 255),
                static_cast<uint8_t>(rendered_color.b * 255),
                255);
            buffer->set_pixel({ x, y }, color);
        }
    }
    Wolf::Assets::save_bitmap_png(image_name, buffer);
}