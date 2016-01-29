#include <iostream>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include "display.h"
#include "cpu.h"

using namespace std;

int main(int argc, char** argv) {

  const int SPEED = 500; //Hz

  if (argc < 2) {
    cout << "no input file specified\n";
    return -1;
  }


  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    cout << "SDL_Init error: " << SDL_GetError() << '\n';
    return -1;
  }
  Display display;

  display.clear();

  Cpu chip8(display);
  chip8.loadProgram(std::string(argv[1]));

  map<SDL_Keycode, uint8_t> keymap;
  keymap.insert({SDLK_1, 0x1});
  keymap.insert({SDLK_2, 0x2});
  keymap.insert({SDLK_3, 0x3});
  keymap.insert({SDLK_4, 0xC});
  keymap.insert({SDLK_q, 0x4});
  keymap.insert({SDLK_w, 0x5});
  keymap.insert({SDLK_e, 0x6});
  keymap.insert({SDLK_r, 0xD});
  keymap.insert({SDLK_a, 0x7});
  keymap.insert({SDLK_s, 0x8});
  keymap.insert({SDLK_d, 0x9});
  keymap.insert({SDLK_f, 0xE});
  keymap.insert({SDLK_z, 0xA});
  keymap.insert({SDLK_x, 0x0});
  keymap.insert({SDLK_c, 0xB});
  keymap.insert({SDLK_v, 0xF});

  SDL_Event e;
  bool running = true;
  while(running) {
    while(SDL_PollEvent(&e)) {
      switch(e.type) {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          {
            const auto key = e.key.keysym.sym;
            if(key == SDLK_ESCAPE) running = false;
            if(keymap.find(key) != keymap.end()) chip8.pressKey(keymap[key]);
          }
          break;
        case SDL_KEYUP:
          {
            const auto key = e.key.keysym.sym;
            if(keymap.find(key) != keymap.end())
              chip8.releaseKey(keymap[key]);
          }
          break;
      }
    }

    if(!chip8.waitingForKey()) {
      chip8.step();
    }

    //TODO run *AT* 500Hz with the option to speed up or slow down
    SDL_Delay(1000 / SPEED); //run at "slightly less than 500Hz"

  }
  SDL_Quit();
}
