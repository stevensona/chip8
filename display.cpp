#include <iostream>

#include "display.h"

using namespace std;

Display::Display() {
  window = SDL_CreateWindow(
    "chip8",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    800, 600, SDL_WINDOW_SHOWN 
  );
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

}

Display::~Display() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Display::update() {
  SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

  for(auto i = 0; i < 2048; i++) {
    if(memory[i]) {
      SDL_Rect r;
      r.x = (i % 64) * 10;
      r.y = i / 64 * 10;
      r.w = 10;
      r.h = 10;
      SDL_RenderFillRect(renderer, &r);
    }
  }
  SDL_RenderPresent(renderer);
}

void Display::clear() {
  SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  memory.reset();
}

uint8_t Display::blit(uint8_t* src, uint8_t size, uint8_t x, uint8_t y) {
  bool pixel_cleared = false;
  for(auto h = 0; h < size; h++) {
    for(auto b = 0; b < 8; b++) {
      if((src[h] >> (7 - b)) & 0x1)
        pixel_cleared = setPixel(x + b, y + h) || pixel_cleared;
    }
  }
  update();
  return pixel_cleared ? 1 : 0;
}

bool Display::setPixel(int16_t x, int16_t y) {
  while(x > 63) x -= 64;
  while(x < 0) x += 64;
  while(y > 31) y -= 32;
  while(y < 0) y += 32;
  size_t pos = y * 64 + x;
  memory.flip(pos);
  return !memory[pos]; //return true if pixel caused one to erase
}