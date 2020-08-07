#include "Memory.h"
#include <bitset> //needed for bitset<size>(unsigned int).to_string()

// A class that contains all information required of an instruction -- decodes the instruction;
// converts instruction from unsigned int to binary; provides various accessor functions for all
// parameters of a given instruction (i.e. get src1, get src2, get operation, get immediate etc)
class Instruction : public Memory
{
public:
  Instruction();
  ~Instruction();

private:
  void decode_opcode(string code);
  int stringToInt(string code);

protected:
  void uintToBinary(unsigned int given);
  void decode(string binary_code);

  string binary_instruction;
  string type;  //bits 0 to 1: used in r, i, p, j
  string opcode;  //bits 2 to 5: used in r, p
  int dest;  //bits 6 to 10: used in r, i, p
  int src1;  //bits 11 to 15: used in r, i
  int src2;  //bits 16 to 20: used in r
  int immediate;  //bits 16 to 31: used in i
  int address;  //bits 2 to 31: used in j
};
