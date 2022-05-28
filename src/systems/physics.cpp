// #include <glm/ext/vector_float3.hpp>
// #include <vector>
#include <cstdio>

#include "systems/physics.hpp"
#include "entity/physics_component.hpp"
#include "entity/components.hpp"
// #include "entity/physics_bodies.hpp"
// #include "entity/entity.hpp"

// // EntityId obj = Entities::GenerateEntity();
// // PhysicsBodies p;
// // p.register_entity(obj);


// void update() {
//     /* for each(i < p.positions.size())
//         p.positions[i] = vec3{69, 1337, 0};
//     */
// }

void Physics::update() {
    ComponentManager<PhysicsComponent> cm{};
    for(EntityId i{ 0 }; i < 10; ++i) {
        cm.register_entity(i);
    }
    // const auto c{ PhysicsComponents::get_component() };
    // const auto x{ }
    // for(EntityId i{ 0 }; i < c.velocity.size(); ++i) {
    //     printf("%f", c.velocity[i].x);
    // }
}