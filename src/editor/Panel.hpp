#ifndef __EDITOR_RT_PANEL__
#define __EDITOR_RT_PANEL__

#include <imgui/imgui.h>
#include <string>

namespace Editor {

class Panel {
public:
    Panel(const std::string& name)
        : _name(name)
    {
    }
    const std::string& get_name() const { return _name; }

    void show()
    {
        ImGui::Begin(_name.c_str());
        _on_render();
        ImGui::End();
    }

protected:
    virtual void _on_render() = 0;

protected:
    std::string _name;
};
}

#endif