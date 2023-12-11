#ifndef __EDITOR_SCENE__
#define __EDITOR_SCENE__
#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include <algorithm>
#include <string>
#include <vector>

namespace Editor {
using namespace RT;

class SceneNode;

typedef std::shared_ptr<SceneNode> SceneNodePtr;

class SceneNode {

public:
    SceneNode(const RT::GeometricObjectPtr& object)
        : _object(object)
        , _name("Unnamed")
    {
        _is_container = is_container_type(_object->get_type());
    }

    inline void set_name(const std::string& name) { _name = name; }

    inline const RT::GeometricObjectPtr& get_object() const { return _object; }

    inline const std::string& get_name() const { return _name; }

    inline const SceneNodePtr& get_parent() const { return _parent; }

    inline std::vector<SceneNodePtr>& get_children() { return _children; }

    inline bool is_container() const { return _is_container; }

    inline bool is_root() const { return _parent == nullptr; }

    static void initialize_recursive(SceneNodePtr& node)
    {
        if (!node->is_container())
            return;

        auto container = std::dynamic_pointer_cast<GeometricObjects::Container>(node->get_object());

        for (int i = 0; i < container->size(); i++) {
            GeometricObjectPtr child_object = *(i + container->begin());
            SceneNodePtr child_node = std::make_shared<SceneNode>(child_object);

            node->_children.push_back(child_node);
            child_node->_parent = node;

            SceneNode::initialize_recursive(child_node);
        }

        // for (auto i = container->begin(); i < container->end(); i++) {
        //     GeometricObjectPtr child_object = *i;
        //     SceneNodePtr child_node = std::make_shared<SceneNode>(child_object);
        //     SceneNode::set_parent(node, child_node);
        //     SceneNode::initialize_recursive(child_node);
        // }
    }

    static void set_parent(SceneNodePtr& parent, SceneNodePtr& child)
    {
        unbind_parent(child);

        bind_parent(parent, child);
    }

    static void unbind_parent(SceneNodePtr& node)
    {
        // Removes child from previous parent
        SceneNodePtr previous_parent = node->get_parent();
        if (previous_parent) {

            // Removes from node
            auto& children = previous_parent->_children;
            auto itr = std::find(children.begin(), children.end(), node);
            if (itr != std::end(children))
                children.erase(itr);

            // Removes from container
            GeometricObjectPtr parent_object = previous_parent->get_object();
            std::shared_ptr<GeometricObjects::Container> previous_parent_container = std::dynamic_pointer_cast<GeometricObjects::Container>(parent_object);
            previous_parent_container->remove(node->get_object());
        }
    }

    static void bind_parent(SceneNodePtr& parent, SceneNodePtr& node)
    {
        // Re-parents nodes
        parent->_children.push_back(node);
        node->_parent = parent;

        // Parents GeometricObjects
        GeometricObjectPtr parent_object = parent->get_object();
        std::shared_ptr<GeometricObjects::Container> parent_container = std::dynamic_pointer_cast<GeometricObjects::Container>(parent_object);
        parent_container->add(node->get_object());
    }

private:
    void _on_immediate_child_added(SceneNodePtr& child)
    {
    }

    void _on_immediate_child_removed(SceneNodePtr& child)
    {
    }

    void _on_child_added(SceneNodePtr& child)
    {
    }

private:
    std::string _name;
    RT::GeometricObjectPtr _object;
    SceneNodePtr _parent;
    std::vector<SceneNodePtr> _children;
    bool _is_container;
};

}
#endif