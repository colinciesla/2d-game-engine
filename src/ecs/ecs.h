//
// Created by colin on 6/27/2024.
//

#pragma once

#include <bitset>
#include <set>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "logger/logger.h"

namespace ecs {
  constexpr unsigned int MAX_COMPONENTS = 32;

  /////////////////////////////////////////////////////////////////////////////////
  // signature
  /////////////////////////////////////////////////////////////////////////////////
  // We use a bitset (0s and 1s) to keep track of which components an entity has,
  // and also helps keep track of which entities a system is interested in.
  /////////////////////////////////////////////////////////////////////////////////
  typedef std::bitset<MAX_COMPONENTS> signature;

  struct iComponent {
    protected:
      static int nextId;
  };

  // Used to assign a unique ID to a component type.
  template <typename tComponent>
  class component : public iComponent {
    public:
      // Returns the unique ID of component<t>.
      static int getId() {
        static int id = ++nextId;

        return id;
      }
  };

  class entity {
    int id;

    public:
      explicit entity(const int id) : id(id) {
      }

      entity(const entity& entity) = default;

      [[nodiscard]] int getId() const;

      entity& operator=(const entity& entity) = default;

      bool operator==(const entity& entity) const;

      bool operator!=(const entity& entity) const;

      bool operator<(const entity& entity) const;

      bool operator>(const entity& entity) const;
  };

  /////////////////////////////////////////////////////////////////////////////////
  // system
  /////////////////////////////////////////////////////////////////////////////////
  // The system processes entities that contain a specific signature.
  /////////////////////////////////////////////////////////////////////////////////
  class system {
    signature componentSignature;
    std::vector<entity> entities;

    public:
      system() = default;

      ~system() = default;

      void addEntityToSystem(entity entity);

      void removeEntityFromSystem(entity entity);

      [[nodiscard]] const std::vector<entity>& getSystemEntities() const;

      [[nodiscard]] const signature& getComponentSignature() const;

      // Defines the component type that entities must have to be considered by the system.
      template <typename t>
      void requireComponent();
  };

  template <typename tComponent>
  void system::requireComponent() {
    const int componentId = component<tComponent>::getId();
    componentSignature.set(componentId);
  }

  /////////////////////////////////////////////////////////////////////////////////
  // pool
  /////////////////////////////////////////////////////////////////////////////////
  // A pool is just a vector (contiguous data) of ogbjects of type T.
  /////////////////////////////////////////////////////////////////////////////////
  class iPool {
    public:
      virtual ~iPool() = default;

      [[nodiscard]] virtual bool isEmpty() const = 0;

      [[nodiscard]] virtual int getSize() const = 0;

      virtual void resize(int n) = 0;

      virtual void clear() = 0;
  };


  template <typename t>
  class pool final : public iPool {
    std::vector<t> data;

    public:
      explicit pool(const int size = 100) {
        data.resize(size);
      }

      ~pool() override = default;

      [[nodiscard]] bool isEmpty() const override {
        return data.empty();
      }

      [[nodiscard]] int getSize() const override {
        return data.size();
      }

      void resize(int n) override {
        data.resize(n);
      }

      void clear() override {
        data.clear();
      }

      void add(t object) {
        data.push_back(object);
      }

      void set(int index, t object) {
        data[index] = object;
      }

      t& get(int index) {
        return static_cast<t>(data[index]);
      }

      t& operator[](unsigned int index) {
        return data[index];
      }
  };

  /////////////////////////////////////////////////////////////////////////////////
  // registry
  /////////////////////////////////////////////////////////////////////////////////
  // The registry manages the creation and destruction of entities, components,
  // and systems.
  /////////////////////////////////////////////////////////////////////////////////
  class registry {
    int numberOfEntities{0};

    // Set of entities that are flagged to be added or removed in the next
    // registry update.
    std::set<entity> entitiesToBeAdded;
    std::set<entity> entitiesToBeRemoved;

    // Vector of component pools. Each pool contains all of the data for a
    // certain component type.
    // [Vector index = component type ID]
    // [Pool index = entity ID]
    std::vector<std::shared_ptr<iPool>> componentPools;

