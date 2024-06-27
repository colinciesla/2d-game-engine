//
// Created by colin on 6/24/2024.
//

#pragma once

#include <SDL2/SDL.h>

constexpr int FPS{120};
constexpr int MILLISECONDS_PER_FRAME{1000 / FPS};

class game
{
  bool isRunning;
  uint64_t millisecondsPreviousFrame{0};
  SDL_Window *window{};
  SDL_Renderer *renderer{};

public:
  int windowWidth{};
  int windowHeight{};

  game();

  ~game();

  void initialize();

  void setup();

  void run();

  void destroy() const;

  void processInput();

  void update();

  void render() const;
};
