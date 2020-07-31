#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;

// A class that contains all information required of an instruction -- decodes the instruction;
// converts instruction from unsigned int to binary; provides various accessor functions for all
// parameters of a given instruction (i.e. get src1, get src2, get operation, get immediate etc)
class Instruction
{

};

// A class that initializes memory (specify size in an overload constructor); initialize contents -
// read in csv file and create instruction memory. Note if you read in your csv contents as a string,
// then you will first need convert string to unsigned integer; fetch from memory using an input address (PC);
class Memory
{

};

// A class that implements the commit, execute and fetch functions, such that main()
// simply instantiates the pipeline class and any necessary structures, and then continuously
// calls pipeline fetch, execute and commit stages until the program has finished executing.
class Pipeline
{
public:
  void Fetch();
  void Execute();
  void Commit();

};

// a class that implements the IQ. It instantiate a (finite) queue; inserts an instruction into queue;
// check if instruction is ready for execution; printIQ() for debugging; delete an entry of the IQ during read and execute;
class IQueue
{

};

// A class that implements the IQ. It instantiate a (finite) queue; inserts an instruction into queue;
// check if instruction is ready for execution; printIQ() for debugging; delete an entry of the IQ during read
// and execute;
class Execute
{

};

//a class that calls the memory class to i) initialize fetch (i.e. initialize the memory) to enforce encapsulation of instruction memory, and to ii) fetch and instruction from memory with the PC parameter passed
class FetchUnit
{

};

//a class that maintains the register file; setup the register file (RF) such that all register locations are associated with i) data value and ii) a status bit. The status bit indicates whether the value in the register is valid for reading or is awaiting an instruction’s result. The register # as shown in the table below is solely used to index a register. More details on the register file system is provided in a subsequent section entitled “Register File FYI”;  accessor and mutator functions should be implemented for setting or obtaining the value and status of a register.
class RegisterFile
{

};

//STL structure used to maintain a list in the pipeline, such that the instructions are inserted and retired inorder. Instructions are inserted at fetch, removed at commit when valid. An example of the contents in the ROB are shown below.
struct ReorderBuffer //????????????????????????
{

};

//a class used to track and calculate latency and throughput statistics of the running program. Statistics are provided to the user via standard output at the end of a program’s execution.
class Statistics
{

};

//this class is used to provide all classes with any helper functions or global variables shared across the classes.
class HelperFunctions
{

};


int main()
{
  /*
  • initializes necessary pipeline structures, including a Pipeline class instance
  • prompts user for program to execute
  • calls the pipeline class to run the member functions commit(..) execute (…) and fetch(..)
  • We will assume that if further instructions exist in the ROB, the program has not finished
    executing. Therefore, if the ROB is empty, the program has finished executing in the context of
    this project
  • Once the program has finished executing, the latency and throughput stats are provided to the
    user
  • Loop again to prompt the user for another program to execute and re-initialize structures to
    provide new stats for the next program. Exit if no further programs are to be executed.
  */
  cout << "Hello world";

}
