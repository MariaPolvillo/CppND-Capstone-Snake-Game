# CPPND: Capstone Snake Game Example

This is a modification of the starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## Overview of The Code Structure and Added functionalities
This code follows the same structure described in the Capstone project. The Snake game code consists of four main classes: Game, Snake, Controller, and Renderer. Main creates a Controller, a Game, which stores a Snake object as part of the state, and a Renderer object. Main calls Game::Run to start the game main loop where Controller::HandleInput, Game::Update and Render::Render are called inside a while loop in order to run and bring this game to life.

The new functionalities added to this project have been to add new mode games to the basic one. Each time the snake feeds, the mode of the game is randomly set to a different one. The game will remain in the new mode until a time delay is reached, after that the game will switch to the basic game mode. There are four different new game modes:
  1. Speed Up Mode: the speed of the snake is multiplied by 1.5.
  2. Speed Down Mode: the speed of the snake is divideb by 0.5.
  3. Inverted Control Mode: in this mode, the controls are inverted so up control would be down and viceversa, and left would be right control and viceversa.
  4. Solid Boder Mode: the snake can not go off the screen window.

# Rubric satisfied
- All related README rubric points are addressed in the project.
- This project complies and runs.
- The project demonstrates an understanding of C++ functions and control structures.
- The project uses Object Oriented Programming techniques.
- Class members and functions use appropriate access specifiers.
- Initialization lists for initializing class members have been modified and used.
- The project makes use of references in function declarations.
- The project uses multithreading.
- A promise and future is used in the project.
- A mutex or lock is used in the project.  

## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
