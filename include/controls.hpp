#pragma once

namespace Input {
    enum class Control { Up, Down, Left, Right, Wireframe };

    bool IsControlPressed(Control c);
}
