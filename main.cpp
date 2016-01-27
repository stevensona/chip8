#include <iostream>
#include <string>
#include <SDL2/SDL.h>

#include "cpu.h"

int main(int argc, char** argv) {


  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init error: " << SDL_GetError() << '\n';
    return -1;
  }
  SDL_Quit();

  Cpu chip8;
  if (argc < 2) {
    std::cout << "no input file specified\n";
    return -1;
  }
  chip8.loadProgram(std::string(argv[1]));
  chip8.run(false);
}
