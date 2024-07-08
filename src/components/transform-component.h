//
// Created by colin on 7/8/2024.
//

#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include <glm/glm.hpp>

struct transformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;

  transformComponent(const glm::vec2 position = glm::vec2(0.0f, 0.0f),
                              const glm::vec2 scale = glm::vec2(1.0f, 1.0f),
                              const double rotation = 0.0) :
    position(position),
    scale(scale), rotation(rotation) {
  }
};
#endif //TRANSFORM_COMPONENT_H
