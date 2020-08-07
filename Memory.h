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

// A class that initializes memory (specify size in an overload constructor); initialize contents -
// read in csv file and create instruction memory. Note if you read in your csv contents as a string,
// then you will first need convert string to unsigned integer; fetch from memory using an input address (PC);
class Memory
{
public:
  Memory();
  ~Memory();

protected:
  void read_csv(string file_name);
  vector<unsigned int> get_csv_contents();
  vector<unsigned int> csv_contents;
};
