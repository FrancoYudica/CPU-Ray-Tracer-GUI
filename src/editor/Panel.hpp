#ifndef __EDITOR_RT_PANEL__
#define __EDITOR_RT_PANEL__

#include <imgui/imgui.h>
#include <string>

namespace Editor {

class Panel {
public:
    Panel(const std::string& name)
        : _name(name)
        , _position()
        , _size()
        , _window_flags(ImGuiWindowFlags_None)
    {
    }
    inline const std::string& get_name() const { return _name; }
    inline ImVec2 get_position() const { return _position; }
    inline ImVec2 get_size() const { return _size; }

    void show();
    bool is_hovered();

protected:
    /// @brief Called before creating window
    virtual void _pre_render() { }

    /// @brief Called when window is created
    virtual void _on_render() = 0;

    /// @brief Called after closing window
    virtual void _post_render() { }

protected:
    ImGuiWindowFlags _window_flags;

private:
    std::string _name;
    ImVec2 _position;
    ImVec2 _size;
};
}

#endif