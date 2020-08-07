#ifndef GENERAL_LIBS_DEFS
#define GENERAL_LIBS_DEFS

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>

#define EXECUTE
//#define DEBUG

using namespace std;
#endif

class Exceptions
{
public:
  Exceptions();
  Exceptions(string entered);
  ~Exceptions();
  string getMsg_parameter();

private:
  string message;
};
