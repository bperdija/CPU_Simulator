#include "Exceptions.h"

// The Exceptions() function is used to construct a default Exceptions object.
// Precondition: Object of Exceptions created.
// Postcondition: Default object created.
Exceptions::Exceptions()
{}

// The Exceptions(string) function is used to construct a Exceptions object.
// Precondition: Object of Exceptions created.
// Postcondition: Default object created.
Exceptions::Exceptions(string entered): message(entered)
{}

// The ~Execeptions() function is used to destruct a Exceptions object.
// Precondition: Object of Esceptions was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
Exceptions::~Exceptions()
{}

// The getMsg_parameter() function is used to return the error message.
// Precondition: The Exceptions class has been initialized.
// Postcondition: The message variable is returned.
string Exceptions::getMsg_parameter()
{
  return message;
}
