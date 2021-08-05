#pragma once

#include "component.hpp"
#include "settings.hpp"
#include "coordinate_frame.hpp"

#include <glm/mat4x4.hpp>

class Camera : public Component {
public:
    glm::mat4 calculate_view();
    glm::mat4 get_projection();
    virtual void update() override;
private:
    float lastX{ Settings::DefaultWidth / 2 };
    float lastY{ Settings::DefaultHeight / 2} ;
    float yaw{ -90.0f };
    float pitch{ 0.0f };
    CoordinateFrame cframe;
};
