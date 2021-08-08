#include "component_manager.hpp"
#include "component.hpp"

#include <vector>

static std::vector<Component*> components;

std::vector<Component*>& ComponentManager::get_components() {
    return components;
}

void ComponentManager::add_component(Component* c) {
    components.push_back(c);
}

Component* ComponentManager::find_component(const std::string& name) {
    for(Component* c : components) {
        if (c->get_name() == name) 
            return c;
    }

    return nullptr;
}
