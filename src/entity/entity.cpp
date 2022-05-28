#include <vector>

#include "entity/entity.hpp"

static std::vector<EntityId> openEntities;

EntityId Entities::GenerateEntity() {
    static EntityId currentId{ 0 };
    return currentId++;
}

void Entities::DestroyEntity(EntityId id) {
    openEntities.push_back(id);
}