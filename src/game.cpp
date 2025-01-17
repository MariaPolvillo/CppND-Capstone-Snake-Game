#include "game.h"
#include <iostream>
#include "SDL.h"
#include <chrono>

Game::Game(std::size_t grid_width, std::size_t grid_height, Renderer &renderer)
    : snake(grid_width, grid_height), engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      gameModeSpawn(static_cast<int>(GameMode::speedUp), static_cast<int>(GameMode::solidBorders)),
      releaseModeEffect(false), gameModeSpawned(false), renderer(renderer),
      solidBorders(false)
      {
  PlaceFood();
}

void Game::Run(Controller &controller, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  bool running = true;
  futureGameMode = std::async(std::launch::async, [this]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return generateSpectialFoodAndSetModeGame();
  });

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update(controller);
    {
      std::unique_lock<std::mutex>(mtx);
      if (releaseModeEffect &&
          releaseFuture.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready) {
        releaseFuture.get();
        releaseModeEffect = false;
      }
      if (!gameModeSpawned &&
          futureGameMode.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready) {
        gameModeType = futureGameMode.get();
        futureGameMode = std::async(std::launch::async, [this]() {
          std::this_thread::sleep_for(std::chrono::milliseconds(2*1000));
          return generateSpectialFoodAndSetModeGame();
        });
      }

      if(gameModeSpawned)
      {
        renderer.Render(snake, food, solidBorders, specialFood);
      }
      else
      {
          renderer.Render(snake, food, solidBorders);
      }      
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      std::unique_lock<std::mutex>(mtx);
      renderer.UpdateWindowTitle(score);      
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  std::unique_lock<std::mutex>(mtx);

  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

GameMode Game::generateSpectialFoodAndSetModeGame()
{
  std::unique_lock<std::mutex>(mtx);
  int x, y;
  while (true) {    
    x = random_w(engine);
    y = random_h(engine);

    // Check that the location is not occupied by a snake item before placing
    //  special food.
    if (!snake.SnakeCell(x, y)) {
      specialFood.x = x;
      specialFood.y = y;      
      gameModeSpawned = true;      
      return setNewModeGame();
    }    
  }
}

GameMode Game::setNewModeGame() {  
  return static_cast<GameMode>(gameModeSpawn(engine));
}

void Game::Update(Controller &controller) {
  if (!snake.alive)
    return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  // Check if there's specialfood has been eaten
  else if(gameModeSpawned && specialFood.x == new_x && specialFood.y == new_y){
    std::unique_lock<std::mutex>(mtx);
    score+=2;
    snake.GrowBody();
    UpdateGameMode(controller);        
    gameModeSpawned = false;
  }
  
}

void Game::UpdateGameMode(Controller &controller) {

  switch (gameModeType) {
  case GameMode::speedUp:
    snake.speed = 1.5 * snake.speed;
    // launch delayed action to undo effect
    releaseFuture = std::async(std::launch::async, [this]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * TIME_GAME_MODE_REMAINS));
      {
        std::unique_lock<std::mutex>(mtx);
        snake.speed = snake.speed / 1.5;
      }
      return;
    });
    releaseModeEffect = true;
    break;
  case GameMode::speedDown:
    snake.speed = 0.5 * snake.speed;

    // launch delayed action to undo effect
    releaseFuture = std::async(std::launch::async, [this]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * TIME_GAME_MODE_REMAINS));
      {
        std::unique_lock<std::mutex>(mtx);
        snake.speed = snake.speed / 0.5;
      }
      return;
    });
    releaseModeEffect = true;
    break;
  case GameMode::invertedControls:
    controller.setInvertControl(true);
    // launch delayed action to undo effect
    releaseFuture = std::async(std::launch::async, [this, &controller]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * TIME_GAME_MODE_REMAINS));
      {
        std::unique_lock<std::mutex>(mtx);
        controller.setInvertControl(false);
      }
      return;
    });
    releaseModeEffect = true;
    break;
  case GameMode::solidBorders:
    snake.setSolidBorders(true);
    solidBorders = true;
    // launch delayed action to undo effect
    releaseFuture = std::async(std::launch::async, [this]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(1000 * TIME_GAME_MODE_REMAINS));
      {
        std::unique_lock<std::mutex>(mtx);
        snake.setSolidBorders(false);
      }
      solidBorders = false;
      releaseModeEffect = true;
    });
    break;
  default:
    break;
  };
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
