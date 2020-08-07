#include "FethcUnit.h"
#include "IQueue.h"

// The FetchUnit() function is used to construct a FethcUnit object.
// Precondition: Object of FetchUnit created.
// Postcondition: Default object created.
FetchUnit::FetchUnit()
{}

// The ~FetchUnit() function is used to destruct a FetchUnit object.
// Precondition: Object of FetchUnit was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
FetchUnit::~FetchUnit()
{}

// The jtype() function is used to redirect fetch to the PC it should fetch next.
// Precondition: The Instruction class have been initialized.
// Postcondition: The value of the instruction's address is set to the next PC to fectch.
void FetchUnit::jtype()
{
  PC = address;
}

// The BNE() function is used to determine which PC the pipeine should fetch next when a BNE operation occurs.
// Precondition: The RegisterFile and Instruction classes have been initialized.
// Postcondition: If the destination value and src1 value are not equal, next fetch the PC equal to the immediate value. If equal, next fetch the next PC.
void FetchUnit::BNE()
{
  if (r[src1].data != r[dest].data)
  {
    PC = immediate;
  }

  else
  {
    PC++;
  }
}

// The BEQ() function is used to determine which PC the pipeine should fetch next when a BEQ operation occurs.
// Precondition: The RegisterFile and Instruction classes have been initialized.
// Postcondition: If the destination value and src1 value are equal, next fetch the PC equal to the immediate value. If not equal, next fetch the next PC.
void FetchUnit::BEQ()
{
  if (r[src1].data == r[dest].data)
  {
    PC = immediate;
  }

  else
  {
    PC++;
  }
}
