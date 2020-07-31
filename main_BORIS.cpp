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
  list<unsigned int> get_csv_contents();
protected:
  list<unsigned int> csv_contents;
};

Memory::Memory()
{}

Memory::~Memory()
{}

// The get_csv_contents() function is used to get the csv_contents from the Memory class. Precondition: read_csv() must have been called to create a list of the requested CSV file. Postcondition: returns the csv_contents from Memory class. This return is used as a parameter in uintToBinary();
list<unsigned int> Memory::get_csv_contents()
{
  return csv_contents;
}

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
class Instruction : public Memory
{
public:
  void uintToBinary(list<unsigned int> given);

  void decode(vector<string> binary);
  void decode_opcode(string code);
  void decode_src1(string code);
  void decode_src2(string code);
  void decode_dest(string code);

  vector<string> binary_instructions;
  string type;  //bits 0 to 1: used in r, i, p, j
  string opcode;  //bits 2 to 5: used in r, p
  int dest;  //bits 6 to 10: used in r, i, p
  int src1;  //bits 11 to 15: used in r, i
  int src2;  //bits 16 to 20: used in r
  int immediate;  //bits 16 to 31: used in i
  int address;  //bits 2 to 31: used in j
};

// The uintToBinary() function is used to convert the unsigned int list values from a specified CSV file (That were retrieved using read_csv()) into string type values. These values are then pushed back into the binary_instructions vector inside of the Instruction class.  Precondition: read_csv() must have been called to create a unsinged integer list of the requested CSV file. The parameter of the uintToBinary() function is this unsigned integer list. Postcondition: Converts each unsigned integer element in the original list into a string and then prepends each string into the binary_instructions vector in the Instruction class.
void Instruction::uintToBinary(list<unsigned int> given)
{
  // Initalize iterator to traverse unsigned integer list.
	list<unsigned int>::iterator it;

	for (it = given.begin(); it != given.end(); ++it)
  {
    // initialize temporary string named temp_string that holds the converted string (that was once an unsigned integer).
		string temp_string = bitset<32>(*it).to_string();
    cout << "conversion: " << temp_string << ", ";
    // Push back each temp_string into the binary_instructions vector.
		binary_instructions.push_back(temp_string);
	}
  cout << "Done conversion" << endl;
}

// The decode() function is used to determine what type of instruction format (R,I,J, or P type) each element in the vector string from the Instruction class is. After that, this function will write to the "type" and "opcode" string variables in the Instruction class, as well as the "dest", "src1", "src2", "immediate" and "address" integer values in the instruction class.  Precondition: A vector string that only contained binary values was successfully passed into the function. Postcondition: Determined what type of instruction format (R,I,J, or P type) each element in the vector string was. Wrote to the "type" and "opcode" string variables in the Instruction class, as well as the "dest", "src1", "src2", "immediate" and "address" integer values in the instruction class.
void Instruction::decode(vector<string> binary)
{
	vector<string>::iterator it;

	for (it = binary.begin(); it != binary.end(); ++it)
	{
		string type_bits = it->substr(0,2);
    cout << "type_bits: " << type_bits << endl;

		if (type_bits == "00")
		{//type r
			type = "r";
			string opcode_bits = it->substr(2,4);
      string dest_bits = it->substr(6,5);
      string src1_bits = it->substr(11,5);
      string src2_bits = it->substr(16,5);
			decode_opcode(opcode_bits);
      dest = stoi(dest_bits, 0, 2);
      src1 = stoi(src1_bits, 0, 2);
      src2 = stoi(src2_bits, 0, 2);

      cout << "dest: " <<  dest << " " << "src1: " << src1 << " " << "src2: " << src2 << endl << endl;
      /*
      decode_dest(dest_bits);
      decode_src1(src1_bits);
      decode_src2(src2_bits);
      */
		}

    //type i. type i DOES NOT have an src2.
		else if (type_bits == "01")
    {
			type = "i";
			string opcode_bits = it->substr(2,4);
      string dest_bits = it->substr(6,5);
      string src1_bits = it->substr(11,5);
      string immediate_bits = it->substr(16, 16); // PLEASE CHECK IF THIS IS CORRECT!!!!!!!!!!
			decode_opcode(opcode_bits);
      dest = stoi(dest_bits, 0, 2);
      src1 = stoi(src1_bits, 0, 2);
      cout << "dest: " <<  dest << " " << "src1: " << src1 << " " << endl << endl;
		}

    // type j
		else if (type_bits == "10")
    {
      // string address == it->substr(2,30);
			type = "j";

		}

    // type p. Type p DOES NOT have an src1 or src2
		else if (type_bits == "11")
    {
			type = "p";
			string opcode_bits = it->substr(2,4);
      string dest_bits = it->substr(6,5);
			decode_opcode(opcode_bits);
      dest = stoi(dest_bits, 0, 2);
      cout << "dest: " << dest <<  endl;
		}

		else{
			cout << "Error: invalid instruction type." << endl << endl;
		}
	}
}

void Instruction::decode_opcode(string code)
{

	if (code == "0000") // add
	{
		opcode = "add";
	}

	else if (code == "0001") // subtract
	{
		opcode = "subtract";
	}

  else if (code == "0010")
  {
    opcode = "multiply";
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

	else if (code == "0111" || code == "1000" ) // move
	{
		opcode = "move";
	}

	else
	{
		cout << "Error: Invalid opcode value." << endl << endl;
	}
}

/*
void Instruction::decode_dest(string code)
{
  dest = stoi(code, 0, 2);
}

void Instruction::decode_src1(string code)
{
  src1 = stoi(code, 0, 2);
}

void Instruction::decode_src2(string code)
{
  src2 = stoi(code, 0, 2);
}
*/


// A class that implements the commit, execute and fetch functions, such that main()
// simply instantiates the pipeline class and any necessary structures, and then continuously
// calls pipeline fetch, execute and commit stages until the program has finished executing.
class Pipeline : public Instruction
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
	Pipeline tester;

	tester.read_csv("factorial.csv");
  tester.uintToBinary(tester.get_csv_contents());
  tester.decode(tester.binary_instructions);
	//cout << "Hello world";

}
