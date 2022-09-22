#pragma once

#include <string>

class Component {
public:
    virtual void update() = 0;
    virtual ~Component() = default;

    inline void set_name(const std::string& newName) { name = newName; };
    inline const std::string& get_name() const { return name; };
private:
    std::string name;
};
