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
  screen_rect.x = 0;
  screen_rect.y = 0;
  screen_rect.w = SCREEN_WIDTH;
  screen_rect.h = SCREEN_HEIGHT;

}

Display::~Display() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Display::selfTest() {

}

void Display::update() {
  SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
  SDL_RenderClear(renderer);
  update(screen_rect); //redraw entire screen
}

void Display::update(const SDL_Rect &rect) { //"dirty rectangles" method

  //clear the rect to be updated
  SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
  SDL_Rect clear = {
    rect.x * PIXEL_SIZE,
    rect.y * PIXEL_SIZE,
    rect.w * PIXEL_SIZE,
    rect.h * PIXEL_SIZE
  };
  SDL_RenderFillRect(renderer, &clear);


  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  for(auto i = 0; i < rect.w * rect.h; i++) {

    uint16_t pos_x = rect.x + i % rect.w;
    uint16_t pos_y = (rect.y + i / rect.w) * SCREEN_WIDTH;
    
    uint16_t pos = pos_x + pos_y;

    if(memory[pos]) {
      SDL_Rect r = { //construct "pixel" rect
        rect.x * PIXEL_SIZE + (i % rect.w) * PIXEL_SIZE,
        rect.y * PIXEL_SIZE + i / rect.w * PIXEL_SIZE,
        PIXEL_SIZE,
        PIXEL_SIZE
      };
      SDL_RenderFillRect(renderer, &r);
    }
  }
  SDL_RenderPresent(renderer);
}

void Display::clear() {
  memory.reset();
  update();
}

uint8_t Display::blit(uint8_t* src, uint8_t size, uint8_t x, uint8_t y) {
  bool pixel_cleared = false;
  for(auto h = 0; h < size; h++) { //Number of bytes to draw
    auto pixels = src[h];
    for(auto b = 0; b < 8; b++) {  //Draw each bit in the byte
      if(pixels & 0x80)
        pixel_cleared = setPixel(x + b, y + h) || pixel_cleared;
      pixels = pixels << 1;
    }
  }

  //The "dirty rectangle" method is not used due to 
  //strange behavior on the Mac platform. I believe this can
  //be attributed to:
  // "The backbuffer should be considered invalidated after
  // each present" https://wiki.libsdl.org/SDL_RenderPresent#Remarks
  //If anyone knows anything more, please let me know
  //SDL_Rect dirty = {x, y, 8, size};
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
  return !memory[pos]; //return true if pixel caused an erase
}
