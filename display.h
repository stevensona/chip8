
#pragma once
#include <bitset>
#include <SDL2/SDL.h>

class Display {

public:
  Display();
  ~Display();
  void update();
  void clear();
  uint8_t blit(uint8_t* src, uint8_t size, uint8_t x, uint8_t y);

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  std::bitset<2048> memory;

  bool setPixel(int16_t x, int16_t y);
};