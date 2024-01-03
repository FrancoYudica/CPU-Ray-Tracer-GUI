#include "RenderSettings.hpp"

using namespace RT;
using namespace RenderSettings;

Settings RenderSettings::read_settings(const RT::World& world)
{
    return Settings {
        world.view_plane.sampler->get_num_samples(),
        world.view_plane.sampler->get_type(),
        world.view_plane.h_res,
        world.view_plane.v_res,
        world.tracer->get_type()
    };
}

void RenderSettings::load_settings(RT::World& world, const Settings& settings)
{

    // Sampler setup
    {
        std::shared_ptr<Sampler> sampler;
        switch (settings.sampler_type) {
        case SamplerType::Regular:
            sampler = std::make_shared<Samplers::RegularSampler>(settings.sample_count);
            break;

        case SamplerType::Jittered:
            sampler = std::make_shared<Samplers::JitteredSampler>(settings.sample_count);
            break;

        case SamplerType::MultiJittered:
            sampler = std::make_shared<Samplers::MultiJitteredSampler>(settings.sample_count);
            break;

        case SamplerType::NRooks:
            sampler = std::make_shared<Samplers::NRooksSampler>(settings.sample_count);
        }

        world.view_plane.set_sampler(sampler);
    }

    // Viewport size
    {
        world.view_plane.set_hres(settings.viewport_width);
        world.view_plane.set_vres(settings.viewport_height);
    }

    // Tracer
    {
        std::shared_ptr<Tracer> new_tracer;
        switch (settings.tracer_type) {
        case TracerType::RayCast:
            new_tracer = std::make_shared<Tracers::RayCastTracer>(world);
            break;
        case TracerType::Depth:
            new_tracer = std::make_shared<Tracers::DepthTracer>(world);
            break;
        case TracerType::Normal:
            new_tracer = std::make_shared<Tracers::NormalTracer>(world);
            break;
        case TracerType::AreaLighting:
            new_tracer = std::make_shared<Tracers::AreaLighting>(world);
            break;
        }
        world.set_tracer(new_tracer);
    }
}