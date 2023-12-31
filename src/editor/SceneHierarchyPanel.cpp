#include "SceneHierarchyPanel.hpp"

void Editor::SceneHierarchyPanel::_render_node(SceneNodePtr& node, uint32_t id)
{
    // Flags setup --------------------------------------------------------------------------------
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
    flags |= ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
    flags |= ImGuiTreeNodeFlags_FramePadding;
    flags |= ImGuiTreeNodeFlags_DefaultOpen;

    if (is_selected(node))
        flags |= ImGuiTreeNodeFlags_Selected;

    bool is_container = node->is_container();

    if (!is_container)
        flags |= ImGuiTreeNodeFlags_Leaf;

    // Tree
    bool tree_expanded = ImGui::TreeNodeEx(node->get_name().c_str(), flags);

    bool node_clicked = ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left);

    if (node_clicked)
        set_selected(node);

    // Drag and drop ------------------------------------------------------------------------------
    // Drag
    if (!node->is_root() && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        ImGui::SetDragDropPayload("DND_GEOMETRIC_OBJECT", (void*)&node, sizeof(SceneNodePtr));
        ImGui::EndDragDropSource();
    }
    // Drop only when it's container type
    if (is_container && ImGui::BeginDragDropTarget()) {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_GEOMETRIC_OBJECT");

        if (payload) {
            SceneNodePtr object = *reinterpret_cast<SceneNodePtr*>(payload->Data);

            if (object->get_parent() == node)
                return;

            // Changes parent
            SceneNode::set_parent(node, object);

            // Recalculates bounding box and propagates upwards
            node->get_object()->recalculate_bounding_box();
            SceneNode::bounding_box_modified(node);
        }
        ImGui::EndDragDropTarget();
    }

    _popup_context_item(node);

    // Iterates through all children
    if (is_container && tree_expanded) {

        // Copies children vector, since SceneNodes can be removed in
        // recursive calls
        std::vector<SceneNodePtr> children = node->get_children();

        for (SceneNodePtr& node : children) {
            std::string id_string = node->get_name() + std::to_string(id);
            ImGui::PushID(id_string.c_str());
            _render_node(node, ++id);
            ImGui::PopID();
        }
    }
    if (tree_expanded)
        ImGui::TreePop();
}

void Editor::SceneHierarchyPanel::_popup_context_item(SceneNodePtr& node)
{

    if (!ImGui::BeginPopupContextItem())
        return;

    if (node->is_container() && ImGui::BeginMenu("New")) {

        GeometricObjectPtr new_object;
        std::string new_node_name = "Unnamed";

        if (ImGui::BeginMenu("Primitive")) {

            if (ImGui::MenuItem("Sphere")) {
                new_object = std::make_shared<RT::GeometricObjects::Sphere>();
                new_node_name = "Sphere";
            }
            if (ImGui::MenuItem("Disk")) {
                new_object = std::make_shared<RT::GeometricObjects::Disk>();
                new_node_name = "Disk";
            }
            if (ImGui::MenuItem("Box")) {
                new_object = std::make_shared<RT::GeometricObjects::Box>();
                new_node_name = "Box";
            }
            if (ImGui::MenuItem("Rect")) {
                new_object = std::make_shared<RT::GeometricObjects::Rect>();
                new_node_name = "Rect";
            }
            if (ImGui::MenuItem("Cone")) {
                new_object = std::make_shared<RT::GeometricObjects::Cone>();
                new_node_name = "Cone";
            }
            if (ImGui::MenuItem("Cylinder")) {
                new_object = std::make_shared<RT::GeometricObjects::GenericCylinder>();
                new_node_name = "Cylinder";
            }
            if (ImGui::MenuItem("Torus")) {
                new_object = std::make_shared<RT::GeometricObjects::Torus>();
                new_node_name = "Torus";
            }
            if (ImGui::MenuItem("Triangle")) {
                new_object = std::make_shared<RT::GeometricObjects::Triangle>();
                new_node_name = "Triangle";
            }
            if (ImGui::MenuItem("Plane")) {
                new_object = std::make_shared<RT::GeometricObjects::Plane>();
                new_node_name = "Plane";
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Compound")) {

            if (ImGui::MenuItem("Capsule")) {
                new_object = std::make_shared<RT::GeometricObjects::Capsule>();
                new_node_name = "Capsule";
            }
            if (ImGui::MenuItem("Solid Cone")) {
                new_object = std::make_shared<RT::GeometricObjects::SolidCone>();
                new_node_name = "Solid Cone";
            }
            if (ImGui::MenuItem("Solid Cylinder")) {
                new_object = std::make_shared<RT::GeometricObjects::SolidCylinder>();
                new_node_name = "Solid Cylinder";
            }
            if (ImGui::MenuItem("Thick Annulus")) {
                new_object = std::make_shared<RT::GeometricObjects::ThickAnnulus>();
                new_node_name = "Thick Annulus";
            }
            if (ImGui::MenuItem("Bowl")) {
                new_object = std::make_shared<RT::GeometricObjects::Bowl>();
                new_node_name = "Bowl";
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Part objects")) {

            if (ImGui::MenuItem("Annulus")) {
                new_object = std::make_shared<RT::GeometricObjects::Annulus>();
                new_node_name = "Annulus";
            }
            if (ImGui::MenuItem("Part Cylinder")) {
                new_object = std::make_shared<RT::GeometricObjects::PartCylinder>();
                new_node_name = "Part Cylinder";
            }
            if (ImGui::MenuItem("Part Sphere")) {
                new_object = std::make_shared<RT::GeometricObjects::PartSphere>();
                new_node_name = "Part Sphere";
            }
            if (ImGui::MenuItem("Part Torus")) {
                new_object = std::make_shared<RT::GeometricObjects::PartTorus>();
                new_node_name = "Part Torus";
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Container")) {
            if (ImGui::MenuItem("Simple container")) {
                new_object = std::make_shared<RT::GeometricObjects::Container>();
                new_node_name = "Simple container";
            }
            if (ImGui::MenuItem("Bounding volume hierarchy")) {
                new_object = std::make_shared<RT::GeometricObjects::BVH>();
                new_node_name = "BVH";
            }
            if (ImGui::MenuItem("Transform container")) {
                new_object = std::make_shared<RT::GeometricObjects::TransformContainer>();
                new_node_name = "Transform container";
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();

        if (new_object != nullptr) {
            auto material = std::make_shared<RT::Materials::Phong>();
            new_object->set_material(material);
            new_object->recalculate_bounding_box();

            SceneNodePtr new_node = std::make_shared<SceneNode>(new_object);
            new_node->set_name(new_node_name);
            SceneNode::bind_parent(node, new_node);
            SceneNode::bounding_box_modified(new_node);

            set_selected(new_node);
        }
    }
    if (ImGui::MenuItem("Remove")) {
        SceneNode::remove(node);
    }
    ImGui::EndPopup();
}