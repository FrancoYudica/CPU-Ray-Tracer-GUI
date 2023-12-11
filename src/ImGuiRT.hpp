#ifndef __IMGUI_RT_UTILS__
#define __IMGUI_RT_UTILS__
#include "ImGuiUtils.hpp"
#include <memory>

#include "CPU-Ray-Tracing/CPURayTracer.hpp"

using namespace RT;

namespace ImGuiRT {

static bool camera_edit(std::shared_ptr<Camera>& camera, World& world)
{
    bool modified = false;
    int current_item = static_cast<int>(camera->camera_type);

    // Changes camera type
    ImGuiUtils::combo_box<7>("Camera",
        { "Orthographic", "Perspective - Pinhole", "Thin lens", "Fish eye",
            "Spherical panoramic", "Stereo Dual", "Stereo Anaglyph" },
        current_item,
        modified);

    if (modified) {
        CameraType selected_type = static_cast<CameraType>(current_item);

        if (selected_type != camera->camera_type) {
            std::shared_ptr<Camera> new_camera;
            switch (selected_type) {
            case CameraType::Orthographic:
                new_camera = std::make_shared<Cameras::OrthographicCamera>();
                break;
            case CameraType::Pinhole:
                new_camera = std::make_shared<Cameras::PinholeCamera>();
                break;
            case CameraType::ThinLens:
                new_camera = std::make_shared<Cameras::ThinLensCamera>();
                break;
            case CameraType::FishEye:
                new_camera = std::make_shared<Cameras::FishEyeCamera>();
                break;
            case CameraType::SphericalPanoramic:
                new_camera = std::make_shared<Cameras::SphericalPanoramicCamera>();
                break;
            case CameraType::StereoDual:
            case CameraType::StereoAnaglyph: {
                std::shared_ptr<Cameras::StereoCamera> stereo_camera;
                if (selected_type == CameraType::StereoDual)
                    stereo_camera = std::make_shared<Cameras::StereoDualCamera>();
                else
                    stereo_camera = std::make_shared<Cameras::AnaglyphCamera>();

                // Copies fields here, because calling setup_cameras requires uvw base
                stereo_camera->set_eye(camera->eye);
                stereo_camera->set_look_at(camera->look_at);
                stereo_camera->set_up(camera->up);

                // Sets default left and right eye cameras as Pinhole
                auto pinhole_left = std::make_shared<Cameras::PinholeCamera>();
                pinhole_left->exposure_time = camera->exposure_time;
                stereo_camera->set_left_camera(pinhole_left);
                auto pinhole_right = std::make_shared<Cameras::PinholeCamera>();
                pinhole_right->exposure_time = camera->exposure_time;
                stereo_camera->set_right_camera(pinhole_right);
                new_camera = stereo_camera;
            } break;
            }

            new_camera->set_eye(camera->eye);
            new_camera->set_look_at(camera->look_at);
            new_camera->set_up(camera->up);
            new_camera->exposure_time = camera->exposure_time;
            new_camera->setup_camera();
            camera = new_camera;
        }
    }

    // Camera specific type settings
    switch (camera->get_camera_type()) {
    case CameraType::Orthographic: {
        auto ortho = std::dynamic_pointer_cast<Cameras::OrthographicCamera>(camera);

        // Zoom
        float zoom = ortho->zoom;
        if (ImGui::SliderFloat("Zoom", &zoom, 0.01f, 5.0f))
            ortho->set_zoom(zoom);

        // View plane distance
        float d = ortho->d;
        if (ImGuiUtils::input("View plane distance", d, modified, 0.01f, 2000.0f))
            ortho->set_view_distance(d);

        // Field of view
        {
            float theta = atanf(world.view_plane.v_res * 0.5f / ortho->d);
            float theta_degrees = theta * 180.0f / Constants::PI;
            if (ImGui::SliderFloat("FOV", &theta_degrees, 0.01f, 89.0f)) {
                float new_theta_rad = theta_degrees / 180.0f * Constants::PI;
                float new_view_distance = world.view_plane.v_res * 0.5f / tan(new_theta_rad);
                ortho->set_view_distance(new_view_distance);
            }
        }
    } break;
    case CameraType::Pinhole: {
        auto pinhole = std::dynamic_pointer_cast<Cameras::PinholeCamera>(camera);

        // Zoom
        float zoom = pinhole->zoom;
        if (ImGui::SliderFloat("Zoom", &zoom, 0.01f, 5.0f))
            pinhole->set_zoom(zoom);

        // View plane distance
        float d = pinhole->d;
        if (ImGuiUtils::input("View plane distance", d, modified, 0.01f, 2000.0f))
            pinhole->set_view_distance(d);

        // Field of view
        {
            float theta = atanf(world.view_plane.v_res * 0.5f / pinhole->d);
            float theta_degrees = theta * 180.0f / Constants::PI;
            if (ImGui::SliderFloat("FOV", &theta_degrees, 0.01f, 89.0f)) {
                float new_theta_rad = theta_degrees / 180.0f * Constants::PI;
                float new_view_distance = world.view_plane.v_res * 0.5f / tan(new_theta_rad);
                pinhole->set_view_distance(new_view_distance);
            }
        }
    } break;
    case CameraType::ThinLens: {
        auto thin_lens = std::dynamic_pointer_cast<Cameras::ThinLensCamera>(camera);

        // Zoom
        float zoom = thin_lens->zoom;
        if (ImGui::SliderFloat("Zoom", &zoom, 0.01f, 5.0f))
            thin_lens->set_zoom(zoom);

        // View plane distance
        float d = thin_lens->d;
        if (ImGui::SliderFloat("View plane distance", &d, 0.01f, 1000.0f))
            thin_lens->set_view_distance(d);

        // Focal distance
        float focal_d = thin_lens->focal_distance;
        if (ImGui::SliderFloat("Focal distance", &focal_d, 0.01f, 1000.0f))
            thin_lens->set_focal_distance(focal_d);

        // Lens radius
        float lens_r = thin_lens->lens_radius;
        if (ImGui::SliderFloat("Lens radius", &lens_r, 0.01f, 10.0f))
            thin_lens->set_lens_radius(lens_r);

        // Field of view
        {
            float theta = atanf(world.view_plane.v_res * 0.5f / thin_lens->d);
            float theta_degrees = theta * 180.0f / Constants::PI;
            if (ImGui::SliderFloat("FOV", &theta_degrees, 0.01f, 89.0f)) {
                float new_theta_rad = theta_degrees / 180.0f * Constants::PI;
                float new_view_distance = world.view_plane.v_res * 0.5f / tan(new_theta_rad);
                thin_lens->set_view_distance(new_view_distance);
            }
        }
    } break;
    case CameraType::FishEye: {
        auto fish_eye = std::dynamic_pointer_cast<Cameras::FishEyeCamera>(camera);

        // Field of view
        {
            float psi_degrees = fish_eye->psi_max * 180.0f / Constants::PI;
            if (ImGui::SliderFloat("Max psi", &psi_degrees, 0.01f, 180.0f)) {
                float new_psi = psi_degrees / 180.0f * Constants::PI;
                fish_eye->set_psi_max(new_psi);
            }
        }
    } break;
    case CameraType::SphericalPanoramic: {
        auto spherical_panoramic = std::dynamic_pointer_cast<Cameras::SphericalPanoramicCamera>(camera);

        // Field of view
        {
            float psi_degrees = spherical_panoramic->psi_max * 180.0f / Constants::PI;
            if (ImGui::SliderFloat("Max psi", &psi_degrees, 0.01f, 90.0f)) {
                float new_spi = psi_degrees / 180.0f * Constants::PI;
                spherical_panoramic->set_psi_max(new_spi);
            }

            float lambda_degrees = spherical_panoramic->lambda_max * 180.0f / Constants::PI;
            if (ImGui::SliderFloat("Max lambda", &lambda_degrees, 0.01f, 180.0f)) {
                float new_lambda = lambda_degrees / 180.0f * Constants::PI;
                spherical_panoramic->set_lambda_max(new_lambda);
            }

            if (ImGui::Button("Fix ratio")) {
                // Viewport aspect ratio should be the same as lambda / psi
                float aspect_ratio = spherical_panoramic->lambda_max / spherical_panoramic->psi_max;
                float new_width = world.view_plane.v_res * aspect_ratio;
                world.view_plane.set_hres(new_width);
            }
        }
    } break;
    case CameraType::StereoDual:
    case CameraType::StereoAnaglyph: {
        auto stereo_camera = std::dynamic_pointer_cast<Cameras::StereoCamera>(camera);

        // Viewing type
        {
            int current_item = static_cast<int>(static_cast<int>(stereo_camera->viewing_type));

            bool modified_viewing_type = ImGuiUtils::combo_box<2>(
                "Viewing type",
                std::array<const char*, 2> { "Parallel", "Transverse" },
                current_item,
                modified);

            if (modified_viewing_type)
                stereo_camera->set_viewing(static_cast<Cameras::StereoViewingType>(current_item));
        }

        // Stereo angle
        float stereo_angle = Constants::OVER_180_PI * stereo_camera->beta;
        if (ImGuiUtils::input("Stereo angle", stereo_angle, modified))
            stereo_camera->set_stereo_angle_degrees(stereo_angle);

        if (camera->get_camera_type() == CameraType::StereoAnaglyph) {
            auto anaglyph_camera = std::dynamic_pointer_cast<Cameras::AnaglyphCamera>(camera);
            ImGuiUtils::color_edit("Left color", anaglyph_camera->left_color, modified);
        } else if (camera->get_camera_type() == CameraType::StereoDual) {
            auto stereo_dual_camera = std::dynamic_pointer_cast<Cameras::StereoDualCamera>(camera);
            // Pixel gap
            int pixel_gap = stereo_dual_camera->pixel_gap;
            if (ImGui::InputInt("Pixel gap", &pixel_gap))
                stereo_dual_camera->set_pixel_gap(pixel_gap);
        }
    } break;
    }

    {
        // Roll angle
        float roll = camera->get_roll() * 180.0f / Constants::PI;
        if (ImGui::SliderFloat("Roll", &roll, 0.0f, 360.0f)) {
            camera->set_roll(roll / 180.0f * Constants::PI);
        }
    }

    return modified;
}

/// @brief Combo box used for changing BRDF type
static bool brdf_combo_box(const char* label, std::shared_ptr<BRDF>& brdf)
{
    static const std::array<const char*, 3> brdf_types = { "BRDF: Lambertian", "BRDF: Glossy Specular - Phong",
        "BRDF: Glossy Specular - Blinn Phong" };

    int current_brdf_index = static_cast<int>(brdf->get_type());
    bool modified = false;
    bool changed = ImGuiUtils::combo_box(label, brdf_types, current_brdf_index, modified);

    if (!changed)
        return false;

    BRDFType selected = static_cast<BRDFType>(current_brdf_index);
    switch (selected) {
    case BRDFType::Lambertian:
        brdf = std::make_shared<BRDFS::Lambertian>();
        break;
    case BRDFType::GlossySpecularPhong:
        brdf = std::make_shared<BRDFS::GlossySpecularPhong>();
        break;
    case BRDFType::GlossySpecularBlinnPhong:
        brdf = std::make_shared<BRDFS::GlossySpecularBlinnPhong>();
        break;

    default:
        std::cout << "[WARNING] Unimplemented BRDFType: <" << brdf_types[current_brdf_index]
                  << "> in 'ImGuiRT::brdf_combo_box'" << std::endl;
    }

    return modified;
}

/// @brief Combo box used for changing material type
static bool material_combo_box(const char* label, std::shared_ptr<Material>& material)
{
    static const std::array<const char*, 4> material_types = { "Material: Matte", "Material: Phong",
        "Material: Plastic", "Material: Emissive" };

    int current_material_index = static_cast<int>(material->get_type());
    bool modified = false;
    ImGuiUtils::combo_box(label, material_types, current_material_index, modified);

    if (!modified)
        return false;

    MaterialType selected = static_cast<MaterialType>(current_material_index);
    switch (selected) {
    case MaterialType::Matte:
        material = std::make_shared<Materials::Matte>();
        break;
    case MaterialType::Phong:
        material = std::make_shared<Materials::Phong>();
        break;
    case MaterialType::Plastic:
        material = std::make_shared<Materials::Plastic>();
        break;
    case MaterialType::Emissive:
        material = std::make_shared<Materials::Emissive>();
        break;
    default:
        std::cout << "[WARNING] Unimplemented MaterialType: <" << material_types[current_material_index]
                  << "> in 'ImGuiRT::material_combo_box'" << std::endl;
    }
    return modified;
}

/// @brief UI that allows BRDF properties editing
/// @param brdf
static bool edit_brdf(const char* label, std::shared_ptr<BRDF> brdf)
{
    static const std::array<const char*, 3> brdf_types = { "BRDF: Lambertian", "BRDF: Glossy Specular - Phong",
        "BRDF: Glossy Specular - Blinn Phong" };
    bool modified = false;
    if (ImGui::TreeNodeEx(label)) {
        const char* brdf_type_name = brdf_types[static_cast<uint32_t>(brdf->get_type())];
        ImGui::Text("%s", brdf_type_name);
        switch (brdf->get_type()) {
        case BRDFType::Lambertian:

        {
            auto lambertian = std::dynamic_pointer_cast<BRDFS::Lambertian>(brdf);
            ImGuiUtils::color_edit("Color", lambertian->cd, modified);
            ImGui::InputFloat("Coefficient", &lambertian->kd, 0.01f, 0.01f);
            lambertian->kd = glm::clamp(lambertian->kd, 0.0f, 1.0f);
        } break;
        case BRDFType::GlossySpecularPhong: {
            auto glossy_specular = std::dynamic_pointer_cast<BRDFS::GlossySpecularPhong>(brdf);
            ImGuiUtils::color_edit("Color", glossy_specular->cs, modified);
            ImGui::InputFloat("Coefficient", &glossy_specular->ks, 0.01f, 0.01f);
            glossy_specular->ks = glm::clamp(glossy_specular->ks, 0.0f, 1.0f);
            ImGui::InputFloat("Specular exponent", &glossy_specular->e, 0.01f, 0.01f);
        } break;

        case BRDFType::GlossySpecularBlinnPhong: {
            auto glossy_specular = std::dynamic_pointer_cast<BRDFS::GlossySpecularBlinnPhong>(brdf);
            ImGuiUtils::color_edit("Color", glossy_specular->cs, modified);
            ImGui::InputFloat("Coefficient", &glossy_specular->ks, 0.01f, 0.01f);
            glossy_specular->ks = glm::clamp(glossy_specular->ks, 0.0f, 1.0f);
            ImGui::InputFloat("Specular exponent", &glossy_specular->e, 0.01f, 0.01f);
        } break;
        }
        ImGui::TreePop();
    }
    return modified;
}

/// @brief UI that allows material properties editing
/// also, modifying the material type
/// @param material Material ptr by reference, it can be reassigned
static bool edit_material(std::shared_ptr<Material>& material)
{
    bool modified = false;
    static const std::array<const char*, 4> material_types = { "Matte material", "Phong material", "Plastic material", "Emissive" };
    const char* material_type_name = material_types[static_cast<uint32_t>(material->get_type())];
    if (ImGui::TreeNodeEx(material_type_name)) {
        material_combo_box("Material", material);
        switch (material->get_type()) {
        case MaterialType::Matte: {
            auto matte = std::dynamic_pointer_cast<Materials::Matte>(material);
            edit_brdf("Ambient", matte->ambient_brdf);
            edit_brdf("Diffuse", matte->diffuse_brdf);
        } break;
        case MaterialType::Phong: {
            auto phong = std::dynamic_pointer_cast<Materials::Phong>(material);
            edit_brdf("Ambient", phong->ambient_brdf);
            edit_brdf("Diffuse", phong->diffuse_brdf);
            edit_brdf("Specular", phong->specular_brdf);
        } break;

        case MaterialType::Plastic: {
            auto plastic = std::dynamic_pointer_cast<Materials::Plastic>(material);
            brdf_combo_box("Ambient BRDF: ", plastic->ambient_brdf);
            brdf_combo_box("Diffuse BRDF: ", plastic->diffuse_brdf);
            brdf_combo_box("Specular BRDF: ", plastic->specular_brdf);
            edit_brdf("Ambient", plastic->ambient_brdf);
            edit_brdf("Diffuse", plastic->diffuse_brdf);
            edit_brdf("Specular", plastic->specular_brdf);
        } break;
        case MaterialType::Emissive: {
            auto emissive = std::dynamic_pointer_cast<Materials::Emissive>(material);
            ImGuiUtils::color_edit("Emission color", emissive->color, modified);
            ImGui::InputFloat("Scale radiance", &emissive->ls);
        } break;
        }
        ImGui::TreePop();
    }
    return modified;
}

static bool edit_container(const std::shared_ptr<GeometricObjects::Container>& container);

static bool edit_geometric_object(GeometricObjectPtr& object)
{
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
    const char* geometric_type_name = geometric_types[static_cast<uint32_t>(object->get_type())];
    if (ImGui::TreeNodeEx(geometric_type_name)) {
        switch (object->get_type()) {
        case GeometricObjectType::Sphere: {
            auto sphere = std::dynamic_pointer_cast<GeometricObjects::Sphere>(object);
            auto center = sphere->get_center();
            ImGuiUtils::input("Center", center, modified);
            sphere->set_center(center);
            double r = sphere->get_radius();
            ImGui::InputDouble("Radius", &r);
            sphere->set_radius(r);
        } break;

        case GeometricObjectType::Plane: {
            auto plane = std::dynamic_pointer_cast<GeometricObjects::Plane>(object);
            auto origin = plane->get_origin();
            auto normal = plane->get_normal();
            ImGuiUtils::input("Origin", origin, modified);
            ImGuiUtils::input("Normal", normal, modified);
            plane->set_normal(normal);
            plane->set_origin(origin);
        } break;

        case GeometricObjectType::Box: {
            auto box = std::dynamic_pointer_cast<GeometricObjects::Box>(object);

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
            auto box = std::dynamic_pointer_cast<GeometricObjects::CompoundBox>(object);

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
            auto capsule = std::dynamic_pointer_cast<GeometricObjects::Capsule>(object);
            float radius = capsule->get_radius();
            float height = capsule->get_height();
            ImGuiUtils::input("Radius", radius, modified);
            ImGuiUtils::input("Height", height, modified);
            capsule->set_radius(radius);
            capsule->set_height(height);
        } break;

        case GeometricObjectType::Disk: {
            auto disk = std::dynamic_pointer_cast<GeometricObjects::Disk>(object);

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
            auto annulus = std::dynamic_pointer_cast<GeometricObjects::Annulus>(object);

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
            auto rect = std::dynamic_pointer_cast<GeometricObjects::Rect>(object);

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
            auto rect = std::dynamic_pointer_cast<GeometricObjects::Triangle>(object);

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
            auto cone = std::dynamic_pointer_cast<GeometricObjects::Cone>(object);

            float height = cone->get_height();
            float radius = cone->get_radius();
            ImGuiUtils::input("Height", height, modified);
            ImGuiUtils::input("Radius", radius, modified);
            cone->set_height(height);
            cone->set_radius(radius);
        } break;
        case GeometricObjectType::SolidCone: {
            auto cone = std::dynamic_pointer_cast<GeometricObjects::SolidCone>(object);

            float height = cone->get_height();
            float radius = cone->get_radius();
            ImGuiUtils::input("Height", height, modified);
            ImGuiUtils::input("Radius", radius, modified);
            cone->set_height(height);
            cone->set_radius(radius);
        } break;

        case GeometricObjectType::GenericCylinder: {
            auto cylinder = std::dynamic_pointer_cast<GeometricObjects::GenericCylinder>(object);
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
            auto thick_annulus = std::dynamic_pointer_cast<GeometricObjects::ThickAnnulus>(object);
            float inner_radius = thick_annulus->get_inner_radius();
            float outer_radius = thick_annulus->get_outer_radius();
            ImGuiUtils::input("Inner radius", inner_radius, modified);
            ImGuiUtils::input("Outer radius", outer_radius, modified);
            thick_annulus->set_inner_radius(inner_radius);
            thick_annulus->set_outer_radius(outer_radius);
        } break;

        case GeometricObjectType::PartCylinder: {
            auto cylinder = std::dynamic_pointer_cast<GeometricObjects::PartCylinder>(object);
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
            auto sphere = std::dynamic_pointer_cast<GeometricObjects::PartSphere>(object);
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
            auto torus = std::dynamic_pointer_cast<GeometricObjects::PartTorus>(object);
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
            auto torus = std::dynamic_pointer_cast<GeometricObjects::Torus>(object);
            float a = torus->get_a();
            float b = torus->get_b();
            ImGui::InputFloat("a", &a);
            ImGui::InputFloat("b", &b);
            torus->set_a(a);
            torus->set_b(b);
        } break;

        case GeometricObjectType::Instance: {
            auto instance = std::dynamic_pointer_cast<Instance>(object);
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
            auto bvh = std::dynamic_pointer_cast<GeometricObjects::BVH>(object);

            if (bvh->is_built())
                ImGui::Text("Tree built");
            else
                ImGui::Text("Tree not built");

            if (ImGui::Button("Build"))
                bvh->build_tree();

            bool modified_child = edit_container(bvh);
            if (modified_child)
                bvh->set_built_state(false);
        } break;

        default:
            std::cout << "Unimplemented object type in Scene objects switch" << std::endl;
        }

        // Visibility
        bool visible = object->is_visible();
        if (ImGui::Checkbox("Visible", &visible))
            object->set_visibility(visible);

        // Shadows
        bool casts_shadows = object->casts_shadows();
        if (ImGui::Checkbox("Casts shadows", &casts_shadows)) {
            if (casts_shadows)
                object->enable_shadows();
            else
                object->disable_shadows();
        }

        // Normal
        int normal_type = static_cast<int>(object->get_normal_type());
        auto normal_types = std::array<const char*, 3> { "Flip", "Outwards", "Inwards" };
        bool normal_changed = ImGuiUtils::combo_box("Normal type", normal_types, normal_type, modified);

        if (normal_changed) {
            switch (static_cast<NormalType>(normal_type)) {
            case NormalType::Flip:
                object->set_normal_flip();
                break;
            case NormalType::Outwards:
                object->set_normal_outwards();
                break;
            case NormalType::Inwards:
                object->set_normal_inwards();
                break;
            default:
                break;
            }
        }
        // Adds material editor
        if (object->has_material())
            edit_material(object->material);

        ImGui::TreePop();
    }
    return modified;
}

static bool edit_container(const std::shared_ptr<GeometricObjects::Container>& container)
{
    bool modified_child = false;
    if (ImGui::TreeNodeEx("container")) {
        int id_index = 0;
        for (auto i = container->begin(); i != container->end(); i++) {
            ImGui::PushID(id_index++);
            auto object = *i;
            modified_child |= edit_geometric_object(object);
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    return modified_child;
}
static bool edit_sampler(const char* label, std::shared_ptr<Sampler>& sampler)
{
    static std::array<const char*, 4> sampler_types = { "Regular", "Jittered", "MultiJittered", "NRooks" };
    int sampler_index = static_cast<uint32_t>(sampler->get_type());
    const char* sampler_label = sampler_types[sampler_index];

    // Tree not opened, quits
    if (!ImGui::TreeNodeEx(label))
        return false;

    ImGui::Text("%s", sampler_label);

    // New variable, so combo_box() doesn't modify 'sampler_index' var
    int modified_index = sampler_index;
    bool modified = false;
    ImGuiUtils::combo_box("Sampler type", sampler_types, modified_index, modified);

    int samples = sampler->get_num_samples();
    modified |= ImGui::InputInt("Samples", &samples);

    // If either, samples or sampler type were modified, creates a new sampler
    if (modified) {
        std::shared_ptr<Sampler> new_sampler;
        switch (static_cast<SamplerType>(modified_index)) {
        case SamplerType::Regular:
            new_sampler = std::make_shared<Samplers::RegularSampler>(samples);
            break;

        case SamplerType::Jittered:
            new_sampler = std::make_shared<Samplers::JitteredSampler>(samples);
            break;

        case SamplerType::MultiJittered:
            new_sampler = std::make_shared<Samplers::MultiJitteredSampler>(samples);
            break;

        case SamplerType::NRooks:
            new_sampler = std::make_shared<Samplers::NRooksSampler>(samples);
        }

        new_sampler->generate_samples();
        new_sampler->setup_shuffled_indices();

        // If previous sampler used Disk or Hemisphere mode,
        // copies the mode by calling the mapping methods
        if (sampler->get_mode() == SampleMode::Disk)
            new_sampler->map_samples_to_unit_disk();

        else if (sampler->get_mode() == SampleMode::Hemisphere)
            new_sampler->map_samples_to_hemisphere(1);

        // Sets the new sampler
        sampler = new_sampler;
    }
    ImGui::TreePop();
    return modified;
}

static bool edit_light(std::shared_ptr<Light>& light)
{

    static std::array<const char*, 8> light_types = {
        "Ambient light",
        "Directional light",
        "Point light",
        "Ambient occluder",
        "Area light",
        "Environment light",
        "Jittered Point light",
        "Jittered Directional light"
    };

    int current_light_index = static_cast<uint32_t>(light->get_type());

    const char* light_label = light_types[static_cast<uint32_t>(light->get_type())];
    bool modified = false;
    if (ImGui::TreeNodeEx(light_label)) {

        // 1 - Light type selector
        bool changed = ImGuiUtils::combo_box("Light type", light_types, current_light_index, modified);

        LightType selected = static_cast<LightType>(current_light_index);

        if (changed && selected != light->get_type()) {
            switch (selected) {
            case LightType::Ambient:
                light = std::make_shared<Lights::AmbientLight>();
                break;
            case LightType::Directional:
                light = std::make_shared<Lights::DirectionalLight>();
                break;
            case LightType::JitteredDirectional: {
                auto jitteredDirectional = std::make_shared<Lights::JitteredDirectionalLight>();
                jitteredDirectional->set_samples(1);
                light = jitteredDirectional;
            } break;
            case LightType::Point:
                light = std::make_shared<Lights::PointLight>();
                break;
            case LightType::JitteredPoint: {
                auto jitteredPoint = std::make_shared<Lights::JitteredDirectionalLight>();
                jitteredPoint->set_samples(1);
                light = jitteredPoint;
            } break;
            case LightType::AmbientOccluder: {
                auto ao = std::make_shared<Lights::AmbientOccluder>();
                ao->set_sampler(std::make_shared<Samplers::MultiJitteredSampler>(1));
                light = ao;
            } break;
            case LightType::Environment: {
                auto environment = std::make_shared<Lights::EnvironmentLight>();
                environment->set_sampler(std::make_shared<Samplers::MultiJitteredSampler>(1));
                light = environment;
            } break;
            case LightType::Area: {
                break;
            }

            default:
                break;
            }
        }

        // 2 - Properties modifier
        ImGui::Checkbox("Casts shadows", &light->shadows);
        switch (light->get_type()) {
        case LightType::Ambient: {
            auto ambient = std::dynamic_pointer_cast<Lights::AmbientLight>(light);
            ImGuiUtils::color_edit("Color", ambient->color, modified);
            ImGui::InputFloat("Scale radiance", &ambient->ls);
        } break;

        case LightType::Directional: {
            auto directional = std::dynamic_pointer_cast<Lights::DirectionalLight>(light);
            ImGuiUtils::color_edit("Color", directional->color, modified);
            ImGui::InputFloat("Scale radiance", &directional->ls);
            ImGuiUtils::input("Direction", directional->direction, modified);

        } break;

        case LightType::Point: {
            auto point = std::dynamic_pointer_cast<Lights::PointLight>(light);
            ImGuiUtils::color_edit("Color", point->color, modified);
            ImGui::InputFloat("Scale radiance", &point->ls);
            ImGui::InputFloat("Fall off power", &point->k);
            ImGuiUtils::input("Origin", point->origin, modified);
        } break;

        case LightType::JitteredPoint: {
            auto point = std::dynamic_pointer_cast<Lights::JitteredPointLight>(light);

            Vec3 origin = point->get_origin();
            float scale_radiance = point->get_scale_radiance();
            float fall_off_power = point->get_fall_off_power();
            float radius = point->get_radius();
            RGBColor color = point->get_color();

            ImGuiUtils::input("Origin", origin, modified);
            ImGuiUtils::input("Scale radiance", scale_radiance, modified);
            ImGuiUtils::input("Fall off power", fall_off_power, modified);
            ImGuiUtils::input("Radius", radius, modified);
            ImGuiUtils::color_edit("Color", color, modified);

            point->set_origin(origin);
            point->set_scale_radiance(scale_radiance);
            point->set_fall_off_power(fall_off_power);
            point->set_radius(radius);
            point->set_color(color);

            edit_sampler("Sampler", point->get_sampler());

        } break;
        case LightType::JitteredDirectional: {
            auto directional = std::dynamic_pointer_cast<Lights::JitteredDirectionalLight>(light);

            Vec3 direction = directional->get_direction();
            float scale_radiance = directional->get_scale_radiance();
            float theta = directional->get_theta();
            RGBColor color = directional->get_color();

            ImGuiUtils::input("Direction", direction, modified);
            ImGuiUtils::input("Scale radiance", scale_radiance, modified);
            ImGuiUtils::input("Theta", theta, modified);
            ImGuiUtils::color_edit("Color", color, modified);

            directional->set_direction(direction);
            directional->set_scale_radiance(scale_radiance);
            directional->set_theta(theta);
            directional->set_color(color);

            edit_sampler("Sampler", directional->get_sampler());

        } break;

        case LightType::AmbientOccluder: {
            auto ao = std::dynamic_pointer_cast<Lights::AmbientOccluder>(light);
            ImGuiUtils::color_edit("Color", ao->color, modified);
            ImGui::InputFloat("Scale radiance", &ao->ls);
            ImGui::InputFloat("Min intensity", &ao->min_intensity);
            edit_sampler("Hemisphere sampler", ao->sampler);
        } break;

        case LightType::Environment: {
            auto environment = std::dynamic_pointer_cast<Lights::EnvironmentLight>(light);
            edit_material(environment->get_material());
            edit_sampler("Hemisphere sampler", environment->get_sampler());
        } break;

        case LightType::Area: {
        } break;

        default:
            std::cout << "Unimplemented light type UI" << std::endl;
        }

        ImGui::TreePop();
    }
    return modified;
}
} // namespace ImGuiRT

#endif