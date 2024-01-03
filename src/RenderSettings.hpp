#ifndef __RENDER_SETTINGS__
#define __RENDER_SETTINGS__
#include <CPU-Ray-Tracing/CPURayTracer.hpp>

namespace RenderSettings {

struct Settings {

    uint32_t sample_count;
    RT::SamplerType sampler_type;
    uint32_t viewport_width, viewport_height;
    RT::TracerType tracer_type;
};

/// @brief Reads settings from world and returns it's state
Settings read_settings(const RT::World& world);

/// @brief Sets up settings in world
void load_settings(RT::World& world, const Settings& settings);

constexpr Settings normal_settings {
    1,
    RT::SamplerType::Regular,
    300,
    300,
    RT::TracerType::Normal
};

constexpr Settings lightweight_settings {
    1,
    RT::SamplerType::Regular,
    300,
    300,
    RT::TracerType::AreaLighting
};

constexpr Settings performant_settings {
    16,
    RT::SamplerType::MultiJittered,
    300,
    300,
    RT::TracerType::AreaLighting
};

constexpr Settings release_settings {
    144,
    RT::SamplerType::MultiJittered,
    300,
    300,
    RT::TracerType::AreaLighting
};

}

#endif