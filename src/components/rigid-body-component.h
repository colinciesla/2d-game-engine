//
// Created by colin on 7/8/2024.
//

#pragma once

#include <glm/glm.hpp>

struct rigidBodyComponent {
  glm::vec2 velocity;

  explicit rigidBodyComponent(const glm::vec2 velocity = glm::vec2(0.0f, 0.0f)) : velocity(velocity) {
  }
};
