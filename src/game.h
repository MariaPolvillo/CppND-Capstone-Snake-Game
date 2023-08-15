#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <future>
#include <mutex>

#define GAME_MODE_REMAINS 3

enum GameMode : unsigned int {
  speedUp = 0,
  speedDown,
  invertedControls,  
  solidBorders  
};

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, Renderer &renderer);
  void Run(Controller &controller, std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;  

  std::mutex mtx;
  std::future<GameMode> futureGameMode;
  std::future<void> releaseFuture;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_int_distribution<int> gameModeSpawn; 

  int score{0}; 
  bool releaseModeEffect;
  bool gameModeSpawned;
  GameMode gameModeType;
  bool solidBorders;  
  Renderer renderer;

  void PlaceFood();
  GameMode setModeGame();
  void Update(Controller &controller);
  void UpdateGameMode(Controller &controller);

};

#endif