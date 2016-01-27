#include <string>

class Cpu {

public:
  Cpu();
  void reset();   
  void loadProgram(const std::string &filename);
  void run();
  void step();
  void dumpMemory();
  void dumpRegisters();

private:  

  static const uint16_t MEMORY_SIZE = 4096;
  static const uint16_t REGISTER_COUNT = 16;
  static const uint16_t STACK_BEGIN = 0xEA0;

  uint8_t memory[MEMORY_SIZE];
  uint16_t pc;
  uint16_t ir;
  uint8_t v[REGISTER_COUNT];
  uint16_t I;
  
  uint8_t stack_pointer; //offset from 0xEA0. acceptable values 0-F
  void push(uint16_t);
  uint16_t pop();
  void fetch();
  uint16_t getDWord(uint16_t);
  void decode_failure(uint16_t);

};
