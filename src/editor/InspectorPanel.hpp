#ifndef __EDITOR_RT_INSPECTOR__
#define __EDITOR_RT_INSPECTOR__
#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include "Panel.hpp"
#include "SceneNode.hpp"

using namespace RT;

namespace Editor {

class InspectorPanel : public Panel {

    enum class InspectorState {
        GeometricObject
    };

public:
    InspectorPanel()
        : Panel("Inspector")
    {
        _state = InspectorState::GeometricObject;
    }

    virtual void _on_render() override
    {
        ImGui::Text("Hi");

        if (_scene_node != nullptr) {
            _render_geometric_object_editor();
        }
    }

    void set_scene_node(const SceneNodePtr& object)
    {
        _scene_node = object;
    }

private:
    void _render_geometric_object_editor();

private:
    InspectorState _state;
    SceneNodePtr _scene_node;
};

}

#endif