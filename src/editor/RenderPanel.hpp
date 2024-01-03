#ifndef __EDITOR_RT_RENDER_PANEL__
#define __EDITOR_RT_RENDER_PANEL__
#include "../RenderSettings.hpp"
#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include "Panel.hpp"
#include "SceneNode.hpp"
#include "WEngine.h"

using namespace RT;

namespace Editor {

class RenderPanel : public Panel {

public:
    RenderPanel()
        : Panel("Render")
    {
    }

    void render_scene(World& world, const RenderSettings::Settings& settings);

    /// @brief Time in seconds of the last render
    inline double get_render_time() const { return _render_time; }

protected:
    virtual void _pre_render() override;
    virtual void _on_render() override;
    virtual void _post_render() override;

private:
private:
    SceneNodePtr _scene_node;
    double _render_time;
    std::shared_ptr<Wolf::Rendering::Texture> _texture;
    uint32_t _render_width, _render_height;
};

}

#endif