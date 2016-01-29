#include <iostream>

#include "display.h"

using namespace std;

Display::Display() {
  window = SDL_CreateWindow(
    "chip8",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE, SDL_WINDOW_SHOWN 
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

  for(auto i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
    if(memory[i]) {
      SDL_Rect r;
      r.x = (i % SCREEN_WIDTH) * PIXEL_SIZE;
      r.y = i / SCREEN_WIDTH * PIXEL_SIZE;
      r.w = PIXEL_SIZE;
      r.h = PIXEL_SIZE;
      SDL_RenderFillRect(renderer, &r);
    }
  }
  SDL_RenderPresent(renderer);
}

void Display::update(SDL_Rect &rect) {
  throw(runtime_error("not implemented"));
}

void Display::clear() {
  SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  memory.reset();
}

uint8_t Display::blit(uint8_t* src, uint8_t size, uint8_t x, uint8_t y) {
  bool pixel_cleared = false;
  for(auto h = 0; h < size; h++) { //Number of bytes to draw
    for(auto b = 0; b < 8; b++) {  //Draw each bit in the byte	
      if((src[h] >> (7 - b)) & 0x1)
		pixel_cleared = setPixel(x + b, y + h) || pixel_cleared;
    }
  }
  update();
  return pixel_cleared ? 1 : 0;
}

bool Display::setPixel(int16_t x, int16_t y) {
  while(x > SCREEN_WIDTH - 1) x -= SCREEN_WIDTH;
  while(x < 0) x += SCREEN_WIDTH;
  while(y > SCREEN_HEIGHT - 1) y -= SCREEN_HEIGHT;
  while(y < 0) y += SCREEN_HEIGHT;
  size_t pos = y * SCREEN_WIDTH + x;
  memory.flip(pos);
  return !memory[pos]; //return true if pixel caused one to erase
}
