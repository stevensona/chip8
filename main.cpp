#include <iostream>
#include <string>
#include "cpu.h"

int main(int argc, char** argv) {
  cpu chip8;
  if (argc < 2) {
    std::cout << "no input file specified\n";
    return -1;
  }
  chip8.loadProgram(std::string(argv[1]));
  for(auto i = 0; i < 20; i++) {
    chip8.debugInfo(std::cout);
    chip8.step();

  }
}
