#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "display.h"
#include "cpu.h"

using namespace std; 

int main(int argc, char** argv) {

  if (argc < 2) {
    cout << "no input file specified\n";
    return -1;
  }

  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    cout << "SDL_Init error: " << SDL_GetError() << '\n';
    return -1;
  }
  cout << "making display\n";
  Display display;
  cout << "made display\n";

  display.clear();

  Cpu chip8(display);
  chip8.loadProgram(std::string(argv[1]));
  
  SDL_Event e;
  bool running = true;
  while(running) {
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) running = false;
    }
    chip8.step();

  }
  SDL_Quit();
}
