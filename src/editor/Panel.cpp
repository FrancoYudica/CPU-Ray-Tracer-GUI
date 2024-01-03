#include "Panel.hpp"

void Editor::Panel::show()
{
    _pre_render();
    ImGui::Begin(_name.c_str(), nullptr, _window_flags);
    _position = ImGui::GetCursorScreenPos();
    _size = ImGui::GetContentRegionAvail();
    _on_render();
    ImGui::End();
    _post_render();
}

bool Editor::Panel::is_hovered()
{
    ImVec2 mouse = ImGui::GetMousePos();
    return _position.x < mouse.x
        && _position.y < mouse.y
        && _position.x + _size.x > mouse.x
        && _position.y + _size.y > mouse.y;
}
