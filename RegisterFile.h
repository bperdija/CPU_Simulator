#ifndef GENERAL_LIBS_DEFS
#define GENERAL_LIBS_DEFS

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;
#endif
#define REGISTER_SIZE 15

struct Register_File
{
  Register_File() : data(), validity() {} //default constructor
  Register_File(int data_given, int valid) : data(data_given), validity(valid) {} //overlaoded constructor

  int data;
  int validity;
};

//a class that maintains the register file; setup the register file (RF) such that all register locations are associated with i) data value and ii) a status bit. The status bit indicates whether the value in the register is valid for reading or is awaiting an instruction’s result. The register # as shown in the table below is solely used to index a register. More details on the register file system is provided in a subsequent section entitled “Register File FYI”;  accessor and mutator functions should be implemented for setting or obtaining the value and status of a register.
class RegisterFile
{
public:
  RegisterFile();
  ~RegisterFile();

protected:
  vector<Register_File> r; // vector used to allow for use of [] 
};
