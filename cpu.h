#include <string>
#include <random>
#include "display.h"

class Cpu {

public:
  Cpu(Display&);
  void reset();   
  void loadProgram(const std::string &filename);
  void step();
  void dumpMemory();
  void dumpRegisters();

private:  

  static const uint16_t MEMORY_SIZE = 4096;
  static const uint16_t REGISTER_COUNT = 16;
  static const uint16_t STACK_SIZE = 16;

  uint8_t memory[MEMORY_SIZE];
  uint16_t pc;
  uint16_t ir;
  uint8_t v[REGISTER_COUNT];
  uint16_t I;
  uint8_t sp;
  uint16_t stack[STACK_SIZE];

  Display *display;
  std::random_device rnd;
  


  void fetch();
  uint16_t getDWord(uint16_t);
  void decode_failure(uint16_t);

};
