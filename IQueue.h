#include "Instruction.h"
#include "RegisterFile.h"

struct IQ
{

  IQ() : opcode_IQ(), src1_IQ(), src1_valid(), src2_IQ(), src2_valid(), immediate_IQ(), dest_IQ(), ROB_ID_IQ(), type_IQ() {} //default constructor
  IQ(string op, int sr1, int val1, int sr2, int val2, int immed, int des, int num, string type_given) : opcode_IQ(op), src1_IQ(sr1), src1_valid(val1), src2_IQ(sr2), src2_valid(val2), immediate_IQ(immed), dest_IQ(des), ROB_ID_IQ(num), type_IQ(type_given){} // Does all types

  string opcode_IQ;
  int src1_IQ;
  int src1_valid;
  int src2_IQ;
  int src2_valid;
  int immediate_IQ;
  int dest_IQ;
  int ROB_ID_IQ;
  string type_IQ;
};

// a class that implements the IQ. It instantiate a (finite) queue; inserts an instruction into queue;
// check if instruction is ready for execution; printIQ() for debugging; delete an entry of the IQ during read and execute;
class IQueue : public Instruction, public RegisterFile
{
public:
  IQueue();
  ~IQueue();

protected:
  deque<IQ> The_Queue; // deque used to manually force FIFO and allow for use of []
  void set_IQ_element(int PC_num);
};
