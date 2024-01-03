#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include "CameraController.hpp"
#include "RenderSettings.hpp"
#include "WEngine.h"
#include "editor/EditorUI.hpp"
#include "editor/InspectorPanel.hpp"
#include "editor/RenderPanel.hpp"
#include "editor/SceneHierarchyPanel.hpp"
#include "editor/SceneNode.hpp"

class MainLayer : public Wolf::Layer {
public:
    Wolf::Rendering::Camera render_camera;
    std::shared_ptr<Wolf::Rendering::Texture> texture;
    RT::World world;
    CameraController camera_controller;
    Editor::SceneHierarchyPanel scene_hierarchy_panel;
    Editor::InspectorPanel inspector_panel;
    Editor::EditorUI editor_ui;
    Editor::RenderPanel render_panel;

    RenderSettings::Settings render_settings;
    RenderSettings::Settings camera_render_settings;
    glm::vec2 mouse_pos;
    bool camera_enabled;

public:
    void on_start() override;
    void on_event(const Wolf::Unique<Wolf::Event>& event) override;
    void on_update(const Wolf::Time& delta) override;
    void on_ui_render_start() override;

private:
    bool _camera_ui(std::shared_ptr<RT::Camera>& camera);

    /// @brief Renders scene to main viewport
    void _render();
    void _save_render();
};