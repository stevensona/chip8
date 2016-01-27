#include <string>

class Cpu {

public:
  Cpu();
  void reset();   
  void loadProgram(const std::string &filename);
  void run(bool debug);
  void step();
  void dumpMemory(std::ostream &stream);
  void dumpRegisters(std::ostream &stream);

private:  

  static const int MEMORY_SIZE = 4096;
  static const int REGISTER_COUNT = 16;

  uint8_t memory[MEMORY_SIZE];
  uint16_t pc;
  uint16_t ir;
  uint8_t v[REGISTER_COUNT];
  uint16_t I;
  
  uint8_t stack_pointer; //offset from 0xEA0. acceptable values 0-F
  void push(uint16_t);
  uint16_t pop();

  uint8_t get_word(uint16_t);
  uint16_t get_dword(uint16_t);
  void decode_failure(uint16_t);

};