    // Vector of component signatures per entity, saying which component is
    // turned "on" for a given entity.
    // [Vector index = entity ID]
    std::vector<signature> entityComponentSignatures;

    std::unordered_map<std::type_index, std::shared_ptr<system>> systems;

    public:
      registry() {
        logger::log("Registry created!");
      };

      ~registry() {
        logger::log("Registry destroyed!");
      };

      // Processes the entities that are waiting to be added or removed.
      void update();

      // Entity management
      entity addEntity();

      // Component management
      template <typename tComponent, typename... tArgs>
      void addComponent(entity entity, tArgs&&... args);

      template <typename tComponent>
      void removeComponent(entity entity);

      template <typename tComponent>
      tComponent& getComponent(entity entity) const;

      template <typename tComponent>
      bool hasComponent(entity entity) const;

      // System management
      template <typename tSystem, typename... tArgs>
      void addSystem(tArgs&&... args);

      template <typename tSystem>
      void removeSystem();

      template <typename tSystem>
      tSystem& getSystem() const;

      template <typename tSystem>
      bool hasSystem() const;

      // Checks the component signature of an entity and adds the entity to the
      // systems that are interested in it.
      void addEntityToSystems(entity entity);
  };

  template <typename tComponent, typename... tArgs>
  void registry::addComponent(const entity entity, tArgs&&... args) {
    const int componentId = component<tComponent>::getId();
    const int entityId = entity.getId();

    // If the component ID is greater than the current size of the component
    // pools, then resize the vector.
    if (componentId >= static_cast<int>(componentPools.size())) {
      componentPools.resize(componentId + 1, nullptr);
    }

    // If we still don't have a pool for that component type, create one.
    if (!componentPools[componentId]) {
      // Get the pool of component values for that component type.
      std::shared_ptr<pool<tComponent>> newComponentPool = std::make_shared<pool<tComponent>>();
      componentPools[componentId] = newComponentPool;
    }

    std::shared_ptr<pool<tComponent>> componentPool = std::static_pointer_cast<pool<tComponent>>(
      componentPools[componentId]);

    // If the entity ID is greater than the current size of the component pool,
    // then resize the pool.
    if (entityId >= componentPool->getSize()) {
      componentPool->resize(numberOfEntities);
    }

    // Create a new component object of type t, and forward the args to constructor.
    tComponent newComponent(std::forward<tArgs>(args)...);

    // Add the new component to the component pool list, using the entity ID as index.
    componentPool->set(entityId, newComponent);

    // Change the component siganature of the entity and set the component ID on the
    // bitset to 1.
    entityComponentSignatures[entityId].set(componentId);

    logger::log("Component ID = " + std::to_string(componentId) + " added to entity ID = " +
                std::to_string(entityId));
  }

  template <typename tComponent>
  void registry::removeComponent(const entity entity) {
    const int componentId = component<tComponent>::getId();
    const int entityId = entity.getId();

    entityComponentSignatures[entityId].set(componentId, false);
  }

  template <typename tComponent>
  tComponent& registry::getComponent(const entity entity) const {
    const int componentId = component<tComponent>::getId();
    const int entityId = entity.getId();

    return pool<tComponent>(componentPools[componentId])->get(entityId);
  }

  template <typename tComponent>
  bool registry::hasComponent(const entity entity) const {
    const int componentId = component<tComponent>::getId();
    const int entityId = entity.getId();

    return entityComponentSignatures[entityId].test(componentId);
  }

  template <typename tSystem, typename... tArgs>
  void registry::addSystem(tArgs&&... args) {
    std::shared_ptr<tSystem> newSystem = std::make_shared<tSystem>(std::forward<tArgs>(args)...);

    systems.insert(std::make_pair(std::type_index(typeid(tSystem)), newSystem));
  }

  template <typename tSystem>
  void registry::removeSystem() {
    const auto system = systems.find(std::type_index(typeid(tSystem)));

    systems.erase(system);
  }

  template <typename tSystem>
  tSystem& registry::getSystem() const {
    const auto system = systems.find(std::type_index(typeid(tSystem)));

    return *std::static_pointer_cast<tSystem>(system->second);
  }

  template <typename tSystem>
  bool registry::hasSystem() const {
    return systems.contains(std::type_index(typeid(tSystem)));
  }
};
