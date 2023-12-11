#ifndef __EDITOR_RT_SCENE_HIERARCHY__
#define __EDITOR_RT_SCENE_HIERARCHY__
#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include "Panel.hpp"
#include "SceneNode.hpp"

using namespace RT;

namespace Editor {

class SceneHierarchyPanel : public Panel {

public:
    SceneHierarchyPanel()
        : Panel("Scene Hierarchy")
    {
    }

    void set_root_container(SceneNodePtr root)
    {
        _root_container = root;
        _selected_object = _root_container;
    }

    inline SceneNodePtr get_selected() const { return _selected_object; }
    inline void set_selected(const SceneNodePtr& object) { _selected_object = object; }
    inline bool is_selected(const SceneNodePtr& object) const { return _selected_object.get() == object.get(); }

protected:
    virtual void _on_render() override
    {
        _render_node(_root_container);
    }

private:
    void _render_node(SceneNodePtr& node);

    void _popup_context_item(SceneNodePtr& container);

private:
    SceneNodePtr _root_container;
    SceneNodePtr _selected_object;
};
}

#endif