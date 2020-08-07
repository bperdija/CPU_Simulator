#include "RegisterFile.h"

// The RegisterFile() function is used to construct a RegisterFile object.
// Precondition: Object of RegisterFile created.
// Postcondition: The r vector size is set to REGISTER_SIZE and each element in r has data set to 0 and validity set to 1 (valid).
RegisterFile::RegisterFile()
{
  // preset register size to control size and allow use of []
  r.resize(REGISTER_SIZE);

  // set all elements in the register to a value of zero and valid
  for (int i = 0; i <= REGISTER_SIZE; i++)
  {
    r[i].data = 0;
    r[i].validity = 1;
  }
}

// The ~RegisterFile() function is used to destruct a RegisterFile object.
// Precondition: Object of RegisterFIle was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
RegisterFile::~RegisterFile()
{}
