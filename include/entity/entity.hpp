#pragma once
using EntityId = unsigned int;

struct Entities {
    EntityId GenerateEntity();
    void DestroyEntity(EntityId id);
};