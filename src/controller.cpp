#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        ChangeDirection(
            snake, _inverted ? Snake::Direction::kDown : Snake::Direction::kUp,
            _inverted ? Snake::Direction::kUp : Snake::Direction::kDown);
        break;

      case SDLK_DOWN:
        ChangeDirection(
            snake, _inverted ? Snake::Direction::kUp : Snake::Direction::kDown,
            _inverted ? Snake::Direction::kDown : Snake::Direction::kUp);
        break;

      case SDLK_LEFT:
        ChangeDirection(
            snake,
            _inverted ? Snake::Direction::kRight : Snake::Direction::kLeft,
            _inverted ? Snake::Direction::kLeft : Snake::Direction::kRight);
        break;

      case SDLK_RIGHT:
        ChangeDirection(
            snake,
            _inverted ? Snake::Direction::kLeft : Snake::Direction::kRight,
            _inverted ? Snake::Direction::kRight : Snake::Direction::kLeft);
        break;
      }
    }
  }
}