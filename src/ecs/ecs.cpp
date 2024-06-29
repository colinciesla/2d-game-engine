//
// Created by colin on 6/27/2024.
//

#include "ecs.h"

#include <algorithm>

int entity::getId() const {
  return id;
}

bool entity::operator==(const entity& entity) const {
  return getId() == entity.getId();
}

bool entity::operator!=(const entity& entity) const {
  return getId() != entity.getId();
}

void system::addEntityToSystem(const entity entity) {
  entities.push_back(entity);
}

void system::removeEntityFromSystem(const entity entity) {
  std::erase(entities, entity);
}

[[nodiscard]] const std::vector<entity>& system::getSystemEntities() const {
  return entities;
}

[[nodiscard]] const signature& system::getComponentSignature() const {
  return componentSignature;
}
