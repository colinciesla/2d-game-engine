//
// Created by colin on 6/24/2024.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

#include "game.h"
#include "../logger/logger.h"
#include "../ecs/ecs.h"

game::game() {
  isRunning = false;
  logger::log("Game constructor called!");
}

game::~game() {
  logger::log("Game destructor called!");
}

void game::initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    logger::log("Error initializing SDL!");

    return;
  }

  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);

  windowWidth = 800;
  windowHeight = 640;

  window = SDL_CreateWindow(nullptr,
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            windowWidth,
                            windowHeight,
                            SDL_WINDOW_BORDERLESS);

  if (!window) {
    logger::log("Error creating SDL window!");

    return;
  }

  renderer = SDL_CreateRenderer(window,
                                -1,
                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    logger::log("Error creating SDL renderer!");

    return;
  }

  isRunning = true;
}

void game::processInput() {
  SDL_Event sdlEvent;

  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
      case SDL_QUIT:
        isRunning = false;
        break;
      case SDL_KEYDOWN:
        if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
          isRunning = false;
        }
        break;
      default:
        break;
    }
  }
}

void game::setup() {
}

void game::update() {
  if (const uint64_t timeToWait = MILLISECONDS_PER_FRAME - (
                                    SDL_GetTicks64() - millisecondsPreviousFrame);
    timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME
  ) {
    SDL_Delay(timeToWait);
  }

  const float deltaTime{
    (static_cast<float>(SDL_GetTicks64()) - static_cast<float>(millisecondsPreviousFrame)) /
    1000.0f};

  millisecondsPreviousFrame = SDL_GetTicks64();
}

void game::render() const {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}

void game::run() {
  setup();

  while (isRunning) {
    processInput();
    update();
    render();
  }
}

void game::destroy() const {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
