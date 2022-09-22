#pragma once
#include "component.hpp"

#include <vector>
#include <string>

namespace ComponentManager {
    std::vector<Component*>& get_components();
    void add_component(Component* c);
    Component* find_component(const std::string& name);
}
