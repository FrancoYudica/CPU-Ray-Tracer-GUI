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
    SceneNode(const GeometricObjectPtr& object)
        : _name("Unnamed")
        , _object(object)
        , _is_container(is_container_type(object->get_type()))
    {
    }

    inline void set_name(const std::string& name) { _name = name; }

    inline const GeometricObjectPtr& get_object() const { return _object; }

    inline const std::string& get_name() const { return _name; }

    inline const SceneNodePtr& get_parent() const { return _parent; }

    inline std::vector<SceneNodePtr>& get_children() { return _children; }

    inline bool is_container() const { return _is_container; }

    inline bool is_root() const { return _parent == nullptr; }

    static void initialize_recursive(SceneNodePtr& node)
    {
        if (!node->is_container())
            return;

        GeometricObjectPtr object = node->get_object();

        auto container = std::dynamic_pointer_cast<GeometricObjects::Container>(object);

        for (int i = 0; i < container->size(); i++) {
            GeometricObjectPtr child_object = *(i + container->begin());
            // Creates child SceneNode
            SceneNodePtr child_node = std::make_shared<SceneNode>(child_object);

            // Links with parent
            node->_children.push_back(child_node);
            child_node->_parent = node;

            SceneNode::initialize_recursive(child_node);
        }
    }

    static void set_parent(SceneNodePtr& parent, SceneNodePtr& child)
    {
        // Unbinds previous parent
        unbind_parent(child);

        // Binds new parent
        bind_parent(parent, child);
    }

    static void unbind_parent(SceneNodePtr& node)
    {
        // Removes child from previous parent
        SceneNodePtr previous_parent = node->get_parent();

        if (!previous_parent)
            return;

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

    static void remove(SceneNodePtr& node)
    {
        unbind_parent(node);

        // Gets parent
        SceneNodePtr parent = node->get_parent();

        // If it has parent
        if (parent) {

            // Recalculates bounding boxes
            parent->get_object()->recalculate_bounding_box();
            SceneNode::bounding_box_modified(parent);
        }
    }

    /// @brief Called when the bounding box of a GeometricObject is modified
    /// propagates this call upwards in 'bottom->up' way.
    static void bounding_box_modified(SceneNodePtr& node)
    {
        if (node == nullptr || node->is_root())
            return;

        SceneNodePtr parent = node->get_parent();
        GeometricObjectPtr parent_container = parent->get_object();
        parent_container->recalculate_bounding_box();
        bounding_box_modified(parent);
    }

private:
    std::string _name;
    GeometricObjectPtr _object;
    SceneNodePtr _parent;
    std::vector<SceneNodePtr> _children;
    const bool _is_container;
};

}

#endif