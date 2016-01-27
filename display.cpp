#include "display.h"

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
      r.x = i % 64 * 4;
      r.y = i / 64 * 4;
      r.w = 4;
      r.h = 4;
      SDL_RenderFillRect(renderer, &r);
    }
  }
  SDL_RenderPresent(renderer);
  SDL_Delay(4000);

}

void Display::clear() {
  SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
  SDL_RenderClear(renderer);
  for(auto i = 0; i < 2048; i++) {
    memory[i] = false;
  }
  SDL_RenderPresent(renderer);
}