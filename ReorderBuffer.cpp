#include "ReorderBuffer.h"

// The ReorderBuffer() function is used to construct a ReorderBufferobject.
// Precondition: Object of ReorderBuffer created.
// Postcondition: Default object created.
ReorderBuffer :: ReorderBuffer()
{}

// The ~ReorderBuffer() function is used to destruct a ReorderBuffer object.
// Precondition: Object of ReorderBuffer was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
ReorderBuffer::~ReorderBuffer()
{}

// The set_ROB_element() function is used to push an instruction onto the ROB.
// Precondition: The binary string representing the instruction and integer for the instruction's PC are passed as parameters.
// Postcondition: new element of ROB deque pushed to back with valid set to 0 (invalid), inst set to the binary instruction string, and ROB_ID set to the PC
void ReorderBuffer::set_ROB_element(string inst, int PC_num)
{
  //create new ROB_element in the ROB queue, set to invalid, set binary string, and PC number
  ROB.push_back(ROB_element(0, inst, PC_num));
}
