#include <fstream>
#include "cpu.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>

using namespace std;
//TODO improve consistency of error handling/reporting
//TODO improve debug info function names

Cpu::Cpu(Display& d):
  memory(MEMORY_SIZE),
  v(REGISTER_COUNT),
  stack(STACK_SIZE),
  keys(16) {
  display = &d;
}

void Cpu::reset() {
  pc = 0x200;
  ir = getDWord(pc);
  fill(memory.begin(), memory.end(), 0x0); //zero all memory
  fill(v.begin(), v.end(), 0x0); //zero all registers
  fill(stack.begin(), stack.end(), 0x0); //zero the stack
  fill(keys.begin(), keys.end(), false); //zero the keypad
  sp = 0;
  I = 0x0;

  vector<uint8_t> fonts = {
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
    0xF0, 0x80, 0xF0, 0x80, 0x80
  };

  copy(fonts.begin(), fonts.end(), memory.begin()); //place fonts in memory
  wait_for_key = false;
  wait_for_key_reg = 0;
  ST = 0;
  DT = 0;
}

void Cpu::loadProgram(const string& filename) {
  reset(); //puts the Cpu in a known state

  ifstream is(filename, ios::in | ios::binary);
  vector<char> contents(
    (istreambuf_iterator<char>(is)),
    istreambuf_iterator<char>()
  );
  is.close();
  copy(contents.begin(), contents.end(), memory.begin() + pc);
}

void Cpu::dumpMemory() {
  //for(auto w = 0; w < )
  //TODO
}

void Cpu::dumpRegisters() {
  cout << hex << "pc: " << pc << ' ';
  cout << hex << "ir: " << ir << ' ';
  cout << hex << "I: " << I << '\n';
  for(auto i = 0; i < REGISTER_COUNT; i++) {
    cout << hex  << "v" << i << ": " << setw(4) << unsigned(v[i]) << '\n';
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

  cout << hex << setw(4) << unsigned(pc) << ' ' <<
   setw(4) << unsigned(ir) << ' ' <<
   setw(2) << unsigned(sp) << ' ';
  //TODO inconsistent matching format
  switch(msb) {
    case 0x0:
      switch(kk) {
        case 0xE0: //TODO clear the screen
          cout << hex << "CLS\n";
          display->clear();
          pc += 2;
          break;
        case 0xEE:
          cout << hex << "RET\n";
          pc = stack[--sp];
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
      stack[sp++] = pc + 2;
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
    case 0x8:
      switch(lsb) {
        case 0x0:
          cout << hex << "LD V" << d_x << ", V" << d_y << '\n';
          v[x] = v[y];
          break;
        case 0x1:
          cout << hex << "OR V" << d_x << ", V" << d_y << '\n';
          v[x] = Vx | Vy;
          break;
        case 0x2:
          cout << hex << "AND V" << d_x << ", V" << d_y << '\n';
          v[x] = Vx & Vy;
          break;
        case 0x3:
          cout << hex << "XOR V" << d_x << ", V" << d_y << '\n';
          v[x] = Vx != Vy;
          break;
        case 0x4:
          cout << hex << "ADD V" << d_x << ", V" << d_y << '\n';
          //might be a better way to do this
          {
            auto x16 = static_cast<uint16_t>(v[x]);
            auto y16 = static_cast<uint16_t>(v[y]);
            v[0xF] = x16 + y16 > 255 ? 1 : 0;
          }
          v[x] = v[x] + v[y];
          break;
        case 0x5:
          cout << hex << "SUB V" << d_x << ", V" << d_y << '\n';
          v[0xF] = Vx > Vy ? 1 : 0;
          v[x] = v[x] - v[y];
          break;
        case 0x6:
          cout << hex << "SHR V" << d_x << ", {V" << d_y << "}\n";
          v[0xF] = lsb == 0x1 ? 1 : 0;
          v[x] = v[x] >> 1;
          break;
    case 0x7:
          cout << hex << "SUBN V" << d_x << ", V" << d_y << '\n';
          v[0xF] = Vy > Vx ? 1 : 0;
          v[x] = v[y] - v[x];
          break;
    case 0xE:
          cout << hex << "SHL V" << d_x << ", {V" << d_y << "}\n";
          v[0xF] = msb == 0x1 ? 1 : 0;
          v[x] = v[x] << 1;
          break;

        default:
          decode_failure(ir);
          break;
      }
      pc += 2;
      break;
    case 0x9:
      cout << hex << "SNE V" << d_x << ", V" << d_y << '\n';
      pc += Vx != Vy ? 4 : 2;
      break;
    case 0xA:
      cout << hex << "LD I, " << d_nnn << '\n';
      I = nnn;
      pc += 2;
      break;
    case 0xC:
      cout << hex << "RND V" << d_x << ", " << d_kk << '\n';
      v[x] = (rnd() & 0xFF) & kk;
      pc += 2;
      break;
    case 0xD:
      cout << hex << "DRW V" << d_x << ", V" << d_y << ", size\n";
      v[0xF] = display->blit(&memory[I], lsb, Vx, Vy);
      pc += 2;
      break;
    case 0xE:
      switch(kk) {
        case 0x9E:
          cout << hex << "SKP V" << d_x << '\n';
          pc += keys[Vx] ? 4 : 2;
          break;
        case 0xA1:
          cout << hex << "SKNP V" << d_x << '\n';
          pc += !keys[Vx] ? 4 : 2;
          break;
        default:
          decode_failure(ir);
          break;
      }
      break;

    case 0xF:

      switch(kk) {
        case 0x0A:
          cout << hex << "LD V" << d_x << ", K\n";
          wait_for_key = true;
          wait_for_key_reg = Vx;
          break;
        case 0x07:
          cout << hex << "LD V" << d_x << ", DT\n";
          v[x] = DT;
          break;
        case 0x15:
          cout << hex << "LD DT, V" << d_x << '\n';
          DT = Vx;
          break;
        case 0x18:
          cout << hex << "LD ST, V" << d_x << '\n';
          ST = Vx;
          break;
        case 0x29:
          cout << hex << "LD F, V" << d_x << '\n';
          I = Vx * 5;
          break;
        case 0x33:
          cout << hex << "LD B, V" << d_x << '\n';
          memory[I] = Vx % 1000 / 100;
          memory[I + 1] = Vx % 100 / 10;
          memory[I + 2] = Vx % 10;
          break;
        case 0x55:
          cout << hex << "LD [i], V" << d_x << '\n';
          for(auto i = 0; i <= x; i++)
            memory[I + i] = v[i];
          break;
        case 0x1E:
          cout << hex << "ADD I, V" << d_x << '\n';
          I += Vx;
          break;
        case 0x65:
          cout << hex << "LD V" << d_x << ", [I]\n";
          for(auto i = 0; i <= x; i++)
            v[i] = memory[I + i];
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

void Cpu::tickTimers() {
  if(DT > 0) DT -= 1;
  if(ST > 0) ST -= 1;
}

void Cpu::fetch() {
  ir = getDWord(pc);
}

uint16_t Cpu::getDWord(uint16_t addr) {
  return (static_cast<uint16_t>(memory[addr]) << 8) | memory[addr + 1];
}

bool Cpu::waitingForKey() {
  return wait_for_key;
}

void Cpu::pressKey(const uint8_t key) {
  keys[key] = true;

  if(wait_for_key) {
    v[wait_for_key_reg] = key;
    wait_for_key = false;
  }

}

void Cpu::releaseKey(const uint8_t key) {
  keys[key] = false;
}
