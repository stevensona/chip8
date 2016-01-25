#include <ifstream>
#include "cpu.h"

cpu::cpu() {}

void cpu::reset() {
  pc = 0x200;
  for(auto i = 0; i < 4096; i++) {
    memory[i] = 0x0;
  }
  for(auto i = 0; i < 16; i++) {
    v[i] = 0x0;
  } 
  stack_pointer = 0x0;

  //TODO set up fonts
}

void cpu::loadProgram(const std::string& filename) {
  reset(); //puts the cpu in a known state
  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  char c = ifs.get();
  uint16_t offset = 0x0;
  while(ifs.good()) {
    memory[pc + offset] = c;
    offset += 1;
    c = ifs.get();
  }
  ifs.close();
}

void cpu::dumpMemory(const std::ostream &stream) {
  //for(auto w = 0; w < )
  //TODO
}


void cpu::run() {
  bool running = true;
  while(running) {
    uint16_t instruction = (static_cast<uint16_t>memory[pc] << 8) | memory[pc + 1];
    switch((instruction & 0xF000) >> 12) {
      case 0x0:
        switch(instruction & 0xFF) {
          case 0xE0: //TODO clear the screen
            break;
          case 0xEE:
            pc = pop();
            break;
        }
        break;
      case 0x8:
        uint8_t x = (instruction & 0x0F00) >> 8;
        uint8_t y = (instruction & 0x00F0) >> 4;
        switch(instruction & 0x000F) {

        }
      default:
        //throw exception
        break;
    }

  }
}

void cpu::push(uint16_t value) {
  stack_pointer += 2;
  if (stack_pointer > 0x1F) //overflow the stack
    stack_pointer = 0x0;
  memory[0xEA0 + stack_pointer] = value & 0xFF00 >> 8;
  memory[0xEA0 + stack_pointer + 1] = value & 0xFF;
}

uint16_t cpu::pop() {
  uint16_t addr = (static_cast<uint16_t>memory[stack_pointer] << 8) | memory[stack_pointer + 1];
  stack_pointer -= 2;
  if (stack_pointer > 0x1F) //handle underflow
    stack_pointer = 0x1F;
  return addr;
}
