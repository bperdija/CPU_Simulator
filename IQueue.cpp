#include "IQueue.h"

// The IQueue() function is used to construct a IQueue object.
// Precondition: Object of IQueue created.
// Postcondition: Default object created.
IQueue::IQueue()
{}

// The ~IQueue() function is used to destruct a IQueue object.
// Precondition: Object of IQueue was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
IQueue::~IQueue()
{}

  // Precondition: The integer representing PC number passed as parameter, RegisterFile vector r has been initialized,  and the instruction has been decoded in decode() in Instruction Class.
  // Postcondition: If the type of instruction is r, i, or p, push instruction onto IQ using overloaded IQ constructor, if not print an error.
void IQueue::set_IQ_element(int PC_num)
{
  // if r-type, set opcode, src1, src2, dest, PC, and type, retrieve validity from register for src1 and src2, and set immediate to -1 as it isn't used
  if (type == "r")
  {
    The_Queue.push_back(IQ(opcode, src1, r[src1].validity, src2, r[src2].validity, -1, dest, PC_num, type)); //first constructor
  }

  // if i-type, set opcode, src1, immediate, dest, PC, and type, retrieve validity from register for src1, and set src2 to -1 and src2 valid to 1 as it isn't used and
  else if (type == "i")
  {
    The_Queue.push_back(IQ(opcode, src1, r[src1].validity, -1, 1, immediate, dest, PC_num, type)); //first constructor
  }

  // if p-type, set opcode, immediate, dest, PC, and type, and set src1/src2 to -1 and src1/src2 valid to 1 as they aren't used
  else if (type == "p")
  {
    The_Queue.push_back(IQ(opcode, -1, 1, -1, 1, immediate, dest, PC_num, type));//second constructor
  }

  // print error if the type is invalid to push onto The_Queue
  else
  {
    #ifdef DEBUG
    cout << "Error: Instruction could not be added to the IQueue." << endl;
    #endif
  }
}
