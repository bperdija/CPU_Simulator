//TO DO: regist file, IQ, ROB, execute, commit, makefile, modes, widths, clock cycles, stats

#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
//#include <typeinfo> // not needed
#include <cmath>
#include <bitset>
#include <queue>

#define FETCH_WIDTH 2
#define ISSUE_WIDTH 2
#define COMMIT_WIDTH 2
#define EXECUTE
#define DEBUG

using namespace std;

class Memory;
class Instruction;
class Pipeline;
class IQueue;
class Execute;
class FetchUnit;
class RegisterFile;
class ReorderBuffer;
class Statistics;
class HelperFunctions;
struct ROB_element;

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
  vector<unsigned int> get_csv_contents();
protected:
  vector<unsigned int> csv_contents;
};

Memory::Memory()
{}

Memory::~Memory()
{}

// The get_csv_contents() function is used to get the csv_contents from the Memory class. Precondition: read_csv() must have been called to create a list of the requested CSV file. Postcondition: returns the csv_contents from Memory class. This return is used as a parameter in uintToBinary();
vector<unsigned int> Memory::get_csv_contents()
{
  return csv_contents;
}

void Memory::read_csv(string file_name)
{
	// variables
	string temp_element;
	// define empty lists to store values
	std::vector<unsigned int> values;
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
  void uintToBinary(unsigned int given);

  void decode(string binary_code);
  void decode_opcode(string code);
  int stringToInt(string code);

  string binary_instruction;
  string type;  //bits 0 to 1: used in r, i, p, j
  string opcode;  //bits 2 to 5: used in r, p
  int dest;  //bits 6 to 10: used in r, i, p
  int src1;  //bits 11 to 15: used in r, i
  int src2;  //bits 16 to 20: used in r
  int immediate;  //bits 16 to 31: used in i
  int address;  //bits 2 to 31: used in j
};

// The uintToBinary() function is used to convert the unsigned int list values from a specified CSV file (That were retrieved using read_csv()) into string type values. These values are then pushed back into the binary_instructions vector inside of the Instruction class.  Precondition: read_csv() must have been called to create a unsinged integer list of the requested CSV file. The parameter of the uintToBinary() function is this unsigned integer list. Postcondition: Converts each unsigned integer element in the original list into a string and then prepends each string into the binary_instructions vector in the Instruction class.
void Instruction::uintToBinary(unsigned int given)
{
  // initialize string named binary_instruction that holds the converted string (that was once an unsigned integer).
	binary_instruction = bitset<32>(given).to_string();
  cout << "conversion: " << binary_instruction << ", ";

  cout << "Done conversion" << endl;
}

