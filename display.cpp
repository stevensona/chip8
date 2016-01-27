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
  for(auto i = 0; i < 2048; i++) {
    memory[i] = false;
  }
  SDL_RenderPresent(renderer);
}

uint8_t Display::blit(uint8_t* src, uint8_t size, uint8_t x, uint8_t y) {
  bool pixel_cleared = false;
  cout << "drawing ";
  for(auto i = 0; i < size; i++) {

    cout << hex << unsigned(src[i]) << " ";
    for(auto b = 0; b < 8; b++) {
      const auto existing = memory[(y + i) * 64 + x + b];
      pixel_cleared = pixel_cleared || (existing && ((src[i] >> (7 - b)) & 0x1));
      memory[(y + i) * 64 + x + b] = existing != ((src[i] >> (7 - b)) & 0x1);
    }

  }
  cout << endl;
  update();
  return pixel_cleared ? 1 : 0;
}
