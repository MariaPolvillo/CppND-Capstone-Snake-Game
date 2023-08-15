#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {

 public:
  void HandleInput(bool &running, Snake &snake) const;
  void setInvertControl(bool invert){ _inverted = invert;}

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  bool _inverted = false;
};

#endif