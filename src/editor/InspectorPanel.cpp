#include "InspectorPanel.hpp"
#include "../ImGuiRT.hpp"
#include <memory>

using namespace RT;

void Editor::InspectorPanel::_render_geometric_object_editor()
{

    GeometricObjectPtr _object = _scene_node->get_object();

    static const std::array<const char*, 22> geometric_types = {
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
        "Container"
    };
    bool modified = false;
    const char* geometric_type_name = geometric_types[static_cast<uint32_t>(_object->get_type())];
    if (ImGui::TreeNodeEx(geometric_type_name)) {
        switch (_object->get_type()) {
        case GeometricObjectType::Sphere: {
            auto sphere = std::dynamic_pointer_cast<GeometricObjects::Sphere>(_object);
            auto center = sphere->get_center();
            ImGuiUtils::input("Center", center, modified);
            sphere->set_center(center);
            double r = sphere->get_radius();
            ImGui::InputDouble("Radius", &r);
            sphere->set_radius(r);
        } break;

        case GeometricObjectType::Plane: {
            auto plane = std::dynamic_pointer_cast<GeometricObjects::Plane>(_object);
            auto origin = plane->get_origin();
            auto normal = plane->get_normal();
            ImGuiUtils::input("Origin", origin, modified);
            ImGuiUtils::input("Normal", normal, modified);
            plane->set_normal(normal);
            plane->set_origin(origin);
        } break;

        case GeometricObjectType::Box: {
            auto box = std::dynamic_pointer_cast<GeometricObjects::Box>(_object);

            Vec3 center = box->get_center();
            Vec3 min = box->get_min();
            Vec3 max = box->get_max();
            float half_dx = box->get_half_dx();
            float half_dy = box->get_half_dy();
            float half_dz = box->get_half_dz();
            if (ImGuiUtils::input("Center", center, modified))
                box->set_center(center);

            if (ImGuiUtils::input("Min", min, modified))
                box->set_min(min);

            if (ImGuiUtils::input("Max", max, modified))
                box->set_max(max);

            if (ImGuiUtils::input("Half Dx", half_dx, modified))
                box->set_half_dx(half_dx);

            if (ImGuiUtils::input("Half Dy", half_dy, modified))
                box->set_half_dy(half_dy);

            if (ImGuiUtils::input("Half Dz", half_dz, modified))
                box->set_half_dz(half_dz);
        } break;

        case GeometricObjectType::CompoundBox: {
            auto box = std::dynamic_pointer_cast<GeometricObjects::CompoundBox>(_object);

            Vec3 center = box->get_center();
            Vec3 min = box->get_min();
            Vec3 max = box->get_max();
            float half_dx = box->get_half_dx();
            float half_dy = box->get_half_dy();
            float half_dz = box->get_half_dz();
            if (ImGuiUtils::input("Center", center, modified))
                box->set_center(center);

            if (ImGuiUtils::input("Min", min, modified))
                box->set_min(min);

            if (ImGuiUtils::input("Max", max, modified))
                box->set_max(max);

            if (ImGuiUtils::input("Half Dx", half_dx, modified))
                box->set_half_dx(half_dx);

            if (ImGuiUtils::input("Half Dy", half_dy, modified))
                box->set_half_dy(half_dy);

            if (ImGuiUtils::input("Half Dz", half_dz, modified))
                box->set_half_dz(half_dz);
        } break;

        case GeometricObjectType::Capsule: {
            auto capsule = std::dynamic_pointer_cast<GeometricObjects::Capsule>(_object);
            float radius = capsule->get_radius();
            float height = capsule->get_height();
            ImGuiUtils::input("Radius", radius, modified);
            ImGuiUtils::input("Height", height, modified);
            capsule->set_radius(radius);
            capsule->set_height(height);
        } break;

        case GeometricObjectType::Disk: {
            auto disk = std::dynamic_pointer_cast<GeometricObjects::Disk>(_object);

            Vec3 center = disk->get_center();
            Vec3 normal = disk->get_normal();
            float radius = disk->get_radius();
            ImGuiUtils::input("Center", center, modified);
            ImGuiUtils::input("Normal", normal, modified);
            ImGui::InputFloat("Radius", &radius, modified);
            disk->set_center(center);
            disk->set_normal(normal);
            disk->set_radius(radius);
        } break;
        case GeometricObjectType::Annulus: {
            auto annulus = std::dynamic_pointer_cast<GeometricObjects::Annulus>(_object);

            Vec3 center = annulus->get_center();
            Vec3 normal = annulus->get_normal();
            float inner_radius = annulus->get_inner_radius();
            float outer_radius = annulus->get_outer_radius();
            ImGuiUtils::input("Center", center, modified);
            ImGuiUtils::input("Normal", normal, modified);
            ImGuiUtils::input("Inner radius", inner_radius, modified);
            ImGuiUtils::input("Outer radius", outer_radius, modified);
            annulus->set_center(center);
            annulus->set_normal(normal);
            annulus->set_inner_radius(inner_radius);
            annulus->set_outer_radius(outer_radius);
        } break;

        case GeometricObjectType::Rect: {
            auto rect = std::dynamic_pointer_cast<GeometricObjects::Rect>(_object);

            Vec3 p0 = rect->get_p0();
            Vec3 a = rect->get_a();
            Vec3 b = rect->get_b();

            ImGuiUtils::input("p0 - corner", p0, modified);
            ImGuiUtils::input("a", a, modified);
            ImGuiUtils::input("b", b, modified);
            rect->set_a(a);
            rect->set_b(b);
            rect->set_p0(p0);
        } break;

        case GeometricObjectType::Triangle: {
            auto rect = std::dynamic_pointer_cast<GeometricObjects::Triangle>(_object);

            Vec3 a = rect->get_a();
            Vec3 b = rect->get_b();
            Vec3 c = rect->get_c();

            ImGuiUtils::input("a", a, modified);
            ImGuiUtils::input("b", b, modified);
            ImGuiUtils::input("c", c, modified);
            rect->set_a(a);
            rect->set_b(b);
            rect->set_c(b);
        } break;

        case GeometricObjectType::Cone: {
            auto cone = std::dynamic_pointer_cast<GeometricObjects::Cone>(_object);

            float height = cone->get_height();
            float radius = cone->get_radius();
            ImGuiUtils::input("Height", height, modified);
            ImGuiUtils::input("Radius", radius, modified);
            cone->set_height(height);
            cone->set_radius(radius);
        } break;
        case GeometricObjectType::SolidCone: {
            auto cone = std::dynamic_pointer_cast<GeometricObjects::SolidCone>(_object);

            float height = cone->get_height();
            float radius = cone->get_radius();
            ImGuiUtils::input("Height", height, modified);
            ImGuiUtils::input("Radius", radius, modified);
            cone->set_height(height);
            cone->set_radius(radius);
        } break;

        case GeometricObjectType::GenericCylinder: {
            auto cylinder = std::dynamic_pointer_cast<GeometricObjects::GenericCylinder>(_object);
            float r = cylinder->get_radius();
            float y0 = cylinder->get_y0();
            float y1 = cylinder->get_y1();
            ImGui::InputFloat("Radius", &r);
            ImGui::InputFloat("Y0", &y0);
            ImGui::InputFloat("Y1", &y1);
            cylinder->set_radius(r);
            cylinder->set_y0(y0);
            cylinder->set_y1(y1);
        } break;

        case GeometricObjectType::ThickAnnulus: {
            auto thick_annulus = std::dynamic_pointer_cast<GeometricObjects::ThickAnnulus>(_object);
            float inner_radius = thick_annulus->get_inner_radius();
            float outer_radius = thick_annulus->get_outer_radius();
            ImGuiUtils::input("Inner radius", inner_radius, modified);
            ImGuiUtils::input("Outer radius", outer_radius, modified);
            thick_annulus->set_inner_radius(inner_radius);
            thick_annulus->set_outer_radius(outer_radius);
        } break;

        case GeometricObjectType::PartCylinder: {
            auto cylinder = std::dynamic_pointer_cast<GeometricObjects::PartCylinder>(_object);
            float r = cylinder->get_radius();
            float y0 = cylinder->get_y0();
            float y1 = cylinder->get_y1();
            float phi_min = cylinder->get_min_phi();
            float phi_max = cylinder->get_max_phi();
            ImGui::InputFloat("Radius", &r);
            ImGui::InputFloat("Y0", &y0);
            ImGui::InputFloat("Y1", &y1);
            ImGuiUtils::input_degrees("Min phi", phi_min, modified, 0.0f, phi_max);
            ImGuiUtils::input_degrees("Max phi", phi_max, modified, phi_min, Constants::PI_2);
            cylinder->set_radius(r);
            cylinder->set_y0(y0);
            cylinder->set_y1(y1);
            cylinder->set_min_phi(phi_min);
            cylinder->set_max_phi(phi_max);
        } break;

        case GeometricObjectType::PartSphere: {
            auto sphere = std::dynamic_pointer_cast<GeometricObjects::PartSphere>(_object);
            float phi_min = sphere->get_min_phi();
            float phi_max = sphere->get_max_phi();
            float theta_min = sphere->get_min_theta();
            float theta_max = sphere->get_max_theta();
            ImGuiUtils::input_degrees("Min phi", phi_min, modified, 0.0f, phi_max);
            ImGuiUtils::input_degrees("Max phi", phi_max, modified, phi_min, Constants::PI_2);
            ImGuiUtils::input_degrees("Min theta", theta_min, modified, 0.0f, theta_max);
            ImGuiUtils::input_degrees("Max theta", theta_max, modified, theta_min, Constants::PI);
            sphere->set_min_phi(phi_min);
            sphere->set_max_phi(phi_max);
            sphere->set_min_theta(theta_min);
            sphere->set_max_theta(theta_max);

        } break;

        case GeometricObjectType::PartTorus: {
            auto torus = std::dynamic_pointer_cast<GeometricObjects::PartTorus>(_object);
            float a = torus->get_a();
            float b = torus->get_b();
            float phi_min = torus->get_min_phi();
            float phi_max = torus->get_max_phi();
            float theta_min = torus->get_min_theta();
            float theta_max = torus->get_max_theta();

            ImGui::InputFloat("a", &a);
            ImGui::InputFloat("b", &b);
            ImGuiUtils::input_degrees("Min phi", phi_min, modified, 0.0f, phi_max);
            ImGuiUtils::input_degrees("Max phi", phi_max, modified, phi_min, Constants::PI_2);
            ImGuiUtils::input_degrees("Min theta", theta_min, modified, 0.0f, theta_max);
            ImGuiUtils::input_degrees("Max theta", theta_max, modified, theta_min, Constants::PI_2);

            torus->set_min_phi(phi_min);
            torus->set_max_phi(phi_max);
            torus->set_min_theta(theta_min);
            torus->set_max_theta(theta_max);
            torus->set_a(a);
            torus->set_b(b);

        } break;
        case GeometricObjectType::Torus: {
            auto torus = std::dynamic_pointer_cast<GeometricObjects::Torus>(_object);
            float a = torus->get_a();
            float b = torus->get_b();
            ImGui::InputFloat("a", &a);
            ImGui::InputFloat("b", &b);
            torus->set_a(a);
            torus->set_b(b);
        } break;

        case GeometricObjectType::Instance: {
            auto instance = std::dynamic_pointer_cast<Instance>(_object);
            Vec3 translation = instance->get_translation();
            Vec3 scale = instance->get_scale();
            Vec3 rotation = instance->get_rotation();
            bool transforms_the_texture = instance->get_transform_the_texture();
            if (ImGuiUtils::input("Translation", translation, modified)) {
                instance->set_translation(translation);
            }

            if (ImGuiUtils::input("Scale", scale, modified)) {
                instance->set_scale(scale);
            }

            if (ImGuiUtils::input_degrees("Rotation", rotation, modified)) {
                instance->set_rotation_x(rotation.x);
                instance->set_rotation_y(rotation.y);
                instance->set_rotation_z(rotation.z);
            }

            if (ImGui::Checkbox("Transform texture", &transforms_the_texture)) {
                instance->set_transform_the_texture(transforms_the_texture);
            }

        } break;

        case GeometricObjectType::BoundingVolumeHierarchy: {
            auto bvh = std::dynamic_pointer_cast<GeometricObjects::BVH>(_object);

            if (bvh->is_built())
                ImGui::Text("Tree built");
            else
                ImGui::Text("Tree not built");

            if (ImGui::Button("Build"))
                bvh->build_tree();

        } break;

        default:
            std::cout << "Unimplemented object type in Scene objects switch" << std::endl;
        }

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

        // Normal
        int normal_type = static_cast<int>(_object->get_normal_type());
        auto normal_types = std::array<const char*, 3> { "Flip", "Outwards", "Inwards" };
        bool normal_changed = ImGuiUtils::combo_box("Normal type", normal_types, normal_type, modified);

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
        }
        // Adds material editor
        if (_object->has_material())
            ImGuiRT::edit_material(_object->material);

        ImGui::TreePop();
    }
}
