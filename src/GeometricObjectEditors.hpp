#ifndef __GEOMETRIC_OBJECT_EDITORS__
#define __GEOMETRIC_OBJECT_EDITORS__

#include "ImGuiRT.hpp"
#include <CPU-Ray-Tracing/CPURayTracer.hpp>

namespace Editor {
namespace ObjectEditor {

    enum EditState : uint8_t {
        None = 0,
        BoundingBoxEdit = 1,
        PropertyEdit = 2
    };

    uint8_t edit_sphere(RT::GeometricObjectPtr& object);

    uint8_t edit_plane(RT::GeometricObjectPtr& object);

    uint8_t edit_box(RT::GeometricObjectPtr& object);

    uint8_t edit_compound_box(RT::GeometricObjectPtr& object);

    uint8_t edit_capsule(RT::GeometricObjectPtr& object);

    uint8_t edit_disk(RT::GeometricObjectPtr& object);

    uint8_t edit_annulus(RT::GeometricObjectPtr& object);

    uint8_t edit_rect(RT::GeometricObjectPtr& object);

    uint8_t edit_triangle(RT::GeometricObjectPtr& object);

    uint8_t edit_cone(RT::GeometricObjectPtr& object);

    uint8_t edit_solid_cone(RT::GeometricObjectPtr& object);

    uint8_t edit_generic_cylinder(RT::GeometricObjectPtr& object);

    uint8_t edit_thick_annulus(RT::GeometricObjectPtr& object);

    uint8_t edit_part_cylinder(RT::GeometricObjectPtr& object);

    uint8_t edit_part_sphere(RT::GeometricObjectPtr& object);

    uint8_t edit_part_torus(RT::GeometricObjectPtr& object);

    uint8_t edit_torus(RT::GeometricObjectPtr& object);

    uint8_t edit_instance(RT::GeometricObjectPtr& object);

    uint8_t edit_bvh(RT::GeometricObjectPtr& object);

    uint8_t edit_container(RT::GeometricObjectPtr& object);

    uint8_t edit_transform_container(RT::GeometricObjectPtr& object);

    uint8_t edit_bowl(RT::GeometricObjectPtr& object);

}

}

#endif