// The decode() function is used to determine what type of instruction format (R,I,J, or P type) each element in the vector string from the Instruction class is. After that, this function will write to the "type" and "opcode" string variables in the Instruction class, as well as the "dest", "src1", "src2", "immediate" and "address" integer values in the instruction class.  Precondition: A vector string that only contained binary values was successfully passed into the function. Postcondition: Determined what type of instruction format (R,I,J, or P type) each element in the vector string was. Wrote to the "type" and "opcode" string variables in the Instruction class, as well as the "dest", "src1", "src2", "immediate" and "address" integer values in the instruction class.
void Instruction::decode(string binary_code)
{
		string type_bits = binary_code.substr(0,2);
    cout << "type_bits: " << type_bits << endl;

		if (type_bits == "00")
		{//type r
			type = "r";
			string opcode_bits = binary_code.substr(2,4);
      string dest_bits = binary_code.substr(6,5);
      string src1_bits = binary_code.substr(11,5);
      string src2_bits = binary_code.substr(16,5);
			decode_opcode(opcode_bits);
      dest = stringToInt(dest_bits);
      src1 = stringToInt(src1_bits);
      src2 = stringToInt(src2_bits);

      cout << "dest: " <<  dest << " " << "src1: " << src1 << " " << "src2: " << src2 << endl << endl;
		}

    //type i. type i DOES NOT have an src2.
		else if (type_bits == "01")
    {
			type = "i";
			string opcode_bits = binary_code.substr(2,4);
      string dest_bits = binary_code.substr(6,5);
      string src1_bits = binary_code.substr(11,5);
      string immediate_bits = binary_code.substr(16, 16);
			decode_opcode(opcode_bits);
      dest = stringToInt(dest_bits);
      src1 = stringToInt(src1_bits);
      immediate = stringToInt(immediate_bits);
      cout << "dest: " <<  dest << " " << "src1: " << src1 << " " << endl << endl;
		}

    // type j
		else if (type_bits == "10")
    {
      type = "j";
      string address_bits = binary_code.substr(2,30);
      address = stringToInt(address_bits);

		}

    // type p. Type p DOES NOT have an src1 or src2
		else if (type_bits == "11")
    {
			type = "p";
			string opcode_bits = binary_code.substr(2,4);
      string dest_bits = binary_code.substr(6,5);
			decode_opcode(opcode_bits);
      dest = stringToInt(dest_bits);
      cout << "Enter parameter for program: ";
      cin >> immediate;

      while(true)
      {
        if(cin.fail())
        {
          cout << "Error: Parameter must be a integer." << endl;
          cin.clear();
          cin.ignore(256, '\n');
          cout << "Enter parameter for program: ";
          cin >> immediate;
        }
        else
        {
          break;
        }
      }

      cout << "dest: " << dest <<  endl;
		}

		else{
			cout << "Error: invalid instruction type." << endl << endl;
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

 int Instruction:: stringToInt(string code)
{
  int result = stoi(code, 0, 2);
  return result;
}

struct IQ
{
  int dest_IQ;
  int src1_IQ;
  int src1_valid;
  int src2_IQ;
  int src2_valid;
  int immediate_IQ;
};

// a class that implements the IQ. It instantiate a (finite) queue; inserts an instruction into queue;
// check if instruction is ready for execution; printIQ() for debugging; delete an entry of the IQ during read and execute;
class IQueue
{
public:
  queue<string> IQueue_finite;
  void insert_instruction(string element);
  void delete_entry();
  void check();
  void printIQ();
  void insert_IQ_instruction(string element);
protected:
  bool ready;
};

void IQueue::insert_IQ_instruction(string element)
{
  IQueue_finite.push(element); // NOT SURE WHAT WE'RE PUSHING TO THIS QUEUE!! STRING ELEMENT?
}

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

struct Register_File
{
  int reg_num;
  int src1_data;
  int src2_data;
  int src1_valid;
  int src2_valid;
};

//a class that maintains the register file; setup the register file (RF) such that all register locations are associated with i) data value and ii) a status bit. The status bit indicates whether the value in the register is valid for reading or is awaiting an instruction’s result. The register # as shown in the table below is solely used to index a register. More details on the register file system is provided in a subsequent section entitled “Register File FYI”;  accessor and mutator functions should be implemented for setting or obtaining the value and status of a register.
class RegisterFile
{
public:


};

//struct to hold the data of each element in the ROB queue
struct ROB_element
{
public:
  ROB_element() : valid(), inst(), ROB_ID() {} //default constructor
  ROB_element(bool validity, string The_inst, int The_ROB_ID) : valid(validity), inst(The_inst), ROB_ID(The_ROB_ID) {} //overlaoded constructor

  bool valid; //do we want bool or just int?
  string inst; //I have no idea what data type this should be??????
  int ROB_ID;
};

//STL structure used to maintain a list in the pipeline, such that the instructions are inserted and retired inorder. Instructions are inserted at fetch, removed at commit when valid. An example of the contents in the ROB are shown below.
class ReorderBuffer
{
public:
  ReorderBuffer();
  //friend void access_ROB();
  void set_ROB_element(string inst);

  queue<ROB_element> ROB;
  int ID_count;
};

/*
ReorderBuffer::access_ROB()
{
  return ROB;
}
*/

ReorderBuffer::ReorderBuffer()
{
  ID_count = 0;
}

void ReorderBuffer::set_ROB_element(string inst)
{
  ID_count++;
  ROB.push(ROB_element(false, inst, ID_count)); //create new ROB_element in the ROB queue
}

//a class used to track and calculate latency and throughput statistics of the running program. Statistics are provided to the user via standard output at the end of a program’s execution.
class Statistics
{
public:
  void calc_latency();
  void calc_throughput();

  int clock_cylce_count;
  double clock_cycles_sum;
  double latency;
  double throughput;
};

//this class is used to provide all classes with any helper functions or global variables shared across the classes.
class HelperFunctions
{

};

// A class that implements the commit, execute and fetch functions, such that main()
// simply instantiates the pipeline class and any necessary structures, and then continuously
// calls pipeline fetch, execute and commit stages until the program has finished executing.
class Pipeline : public Instruction, public ReorderBuffer, public IQueue
{
public:
  Pipeline();
  ~Pipeline();
  void Fetch(unsigned int);
  void Execute(unsigned int);
  void Commit(unsigned int);
  friend void set_ROB_element(string inst);
  void run(int PC_number);
  void select_file();

  int PC_number;
};

Pipeline::~Pipeline()
{}

Pipeline::Pipeline()
{
  PC_number = 0;
  select_file();
  run(PC_number);
  /*
  for (int i = 0; i < csv_contents.end(); i++)
  {
    uintToBinary(csv_contents[i]);
  }
  */
}

void Pipeline :: select_file()
{
  string user_choice;

  cout << "Type 'exit' to terminate this application" << endl;
  cout << "Enter a program for execution: ";
  cin >> user_choice;
    user_choice[0] = toupper(user_choice[0]);

  if (user_choice == "Fibonacci" || user_choice == "Fibonacci.csv")
  {
    read_csv("fibonacci.csv");
    cout << "reading file fibonacci.csv" << endl << endl;
  }

  else if (user_choice == "Factorial" || user_choice == "Factorial.csv")
  {
    read_csv("factorial.csv");
    cout << "reading file factorial.csv" << endl << endl;
  }

  else if (user_choice == "Test program" || user_choice == "inst_mem.csv")
  {
    read_csv("inst_mem.csv");
    cout << "reading file inst_mem.csv" << endl << endl;
  }

  else if (user_choice == "exit")
  {
    exit(0);
  }

  else{
    cout << "Error: Invalid file name." << endl;
  }
}

void Pipeline::run(int PC_number)
{
  std::vector<unsigned int>::iterator it;
  it = csv_contents.begin();

  for (it; it != csv_contents.end(); ++it)
  {
    Fetch(csv_contents[PC_number]);
    Execute(csv_contents[PC_number]);
    Commit(csv_contents[PC_number]);
  }

  if (it == csv_contents.end()){
    //pop everything off csv_contents
    for (int i = 0; i <= csv_contents.size(); i++)
    {
      csv_contents.pop_back();
    }

    select_file();
  }
}

void Pipeline::Fetch(unsigned int inst)
{
  //need to check if IQ and ROB have enough free spaces to proceed to fetch more instructions
  // if IQ (element) > 2 || ROB (element) > 2, do not fetch(?)

  std::vector<unsigned int>::iterator it;
  it = csv_contents.begin();

  while(((IQueue_finite.size() < 3) && (it != csv_contents.end())) || ((ROB.size() < 3) && (it != csv_contents.end()))) // I would like to iterate as long as it hasnt reached the end of the csv file but the while loop always goes to the end of the csv file, regardless if theres more elements in IQUEUE or ROB. This is wrong!
  // while(((IQueue_finite.size() < 3) || (it != csv_contents.end()))
  {
    cout << "Iqueue size: " << IQueue_finite.size() << endl<< "ROB size: " << ROB.size() << endl;
    uintToBinary(*it); // Fetch the instruction located at PC from the instruction memory
    PC_number++;
    decode(binary_instruction); // Decode the instruction
    set_ROB_element(binary_instruction); // Assigns a ROB ID to the instruction, places the instruction in the ROB
    insert_IQ_instruction(binary_instruction); // and places the instruction in the IQ.
    // CHECK THE IQ FOR VALIDY HAS NOT BEEN DONE!!!


    // Check if the instruction is a branch or jump;

    /*
    if it == csv_contents.end()
    {
      break;
    }
    */
    it++;
    //PC++;

    /*
    uintToBinary(1234);
    decode(binary_instruction);
    set_ROB_element(binary_instruction);
    */
  }
}

void Pipeline :: Execute(unsigned int inst)
{}

void Pipeline :: Commit(unsigned int inst)
{
  if (ROB.front().valid == true)
  {
    ROB.pop();
  }

  else
  {
    cout << "Error: Instruction attempted to commit before valid in Reorder Buffer." << endl;
  }
}

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


  /*
  string response;
  bool runner = true;
  while (true)
  {
    cout << "What program????";
    cin >> response;
    if response == "exit"
    {
      runner = false;
    {
    else
    {
      read_csv(response);
      tester.Fetch();
    }
  }

  }
  */

	//tester.read_csv("factorial.csv");
  //tester.Fetch();
  //tester.uintToBinary(tester.get_csv_contents());
  //tester.decode(tester.binary_instructions);
	//cout << "Hello world";

}
