#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <typeinfo> // not needed
#include <cmath>
#include <bitset>

using namespace std;

// A class that initializes memory (specify size in an overload constructor); initialize contents -
// read in csv file and create instruction memory. Note if you read in your csv contents as a string,
// then you will first need convert string to unsigned integer; fetch from memory using an input address (PC);
class Memory
{
public:
  Memory();
  ~Memory();
  void menu(Memory obj);
  void read_csv(string file_name);
protected:
  list<unsigned int> csv_contents;
};

Memory::Memory(){}

Memory::~Memory(){}

void Memory::read_csv(string file_name)
{
  // variables
  string temp_element;

  // define empty lists to store values
  std::list<unsigned int> values;

  string file = file_name;

  ifstream myFile;
  myFile.open(file);

  if(myFile.is_open()) // while the file is open
  {
    while(!myFile.eof())
    {
      getline(myFile, temp_element, ',');
      //cout << "element as is: " << temp_element << endl;
      unsigned int temp = stoul((temp_element).c_str(), 0); // string to unsigned int with help from https://www.cplusplus.com/forum/beginner/148948/
      //cout << "unsigned int: " << temp << endl;
      // cout << typeid(temp).name() << endl; // the output for an unsigned integer is j. This confirms that we have successfully turned a string into an unsigned int.
      values.push_back(temp);
    }
  }

  csv_contents = values;


  for (auto i = values.begin(); i != values.end(); ++i)
  {
    std::cout << *i << ' ';
  }
  cout << "done" << endl;
}

// A class that contains all information required of an instruction -- decodes the instruction;
// converts instruction from unsigned int to binary; provides various accessor functions for all
// parameters of a given instruction (i.e. get src1, get src2, get operation, get immediate etc)
class Instruction
{
public:
  void uintToBinary(list<unsigned int> given);
  void decode(vector<string> binary);
  void decode_opcode(string code);
  //void decode_src1(string code);
  //void decode_src2(string code);
  //void decode_dest(string code);

  vector<string> binary_instructions;
  string type;//bits 0 to 1: used in r, i, p, j
  string opcode;//bits 2 to 5: used in r, p
  string dest;//bits 6 to 10: used in r, i, p
  string src1;//bits 11 to 15: used in r, i
  string src2;//bits 16 to 20: used in r
  int immediate;//bits 16 to 31: used in i
  int address;//bits 2 to 31: used in j
};

void Instruction::uintToBinary(list<unsigned int> given){
  list<unsigned int>::iterator it;

  for (it = given.begin(); it != given.end(); ++it){
    string temp_string = bitset<32>(*it).to_string();//convert uint to binary string

    binary_instructions.push_back(temp_string);
  }
}

void Instruction :: decode(vector<string> binary)
{
  vector<string>::iterator it;

  for (it = binary.begin(); it != binary.end(); ++it)
  {
    string type_bits = it -> substr(0,2);

    if (type_bits == "00")
    {//type r
      type = "r";
      string opcode_bits = it -> substr(2,4);
      decode_opcode(opcode_bits);
    }

    else if (type_bits == "01"){//type i
      type = "i";
      string opcode_bits = it -> substr(2,4);
      decode_opcode(opcode_bits);
    }

    else if (type_bits == "10"){//type j
      type = "j";

    }

    else if (type_bits == "11"){//type p
      type = "p";
      string opcode_bits = it -> substr(2,4);
      decode_opcode(opcode_bits);
    }

    else{
      cout << "Error: invalid instruction type." << endl;
    }
  }
}

void Instruction :: decode_opcode(string code)
{ //I haven't yet tested this file out

  if (code == "0000") // add
  {
    opcode = "add";
  }

  else if (code == "0001") // subtract
  {
    opcode = "subtract";
  }

  else if (code == "0011") // divide
  {
    opcode = "divide";
  }

  else if (code == "0100") // modulo
  {
    opcode = "modulo";
  }

  else if (code == "0101") // BEQ
  {
    opcode = "BEQ";
  }

  else if (code == "0110") // BNE (sub)
  {
    opcode = "BNE";
  }

  else if (code == "0111") // move
  {
    opcode = "move";
  }

  else if (code == "1000") // move again....
  {
    opcode = "move";
  }

  else
  {
    cout << "Error: Invalid opcode value." << endl;
  }
}

// A class that implements the commit, execute and fetch functions, such that main()
// simply instantiates the pipeline class and any necessary structures, and then continuously
// calls pipeline fetch, execute and commit stages until the program has finished executing.
class Pipeline : public Memory
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
public:


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
  Memory tester;
  //tester.read_csv("factorial.csv");
  //cout << "Hello world";

}
