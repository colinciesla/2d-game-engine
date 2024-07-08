//
// Created by colin on 6/27/2024.
//

#include <memory>
#include <ranges>

#include "ecs.h"
#include "logger/logger.h"

using namespace ecs;

int iComponent::nextId = 0;

int entity::getId() const {
  return id;
}

bool entity::operator==(const entity& entity) const {
  return getId() == entity.getId();
}

bool entity::operator!=(const entity& entity) const {
  return getId() != entity.getId();
}

bool entity::operator<(const entity& entity) const {
  return getId() < entity.getId();
}

bool entity::operator>(const entity& entity) const {
  return getId() > entity.getId();
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

entity registry::addEntity() {
  const int entityId = ++numberOfEntities;

  if (entityId >= static_cast<int>(entityComponentSignatures.size())) {
    entityComponentSignatures.resize(entityId + 1);
  }

  const entity entity(entityId);
  entitiesToBeAdded.insert(entity);
  logger::log("Entity created with ID = " + std::to_string(entityId));

  return entity;
}

void registry::update() {
  // Add the entities that are waiting to be created to the active systems
  for (const entity entity : entitiesToBeAdded) {
    addEntityToSystems(entity);
  }

  entitiesToBeAdded.clear();
}

void registry::addEntityToSystems(const entity entity) {
  const int entityId = entity.getId();

  const signature entityComponentSignature = entityComponentSignatures[entityId];

  for (const std::shared_ptr<system>& system : systems | std::views::values) {
    if (const signature& systemComponentSignature = system->getComponentSignature();
      (entityComponentSignature & systemComponentSignature) == systemComponentSignature) {
      system->addEntityToSystem(entity);
    }
  }
}
