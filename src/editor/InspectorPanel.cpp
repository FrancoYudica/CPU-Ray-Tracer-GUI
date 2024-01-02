#include "InspectorPanel.hpp"
#include "../GeometricObjectEditors.hpp"
#include "../ImGuiRT.hpp"
#include <memory>
#include <string>

using namespace RT;

void Editor::InspectorPanel::_render_geometric_object_editor(SceneNodePtr& scene_node)
{

    GeometricObjectPtr _object = scene_node->get_object();

    static const std::array<const char*, 23> geometric_types = {
        "Sphere",
        "Plane",
        "Box",
        "Disk",
        "Rect",
        "Triangle",
        "GenericCylinder",
        "Torus",
        "Part Cylinder",
        "Part Sphere",
        "Solid Cylinder",
        "Capsule",
        "Part Torus",
        "Compound Box",
        "Annulus",
        "Cone",
        "Solid Cone",
        "Thick Annulus",
        "Bowl",
        "Instance",
        "BVH",
        "Container",
        "Transform"
    };
    bool modified = false;
    const char* geometric_type_name = geometric_types[static_cast<uint32_t>(_object->get_type())];
    std::string title = std::string(geometric_type_name) + ": " + scene_node->get_name();
    ImGui::Text(title.c_str());

    ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

    uint32_t edit_states = Editor::ObjectEditor::EditState::None;

    if (ImGui::TreeNodeEx(geometric_type_name, tree_flags)) {
        switch (_object->get_type()) {

        case GeometricObjectType::Sphere:
            edit_states = Editor::ObjectEditor::edit_sphere(_object);
            break;

        case GeometricObjectType::Plane:
            edit_states = Editor::ObjectEditor::edit_plane(_object);
            break;

        case GeometricObjectType::Box:
            edit_states = Editor::ObjectEditor::edit_box(_object);
            break;

        case GeometricObjectType::CompoundBox:
            edit_states = Editor::ObjectEditor::edit_compound_box(_object);
            break;

        case GeometricObjectType::Capsule:
            edit_states = Editor::ObjectEditor::edit_capsule(_object);
            break;

        case GeometricObjectType::Disk:
            edit_states = Editor::ObjectEditor::edit_disk(_object);
            break;

        case GeometricObjectType::Annulus:
            edit_states = Editor::ObjectEditor::edit_annulus(_object);
            break;

        case GeometricObjectType::Rect:
            edit_states = Editor::ObjectEditor::edit_rect(_object);
            break;

        case GeometricObjectType::Triangle:
            edit_states = Editor::ObjectEditor::edit_triangle(_object);
            break;

        case GeometricObjectType::Cone:
            edit_states = Editor::ObjectEditor::edit_cone(_object);
            break;
        case GeometricObjectType::SolidCone:
            edit_states = Editor::ObjectEditor::edit_solid_cone(_object);
            break;

        case GeometricObjectType::GenericCylinder:
            edit_states = Editor::ObjectEditor::edit_generic_cylinder(_object);
            break;

        case GeometricObjectType::ThickAnnulus:
            edit_states = Editor::ObjectEditor::edit_thick_annulus(_object);
            break;

        case GeometricObjectType::PartCylinder:
            edit_states = Editor::ObjectEditor::edit_part_cylinder(_object);
            break;

        case GeometricObjectType::PartSphere:
            edit_states = Editor::ObjectEditor::edit_part_sphere(_object);
            break;

        case GeometricObjectType::PartTorus:
            edit_states = Editor::ObjectEditor::edit_part_torus(_object);
            break;

        case GeometricObjectType::Torus:
            edit_states = Editor::ObjectEditor::edit_torus(_object);
            break;

        case GeometricObjectType::Bowl:
            edit_states = Editor::ObjectEditor::edit_bowl(_object);
            break;

        case GeometricObjectType::Instance:
            edit_states = Editor::ObjectEditor::edit_instance(_object);
            break;

        case GeometricObjectType::BoundingVolumeHierarchy:
            edit_states = Editor::ObjectEditor::edit_bvh(_object);
            break;

        case GeometricObjectType::Container:
            edit_states = Editor::ObjectEditor::edit_container(_object);
            break;

        case GeometricObjectType::TransformContainer:
            edit_states = Editor::ObjectEditor::edit_transform_container(_object);
            break;

        default:
            std::cout << "Unimplemented object type in Scene objects switch: " << static_cast<uint32_t>(_object->get_type()) << ", named: " << geometric_type_name << std::endl;
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("Geometric object", tree_flags)) {

        // Visibility
        bool visible = _object->is_visible();
        if (ImGui::Checkbox("Visible", &visible))
            _object->set_visibility(visible);

        // Shadows
        bool casts_shadows = _object->casts_shadows();
        if (ImGui::Checkbox("Casts shadows", &casts_shadows)) {
            if (casts_shadows)
                _object->enable_shadows();
            else
                _object->disable_shadows();
        }

        bool has_bounding_box = _object->has_bounding_box();
        if (has_bounding_box) {
            ImGui::Text("Has bounding box");
            bool bounding_box_enabled = _object->bounding_box_enabled();
            if (ImGui::Checkbox("Bounding box enabled", &bounding_box_enabled)) {
                if (bounding_box_enabled)
                    _object->enable_bounding_box();
                else
                    _object->disable_bounding_box();
            }
        } else
            ImGui::Text("Doesn't have bounding box");

        if (ImGui::Button("Recalculate bounding box"))
            _object->recalculate_bounding_box();

        // Normal
        int normal_type = static_cast<int>(_object->get_normal_type());
        auto normal_types = std::array<const char*, 3> { "Flip", "Outwards", "Inwards" };
        bool normal_changed = ImGuiUtils::combo_box("Normal type", normal_types, normal_type);

        if (normal_changed) {
            switch (static_cast<NormalType>(normal_type)) {
            case NormalType::Flip:
                _object->set_normal_flip();
                break;
            case NormalType::Outwards:
                _object->set_normal_outwards();
                break;
            case NormalType::Inwards:
                _object->set_normal_inwards();
                break;
            default:
                break;
            }
            // Adds material editor
            if (_object->has_material())
                ImGuiRT::edit_material(_object->material);
        }
        ImGui::TreePop();
    }

    if (edit_states & ObjectEditor::EditState::BoundingBoxEdit) {
        scene_node->get_object()->recalculate_bounding_box();
        SceneNode::bounding_box_modified(scene_node);
    }

    if (edit_states & ObjectEditor::EditState::PropertyEdit) {
        // @todo re-render call
    }
}
