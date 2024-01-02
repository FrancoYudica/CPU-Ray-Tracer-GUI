#include "GeometricObjectEditors.hpp"

uint8_t Editor::ObjectEditor::edit_sphere(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto sphere = std::dynamic_pointer_cast<RT::GeometricObjects::Sphere>(object);

    auto center = sphere->get_center();
    if (ImGuiUtils::input("Center", center)) {
        sphere->set_center(center);
        state |= BoundingBoxEdit;
    }
    double r = sphere->get_radius();
    if (ImGui::InputDouble("Radius", &r)) {
        sphere->set_radius(r);
        state |= BoundingBoxEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_plane(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;

    auto plane = std::dynamic_pointer_cast<GeometricObjects::Plane>(object);
    auto origin = plane->get_origin();
    auto normal = plane->get_normal();
    if (ImGuiUtils::input("Origin", origin)) {
        plane->set_origin(origin);
        state |= PropertyEdit;
    }
    if (ImGuiUtils::input_normal("Normal", normal)) {
        plane->set_normal(normal);
        state |= PropertyEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_box(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;

    auto box = std::dynamic_pointer_cast<GeometricObjects::Box>(object);

    Vec3 center = box->get_center();
    Vec3 min = box->get_min();
    Vec3 max = box->get_max();
    float half_dx = box->get_half_dx();
    float half_dy = box->get_half_dy();
    float half_dz = box->get_half_dz();
    if (ImGuiUtils::input("Center", center)) {
        box->set_center(center);
        state |= BoundingBoxEdit;
    }

    if (ImGuiUtils::input("Min", min)) {
        box->set_min(min);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Max", max)) {
        box->set_max(max);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Half Dx", half_dx)) {
        box->set_half_dx(half_dx);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Half Dy", half_dy)) {
        box->set_half_dy(half_dy);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Half Dz", half_dz)) {
        box->set_half_dz(half_dz);
        state |= BoundingBoxEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_compound_box(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;

    auto box = std::dynamic_pointer_cast<GeometricObjects::CompoundBox>(object);

    Vec3 center = box->get_center();
    Vec3 min = box->get_min();
    Vec3 max = box->get_max();
    float half_dx = box->get_half_dx();
    float half_dy = box->get_half_dy();
    float half_dz = box->get_half_dz();
    if (ImGuiUtils::input("Center", center)) {
        box->set_center(center);
        state |= BoundingBoxEdit;
    }

    if (ImGuiUtils::input("Min", min)) {
        box->set_min(min);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Max", max)) {
        box->set_max(max);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Half Dx", half_dx)) {
        box->set_half_dx(half_dx);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Half Dy", half_dy)) {
        box->set_half_dy(half_dy);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Half Dz", half_dz)) {
        box->set_half_dz(half_dz);
        state |= BoundingBoxEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_capsule(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;

    auto capsule = std::dynamic_pointer_cast<GeometricObjects::Capsule>(object);
    float radius = capsule->get_radius();
    float height = capsule->get_height();
    if (ImGuiUtils::input("Radius", radius)) {
        capsule->set_radius(radius);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Height", height)) {
        capsule->set_height(height);
        state |= BoundingBoxEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_disk(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;

    auto disk = std::dynamic_pointer_cast<GeometricObjects::Disk>(object);

    Vec3 center = disk->get_center();
    Vec3 normal = disk->get_normal();
    float radius = disk->get_radius();
    if (ImGuiUtils::input("Center", center)) {
        disk->set_center(center);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input_normal("Normal", normal)) {
        disk->set_normal(normal);
        state |= BoundingBoxEdit;
    }
    if (ImGui::InputFloat("Radius", &radius)) {
        disk->set_radius(radius);
        state |= BoundingBoxEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_annulus(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;

    auto annulus = std::dynamic_pointer_cast<GeometricObjects::Annulus>(object);

    Vec3 center = annulus->get_center();
    Vec3 normal = annulus->get_normal();
    float inner_radius = annulus->get_inner_radius();
    float outer_radius = annulus->get_outer_radius();
    if (ImGuiUtils::input("Center", center)) {
        annulus->set_center(center);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input_normal("Normal", normal)) {
        annulus->set_normal(normal);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Inner radius", inner_radius)) {
        annulus->set_inner_radius(inner_radius);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Outer radius", outer_radius)) {
        annulus->set_outer_radius(outer_radius);
        state |= BoundingBoxEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_rect(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;

    auto rect = std::dynamic_pointer_cast<GeometricObjects::Rect>(object);

    Vec3 p0 = rect->get_p0();
    Vec3 a = rect->get_a();
    Vec3 b = rect->get_b();

    if (ImGuiUtils::input("p0 - corner", p0)) {
        rect->set_p0(p0);
        state |= BoundingBoxEdit;
    }

    if (ImGuiUtils::input("a", a)) {
        rect->set_a(a);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("b", b)) {
        rect->set_b(b);
        state |= BoundingBoxEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_triangle(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;

    auto triangle = std::dynamic_pointer_cast<GeometricObjects::Triangle>(object);

    Vec3 a = triangle->get_a();
    Vec3 b = triangle->get_b();
    Vec3 c = triangle->get_c();

    if (ImGuiUtils::input("a", a)) {

        triangle->set_a(a);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("b", b)) {

        triangle->set_b(b);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("c", c)) {

        triangle->set_c(c);
        state |= BoundingBoxEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_cone(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto cone = std::dynamic_pointer_cast<GeometricObjects::Cone>(object);

    float height = cone->get_height();
    float radius = cone->get_radius();
    if (ImGuiUtils::input("Height", height)) {
        cone->set_height(height);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Radius", radius)) {
        cone->set_radius(radius);
        state |= BoundingBoxEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_solid_cone(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto cone = std::dynamic_pointer_cast<GeometricObjects::SolidCone>(object);

    float height = cone->get_height();
    float radius = cone->get_radius();
    if (ImGuiUtils::input("Height", height)) {
        cone->set_height(height);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Radius", radius)) {
        cone->set_radius(radius);
        state |= BoundingBoxEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_generic_cylinder(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto cylinder = std::dynamic_pointer_cast<GeometricObjects::GenericCylinder>(object);
    float r = cylinder->get_radius();
    float y0 = cylinder->get_y0();
    float y1 = cylinder->get_y1();
    if (ImGui::InputFloat("Radius", &r)) {
        cylinder->set_radius(r);
        state |= BoundingBoxEdit;
    }
    if (ImGui::InputFloat("Y0", &y0)) {
        cylinder->set_y0(y0);
        state |= BoundingBoxEdit;
    }
    if (ImGui::InputFloat("Y1", &y1)) {
        cylinder->set_y1(y1);
        state |= BoundingBoxEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_thick_annulus(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto thick_annulus = std::dynamic_pointer_cast<GeometricObjects::ThickAnnulus>(object);
    float inner_radius = thick_annulus->get_inner_radius();
    float outer_radius = thick_annulus->get_outer_radius();
    if (ImGuiUtils::input("Inner radius", inner_radius)) {

        thick_annulus->set_inner_radius(inner_radius);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Outer radius", outer_radius)) {
        thick_annulus->set_outer_radius(outer_radius);
        state |= BoundingBoxEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_part_cylinder(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto cylinder = std::dynamic_pointer_cast<GeometricObjects::PartCylinder>(object);
    float r = cylinder->get_radius();
    float y0 = cylinder->get_y0();
    float y1 = cylinder->get_y1();
    float phi_min = cylinder->get_min_phi();
    float phi_max = cylinder->get_max_phi();
    if (ImGui::InputFloat("Radius", &r)) {
        cylinder->set_radius(r);
        state |= BoundingBoxEdit;
    }
    if (ImGui::InputFloat("Y0", &y0)) {

        cylinder->set_y0(y0);
        state |= BoundingBoxEdit;
    }
    if (ImGui::InputFloat("Y1", &y1)) {
        cylinder->set_y1(y1);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input_degrees("Min phi", phi_min, 0.0f, phi_max)) {

        cylinder->set_min_phi(phi_min);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input_degrees("Max phi", phi_max, phi_min, Constants::PI_2)) {
        cylinder->set_max_phi(phi_max);
        state |= BoundingBoxEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_part_sphere(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto sphere = std::dynamic_pointer_cast<GeometricObjects::PartSphere>(object);
    float phi_min = sphere->get_min_phi();
    float phi_max = sphere->get_max_phi();
    float theta_min = sphere->get_min_theta();
    float theta_max = sphere->get_max_theta();
    if (ImGuiUtils::input_degrees("Min phi", phi_min, 0.0f, phi_max)) {
        sphere->set_min_phi(phi_min);
        state |= PropertyEdit;
    }
    if (ImGuiUtils::input_degrees("Max phi", phi_max, phi_min, Constants::PI_2)) {
        sphere->set_max_phi(phi_max);
        state |= PropertyEdit;
    }
    if (ImGuiUtils::input_degrees("Min theta", theta_min, 0.0f, theta_max)) {
        sphere->set_min_theta(theta_min);
        state |= PropertyEdit;
    }
    if (ImGuiUtils::input_degrees("Max theta", theta_max, theta_min, Constants::PI)) {
        sphere->set_max_theta(theta_max);
        state |= PropertyEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_part_torus(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto torus = std::dynamic_pointer_cast<GeometricObjects::PartTorus>(object);
    float a = torus->get_a();
    float b = torus->get_b();
    float phi_min = torus->get_min_phi();
    float phi_max = torus->get_max_phi();
    float theta_min = torus->get_min_theta();
    float theta_max = torus->get_max_theta();

    if (ImGuiUtils::input("a", a)) {
        torus->set_a(a);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("b", b)) {
        torus->set_b(b);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input_degrees("Min phi", phi_min, 0.0f, phi_max)) {
        torus->set_min_phi(phi_min);
        state |= PropertyEdit;
    }
    if (ImGuiUtils::input_degrees("Max phi", phi_max, phi_min, Constants::PI_2)) {
        torus->set_max_phi(phi_max);
        state |= PropertyEdit;
    }
    if (ImGuiUtils::input_degrees("Min theta", theta_min, 0.0f, theta_max)) {
        torus->set_min_theta(theta_min);
        state |= PropertyEdit;
    }
    if (ImGuiUtils::input_degrees("Max theta", theta_max, theta_min, Constants::PI_2)) {
        torus->set_max_theta(theta_max);
        state |= PropertyEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_torus(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto torus = std::dynamic_pointer_cast<GeometricObjects::Torus>(object);
    float a = torus->get_a();
    float b = torus->get_b();

    if (ImGuiUtils::input("a", a)) {
        torus->set_a(a);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("b", b)) {
        torus->set_b(b);
        state |= BoundingBoxEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_instance(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto instance = std::dynamic_pointer_cast<Instance>(object);
    Vec3 translation = instance->get_translation();
    Vec3 scale = instance->get_scale();
    Vec3 rotation = instance->get_rotation();
    bool transforms_the_texture = instance->get_transform_the_texture();
    if (ImGuiUtils::input("Translation", translation)) {
        instance->set_translation(translation);
        state |= BoundingBoxEdit;
    }

    if (ImGuiUtils::input("Scale", scale, 0.25F, 1.0F)) {
        instance->set_scale(scale);
        state |= BoundingBoxEdit;
    }

    if (ImGuiUtils::input_degrees("Rotation", rotation)) {
        instance->set_rotation_x(rotation.x);
        instance->set_rotation_y(rotation.y);
        instance->set_rotation_z(rotation.z);
        state |= BoundingBoxEdit;
    }

    if (ImGui::Checkbox("Transform texture", &transforms_the_texture)) {
        instance->set_transform_the_texture(transforms_the_texture);
        state |= PropertyEdit;
    }

    return state;
}

uint8_t Editor::ObjectEditor::edit_bvh(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto bvh = std::dynamic_pointer_cast<GeometricObjects::BVH>(object);

    if (bvh->is_built())
        ImGui::Text("Tree built");
    else
        ImGui::Text("Tree not built");

    return state;
}

uint8_t Editor::ObjectEditor::edit_container(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    ImGui::Text("Simple container");
    return state;
}

uint8_t Editor::ObjectEditor::edit_transform_container(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto transform = std::dynamic_pointer_cast<GeometricObjects::TransformContainer>(object);
    Vec3 translation = transform->get_translation();
    Vec3 scale = transform->get_scale();
    Vec3 rotation = transform->get_rotation();
    bool transforms_the_texture = transform->get_transform_the_texture();
    if (ImGuiUtils::input("Translation", translation)) {
        transform->set_translation(translation);
        state |= BoundingBoxEdit;
    }

    if (ImGuiUtils::input("Scale", scale, 0.25F, 1.0F)) {
        transform->set_scale(scale);
        state |= BoundingBoxEdit;
    }

    if (ImGuiUtils::input_degrees("Rotation", rotation)) {
        transform->set_rotation_x(rotation.x);
        transform->set_rotation_y(rotation.y);
        transform->set_rotation_z(rotation.z);
        state |= BoundingBoxEdit;
    }

    if (ImGui::Checkbox("Transform texture", &transforms_the_texture)) {
        transform->set_transform_the_texture(transforms_the_texture);
        state |= PropertyEdit;
    }
    return state;
}

uint8_t Editor::ObjectEditor::edit_bowl(RT::GeometricObjectPtr& object)
{
    uint8_t state = EditState::None;
    auto bowl = std::dynamic_pointer_cast<GeometricObjects::Bowl>(object);
    float inner_radius = bowl->get_inner_radius();
    float outer_radius = bowl->get_outer_radius();

    if (ImGuiUtils::input("Inner radius", inner_radius)) {
        bowl->set_inner_radius(inner_radius);
        state |= BoundingBoxEdit;
    }
    if (ImGuiUtils::input("Outer radius", outer_radius)) {
        bowl->set_outer_radius(outer_radius);
        state |= BoundingBoxEdit;
    }
    return state;
}
