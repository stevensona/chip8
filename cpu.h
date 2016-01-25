#include <string>

class cpu {

public:
	cpu();
	void reset();		
	void loadProgram(const std::string &filename);
	void run();
	void dumpMemory(const std::ostream &stream);

private:	

	uint8_t memory[4096];
	uint16_t pc;
	uint8_t v[16];
	uint16_t I;
	

	/* 
	pushing to the stack causes the value of the stack
	at the current stack pointer to be set, and the stack pointer 
	to shift 
	*/
	uint8_t stack_pointer; //offset from 0xEA0. acceptable values 0-F
	void push(uint8_t);
	uint16_t pop();

};
