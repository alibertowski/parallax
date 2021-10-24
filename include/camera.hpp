#pragma once

#include "component.hpp"
#include "settings.hpp"
#include "coordinate_frame.hpp"
#include "collision.hpp"

#include <glm/mat4x4.hpp>
#include <glm/ext/vector_float3.hpp>

class Camera : public Component {
public:
    Camera();

    glm::mat4 calculate_view();
    glm::mat4 get_projection();
    virtual void update() override;
    inline CoordinateFrame& get_coordinate_frame() { return cframe; };
    inline const Collision& get_collision() const { return collision; };
private:
    float lastX{ Settings::DefaultWidth / 2 };
    float lastY{ Settings::DefaultHeight / 2} ;
    float yaw{ -90.0f };
    float pitch{ 0.0f };
    CoordinateFrame cframe;
    Collision collision{glm::vec3{ 1.0f, 3.0f, 1.0f} };
};
