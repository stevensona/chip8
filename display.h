
#pragma once
#include <SDL2/SDL.h>

class Display {

public:
  Display();
  ~Display();
  void update();
  void clear();

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool memory[2048];

};