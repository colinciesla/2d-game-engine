//
// Created by colin on 6/27/2024.
//

#pragma once

#include <bitset>
#include <vector>

constexpr unsigned int MAX_COMPONENTS = 32;

/////////////////////////////////////////////////////////////////////////////////
// signature
/////////////////////////////////////////////////////////////////////////////////
// We use a bitset (0s and 1s) to keep track of which components aqn entity has,
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
  // Returns the unique ID of component<T>.
  static int getId() {
    static auto id = ++nextId;

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

class registry {
};

template <typename tComponent>
void system::requireComponent() {
  const auto componentId = component<tComponent>::getId();
  componentSignature.set(componentId);
}
