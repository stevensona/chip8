#include <fstream>
#include "cpu.h"
#include <iostream>

cpu::cpu() {}

void cpu::reset() {
  pc = 0x200;
  for(auto i = 0; i < 4096; i++) {
    memory[i] = 0x0;
  }
  for(auto i = 0; i < 16; i++) {
    v[i] = 0xAA;
  } 
  stack_pointer = 0x0;
  I = 0x0;
  //TODO set up fonts
}

void cpu::loadProgram(const std::string& filename) {
  reset(); //puts the cpu in a known state
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  ifs.seekg(0, ifs.end);
  std::streampos size = ifs.tellg();
  ifs.seekg(0, ifs.beg);
  ifs.read((char*)&memory[pc], size);
  std::cout << "read " << size << " bytes\n";
  ifs.close();
}

void cpu::dumpMemory(std::ostream &stream) {
  //for(auto w = 0; w < )
  //TODO
}

void cpu::debugInfo(std::ostream &stream) {
  stream << "pc: " << std::hex << pc << ' ';
  stream << "current: " << std::hex << getDWord(pc) << ' ';
  stream << "I: " << std::hex << I << std::endl;
  for(auto i = 0; i < 16; i++) {
    stream << "v" << i << ": " << std::hex << static_cast<uint16_t>(v[i]) << ' ';
  }
  stream << std::endl;
}

void cpu::step() {
  uint16_t instruction = getDWord(pc);
  switch(instruction >> 12) {
    case 0x0:
      switch(instruction & 0xFF) {
        case 0xE0: //TODO clear the screen
          pc += 2;
          break;
        case 0xEE:
          pc = pop();
          break;
        default:
          std::cout << "halting on unknown instruction: " << std::hex << getDWord(pc) << '\n';
          exit(-1);
          break;
      }
      break;
    case 0x1: 
      std::cout << "jumping to " << std::hex << (instruction & 0xFFF) << '\n';
      pc = instruction & 0xFFF;
      break;
    case 0x2:
      std::cout << "doing sub " << std::hex << (instruction & 0xfff) << '\n';
      push(pc);
      pc = instruction & 0xFFF;
      break;
    case 0x3:
      std::cout << "skipping if " << std::hex << (instruction & 0xFF) << "=v[" <<
        ((instruction & 0xFFF) >> 8) << "]\n";
      pc += ((instruction & 0xFF) == v[(instruction & 0xFFF) >> 8]) ? 4 : 2;
      break;
    case 0x4:
      std::cout << "skipping if " << std::hex << (instruction & 0xFF) << "!=v[" <<
        ((instruction & 0xFFF) >> 8) << "]\n";
      pc += ((instruction & 0xFF) == v[(instruction & 0xFFF) >> 8]) ? 2 : 4;
      break;
    case 0x6: 
      std::cout << "moving " << std::hex << (instruction & 0xFF) << " to v[" <<
        ((instruction & 0xFFF) >> 8) << "]\n";
      v[(instruction & 0xFFF) >> 8] = instruction & 0xFF;
      pc += 2;
      break;
    case 0x8: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;
      switch(instruction & 0x000F) {
      }
      break;
    }
    case 0xA: 
      I = instruction & 0xFFF;
      pc += 2;
      break;
    case 0xF:
      switch(instruction & 0xFF) {
        case 0x55:
          for(auto i = 0; i <= (instruction & 0xFFF) >> 8; i++) {
            memory[I + i] = v[(instruction & 0xFFF) >> 8];
          }
          break;
        case 0x1E:
          I += v[(instruction & 0xFFF) >> 8];
          break;
        case 0x65:
          for(auto i = 0; i <= (instruction & 0xFFF) >> 8; i++) {
            v[(instruction & 0xFFF) >> 8] = memory[I + i];
          }
          break;
        default:
          std::cout << "halting on unknown instruction: " << std::hex << getDWord(pc) << '\n';
          exit(-1);
          break;
      }
      pc += 2;
      break;

    default:
      //thriow exception
      std::cout << "halting on unknown instruction: " << std::hex << getDWord(pc) << '\n';
      exit(-1);
      break;
  }
}

void cpu::run() {
  bool running = true;
  while(running) {
    step();
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
  uint16_t top = getDWord(stack_pointer);
  stack_pointer -= 2;
  if (stack_pointer > 0x1F) //handle underflow
    stack_pointer = 0x1F;
  return top;
}

uint8_t cpu::getWord(uint16_t addr) {
  return memory[addr];
}

uint16_t cpu::getDWord(uint16_t addr) {
  return (static_cast<uint16_t>(memory[addr]) << 8) | memory[addr + 1];
}


