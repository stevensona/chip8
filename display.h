
#pragma once
#include <bitset>
#include <SDL2/SDL.h>

class Display {

public:
  Display();
  ~Display();
  void update();
  void update(SDL_Rect &rect);
  void clear();
  uint8_t blit(uint8_t* src, uint8_t size, uint8_t x, uint8_t y);

private:

  static const uint8_t SCREEN_WIDTH = 64;
  static const uint8_t SCREEN_HEIGHT = 32;
  static const uint8_t PIXEL_SIZE = 10;

  SDL_Window *window;
  SDL_Renderer *renderer;
  std::bitset<SCREEN_WIDTH * SCREEN_HEIGHT> memory;

  bool setPixel(int16_t x, int16_t y);
};
