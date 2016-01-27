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
      r.x = (i % 64) * 8;
      r.y = i / 64 * 8;
      r.w = 7;
      r.h = 7;
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
  //This method needs to be refactored
  bool pixel_cleared = false;
  for(auto i = 0; i < size; i++) {
    for(auto b = 0; b < 8; b++) {
      size_t pos = (y + i) * 64 + x + b;
      const auto dest = memory[pos];
      const auto source = (src[i] >> (7 - b)) & 0x1; 
      pixel_cleared = pixel_cleared || (dest && source);
      memory.set(pos, dest != source);
    }
  }
  update();
  return pixel_cleared ? 1 : 0;
}
