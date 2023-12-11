#include "SceneHierarchyPanel.hpp"

void Editor::SceneHierarchyPanel::_render_node(SceneNodePtr& node)
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

            if (object->get_parent() != node)
                SceneNode::set_parent(node, object);
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
            ImGui::PushID(node->get_name().c_str());
            _render_node(node);
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
        if (ImGui::BeginMenu("Geometric Object")) {
            if (ImGui::MenuItem("Sphere")) {
                new_object = std::make_shared<RT::GeometricObjects::Sphere>();
                new_node_name = "Sphere";
            }
            ImGui::MenuItem("Plane");
            ImGui::MenuItem("Disk");
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
            ImGui::EndMenu();
        }
        ImGui::EndMenu();

        if (new_object != nullptr) {
            auto material = std::make_shared<RT::Materials::Phong>();
            new_object->set_material(material);
            SceneNodePtr new_node = std::make_shared<SceneNode>(new_object);
            new_node->set_name(new_node_name);
            SceneNode::set_parent(node, new_node);
        }
    }
    ImGui::MenuItem("Remove");
    ImGui::EndPopup();
}