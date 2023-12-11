#ifndef __EDITOR_UI__
#define __EDITOR_UI__
#include <imgui/imgui.h>

namespace Editor {

class EditorUI {
public:
    EditorUI() = default;

    void init();
    void show();
};

}

#endif