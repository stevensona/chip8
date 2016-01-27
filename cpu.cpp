#include <fstream>
#include "cpu.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>

using namespace std;
//TODO improve consistency of error handling/reporting
//TODO improve debug info function names

Cpu::Cpu() {}

void Cpu::reset() {
  pc = 0x200;
  ir = getDWord(pc);
  for(auto i = 0; i < 4096; i++) {
    memory[i] = 0x0;
  }
  for(auto i = 0; i < 16; i++) {
    v[i] = 0x0;
  } 
  stack_pointer = 0x0;
  I = 0x0;

  uint8_t fonts[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, 
    0x20, 0x60, 0x20, 0x20, 0x70, 
    0xF0, 0x10, 0xF0, 0x80, 0xF0, 
    0xF0, 0x10, 0xF0, 0x10, 0xF0, 
    0x90, 0x90, 0xF0, 0x10, 0x10, 
    0xF0, 0x80, 0xF0, 0x10, 0xF0, 
    0xF0, 0x80, 0xF0, 0x90, 0xF0, 
    0xF0, 0x10, 0x20, 0x40, 0x40, 
    0xF0, 0x90, 0xF0, 0x90, 0xF0, 
    0xF0, 0x90, 0xF0, 0x10, 0xF0, 
    0xF0, 0x90, 0xF0, 0x90, 0x90, 
    0xE0, 0x90, 0xE0, 0x90, 0xE0, 
    0xF0, 0x80, 0x80, 0x80, 0xF0, 
    0xE0, 0x90, 0x90, 0x90, 0xE0, 
    0xF0, 0x80, 0xF0, 0x80, 0xF0, 
    0xF0, 0x80, 0xF0, 0x80, 0x80 };

  memcpy(&memory, &fonts, 80); //place fonts in memory starting at 0x0000

}

void Cpu::loadProgram(const string& filename) {
  reset(); //puts the Cpu in a known state
  ifstream ifs(filename, ios::in | ios::binary);
  ifs.seekg(0, ifs.end);
  streampos size = ifs.tellg();
  ifs.seekg(0, ifs.beg);
  ifs.read((char*)&memory[pc], size);
  cout << "read " << size << " bytes\n";
  ifs.close();
}

void Cpu::dumpMemory() {
  //for(auto w = 0; w < )
  //TODO
}

void Cpu::dumpRegisters() {
  cout << "pc: " << pc << ' ';
  cout << "ir: " << ir << ' ';
  cout << "I: " << I << '\n';
  for(auto i = 0; i < REGISTER_COUNT; i++) {
    cout  << "v" << i << ": " << setw(4) << unsigned(v[i]) << '\n';
  }
  cout << endl;
}

void Cpu::decode_failure(uint16_t instruction) {
  ostringstream oss;
  oss << hex << "unknown instruction: " << instruction;
  throw runtime_error(oss.str());
}

void Cpu::step() {
  fetch();
  const uint8_t msb = ir >> 12;
  const uint8_t lsb = ir & 0xF;

  const uint8_t x = (ir & 0xF00) >> 8;
  const uint8_t y = (ir & 0xF0) >> 4;
  const uint8_t kk = ir & 0xFF;
  const uint16_t nnn = ir & 0xFFF;
  const uint8_t Vx = v[x];
  const uint8_t Vy = v[y];

  //for displaying values
  auto d_x = unsigned(x);
  auto d_y = unsigned(y);
  auto d_kk = unsigned(kk);
  auto d_nnn = unsigned(nnn);

  cout << hex << setw(4) << pc << ' ' << setw(4) << ir << ' ';
  switch(msb) {
    case 0x0:
      switch(kk) {
        case 0xE0: //TODO clear the screen
          cout << hex << "CLS * * * * * TODO\n";
          pc += 2;
          break;
        case 0xEE:
          cout << hex << "RET\n";
          pc = pop();
          break;
        default:
          decode_failure(ir);
          break;
      }
      break;
    case 0x1: 
      cout << hex << "JP " << d_nnn << '\n';
      pc = nnn;
      break;
    case 0x2:
      cout << hex << "CALL " << d_nnn << '\n';
      push(pc + 2);
      pc = nnn;
      break;
    case 0x3:
      cout << hex << "SE V" << d_x << ", " << d_kk << '\n';
      pc += Vx == kk ? 4 : 2;
      break;
    case 0x4:
      cout << hex << "SNE V" << d_x << ", " << d_kk << '\n';
      pc += Vx != kk ? 4 : 2;
      break;
    case 0x5:
      cout << hex << "SE V" << d_x << ", V" << d_y << '\n';
      pc += Vx == Vy ? 4 : 2;
      break;
    case 0x6: 
      cout << hex << "LD V" << d_x << ", " << d_kk << '\n';
      v[x] = kk;
      pc += 2;
      break;
    case 0x7:
      cout << hex << "ADD V" << d_x << ", " << d_kk << '\n';
      v[x] += kk;
      pc += 2;
      break;
    case 0x8: {

      switch(lsb) {
        case 1:
          cout << hex << "OR V" << d_x << ", V" << d_y << '\n';
          v[x] = Vx | Vy;
          break;
        case 2:
          cout << hex << "AND V" << d_x << ", V" << d_y << '\n';
          v[x] = Vx & Vy;
          break;
        default:
          decode_failure(ir);
          break;
      }
      pc += 2;
      break;
    }
    case 0xA: 
      cout << hex << "LD I, " << d_nnn << '\n';
      I = nnn;
      pc += 2;
      break;
    case 0xD:
      //TODO draw a sprite
      cout << hex << "DRW * * * * * TODO\n";
      pc += 2;
      break;
    case 0xF:
      switch(kk) {
        case 0x29:
          cout << hex << "LD F, V" << d_x << '\n';
          I = Vx * 5;
          break;
        case 0x55:
          cout << hex << "LD [i], V" << d_x << '\n';
          for(auto i = 0; i <= x; i++) {
            memory[I + i] = v[i];
          }
          break;
        case 0x1E:
          cout << hex << "ADD I, V" << d_x << '\n';
          I += Vx;
          break;
        case 0x65:
          cout << hex << "LD V" << d_x << ", [I]\n";
          for(auto i = 0; i <= x; i++) {
            v[i] = memory[I + i];
          }
          break;
        default:
          decode_failure(ir);
          break;
      }
      pc += 2;
      break;

    default:
      decode_failure(ir);
      break;
  }
}

void Cpu::run() {
  bool running = true;
  while(running) {
    step();
  }
}

void Cpu::push(uint16_t value) {
  stack_pointer += 2;
  if (stack_pointer > 0x1F) //overflow the stack
    stack_pointer = 0x0;
  memory[STACK_BEGIN + stack_pointer] = (value & 0xFF00) >> 8;
  memory[STACK_BEGIN + stack_pointer + 1] = value & 0xFF;
}

uint16_t Cpu::pop() {
  uint16_t top = getDWord(STACK_BEGIN + stack_pointer);
  stack_pointer -= 2;
  if (stack_pointer > 0x1F) //handle underflow
    stack_pointer = 0x1F;
  return top;
}

void Cpu::fetch() {
  ir = getDWord(pc);
}

uint16_t Cpu::getDWord(uint16_t addr) {
  return (static_cast<uint16_t>(memory[addr]) << 8) | memory[addr + 1];
}


