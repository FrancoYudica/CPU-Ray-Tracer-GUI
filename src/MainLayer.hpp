#include "CameraController.hpp"
#include "WEngine.h"
#include "editor/EditorUI.hpp"
#include "editor/InspectorPanel.hpp"
#include "editor/SceneHierarchyPanel.hpp"
#include "editor/SceneNode.hpp"
#include "CPU-Ray-Tracing/CPURayTracer.hpp"

class MainLayer : public Wolf::Layer {
public:
    Wolf::Rendering::Camera render_camera;
    std::shared_ptr<Wolf::Rendering::Texture> texture;
    RT::World world;
    uint32_t texture_width;
    uint32_t texture_height;
    float render_scene_time;
    glm::uvec2 viewport_size;
    CameraController camera_controller;
    Editor::SceneHierarchyPanel scene_hierarchy_panel;
    Editor::InspectorPanel inspector_panel;
    Editor::EditorUI editor_ui;

public:
    void on_start() override;
    void on_event(const Wolf::Unique<Wolf::Event>& event) override;
    void on_update(const Wolf::Time& delta) override;
    void on_ui_render_start() override;

private:
    void _render_to_texture();
    bool _camera_ui(std::shared_ptr<RT::Camera>& camera);
    void _save_render();
};