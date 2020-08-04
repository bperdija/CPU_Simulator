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
#define FINITE_LIMIT 100
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

  cout << "Done conversion" << "original: " << given << endl ;
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
      cout << "dest: " <<  dest << " " << "src1: " << src1 << " " << endl;
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
  vector<Register_File> r;
};

RegisterFile::RegisterFile()
{
  // WHAT ABOUT R2?
  r.resize(15);
  //fill(r.validity.begin(), r.validity.end(), -1);
  r[0].data = 0;
  r[0].validity = 1;

}

//struct to hold the data of each element in the ROB queue
struct ROB_element
{
public:
  ROB_element() : valid(), inst(), ROB_ID() {} //default constructor
  ROB_element(int validity, string The_inst, int The_ROB_ID) : valid(validity), inst(The_inst), ROB_ID(The_ROB_ID) {} //overlaoded constructor

  int valid;
  string inst;
  int ROB_ID;
};

//STL structure used to maintain a list in the pipeline, such that the instructions are inserted and retired inorder. Instructions are inserted at fetch, removed at commit when valid. An example of the contents in the ROB are shown below.
class ReorderBuffer
{
public:
  ReorderBuffer();
  deque<ROB_element> ROB;
  // void Access_ROB_element(int validity);
  void set_ROB_element(string inst, int PC_num);

};

ReorderBuffer :: ReorderBuffer()
{}

/*
void ReorderBuffer :: Access_ROB_element(int validity)
{
  ROB.front().valid(1); // Change the last element to
}
*/

void ReorderBuffer::set_ROB_element(string inst, int PC_num)
{
  ROB.push_back(ROB_element(0, inst, PC_num)); //create new ROB_element in the ROB queue
}

struct IQ
{
  IQ() : opcode_IQ(), src1_IQ(), src1_valid(), src2_IQ(), src2_valid(), dest_IQ(), ROB_ID_IQ() {} //default constructor
  IQ(string op, int sr1, int val1, int sr2, int val2, int des, int num) : opcode_IQ(op), src1_IQ(sr1), src1_valid(val1), src2_IQ(sr2), src2_valid(val2), dest_IQ(des), ROB_ID_IQ(num) {}
  IQ(string op, int val1, int sr2, int val2, int des, int num) : opcode_IQ(op), src1_valid(val1), src2_IQ(sr2) , src2_valid(val2), dest_IQ(des), ROB_ID_IQ(num) {}
  string opcode_IQ = 0 ;
  int src1_IQ = 0;
  int src1_valid = 0;
  int src2_IQ = 0;
  int src2_valid = 0;
  int dest_IQ = 0;
  int ROB_ID_IQ = 0;
};

/*
string binary_instruction;
string type;  //bits 0 to 1: used in r, i, p, j
string opcode;  //bits 2 to 5: used in r, p
int dest;  //bits 6 to 10: used in r, i, p
int src1;  //bits 11 to 15: used in r, i
int src2;  //bits 16 to 20: used in r
int immediate;  //bits 16 to 31: used in i
int address;  //bits 2 to 31: used in j
*/

// a class that implements the IQ. It instantiate a (finite) queue; inserts an instruction into queue;
// check if instruction is ready for execution; printIQ() for debugging; delete an entry of the IQ during read and execute;
class IQueue : public Instruction, public RegisterFile
{
public:
  deque<IQ> The_Queue;
  void set_IQ_element(int PC_num);
};

void IQueue::set_IQ_element(int PC_num)
{
  cout << "this is the type code: " << type << endl;
  cout << "this is the opcode code: " << opcode << endl << endl;
  if (type == "r")
  {
    The_Queue.push_back(IQ(opcode, src1, r[src1].validity, src2, r[src2].validity, dest, PC_num));
    cout << "it was r" << endl;
  }

  else if (type == "i")
  {
    The_Queue.push_back(IQ(opcode, src1, r[src1].validity, immediate, 1, dest, PC_num));
    cout << "it was i" << endl;
  }

  else if (type == "p")
  {
    The_Queue.push_back(IQ(opcode, 1, immediate, 1, dest, PC_num));
    cout << "it was p" << endl;
  }

  else
  {
    cout << "Error: Instruction could not be added to the IQueue." << endl;
  }
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
class Pipeline :  public ReorderBuffer, public IQueue
{
public:
  Pipeline();
  ~Pipeline();
  void run(int PC_number);
  void select_file();
  void Fetch(unsigned int); // passing a binary value here
  void Execute();
  void Commit();
  int PC;

};

Pipeline::~Pipeline()
{}

Pipeline::Pipeline()
{
  PC = 0;
  select_file();
  run(PC);
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

  else if (user_choice == "Test" || user_choice == "inst_mem.csv")
  {
    read_csv("inst_mem.csv");
    cout << "reading file inst_mem.csv" << endl << endl;
  }

  else if (user_choice == "exit")
  {
    cout << "Good bye.";
    exit(0);
  }

  else{
    cout << "Error: Invalid file name." << endl;
  }
}

void Pipeline::run(int PC_number)
{
  cout << "this is the csv size: " << csv_contents.size() << endl;

  while ((PC < csv_contents.size()))// || !ROB.empty())
  {
    //cout << csv_contents.size();
    Fetch(csv_contents[PC_number]);
    Execute();
    Commit();
  }

  for (int i = 0; i < The_Queue.size(); i++)
  {
    cout << "IQ #" << i << ": " << The_Queue[i].opcode_IQ << endl;
  }

  //pop everything off csv_contents
  int temp = csv_contents.size();
  for (int i = 0; i <= temp; i++)
  {
    csv_contents.pop_back();
  }

  select_file();
  PC = 0;
  run(PC);
}

void Pipeline::Fetch(unsigned int inst)
{
  //cout << "This is inside fetch" << endl;
  for (int i = 0; (i < FETCH_WIDTH && PC < csv_contents.size()); i++)
  {
    //cout << "This is inside fetch for loop" << endl;

    //cout << "This is inside fetch if statement" << endl;

      if ((The_Queue.size() < FINITE_LIMIT) || (ROB.size() < FINITE_LIMIT))
      {
        cout << "This is the PC: " << PC << endl;
        cout << "Iqueue size: " << The_Queue.size() << endl<< "ROB size: " << ROB.size() << endl;
        uintToBinary(csv_contents[PC]);
        decode(binary_instruction);


        if (type == "j")
        {
          PC = address;
          Fetch(PC);
        }


        else if (opcode == "BEQ")
        {
          if (r[src1].data == r[dest].data)
          {
            PC = immediate;
          }

          else
          {
            PC++;
          }
          Fetch(PC);
        }


        else if (opcode == "BNE")
        {
          cout << "this is src1: " << r[src1].data << " This is dest: " << r[dest].data << endl;
          if (r[src1].data != r[dest].data)
          {
            cout << "NOOOOOOOOOOOO" << endl;
            PC = immediate;
          }

          else
          {
            PC++;
          }

          Fetch(PC);
        }


        set_ROB_element(binary_instruction, PC);
        set_IQ_element(PC);

        PC++;
      }
  }
}





void Pipeline :: Execute()
{
  for (int i = 0; i < ISSUE_WIDTH; i++)
  {
    if (The_Queue.front().src1_valid == 1 && The_Queue.front().src2_valid == 1)
    {
      if (type == "r")
      {
        if (The_Queue.front().opcode_IQ == "add")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data + r[The_Queue.front().src2_IQ].data;
        }

        else if (The_Queue.front().opcode_IQ == "subtract")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data - r[The_Queue.front().src2_IQ].data;
        }

        else if (The_Queue.front().opcode_IQ == "multiply")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data * r[The_Queue.front().src2_IQ].data;
        }

        else if (The_Queue.front().opcode_IQ == "divide")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data / r[The_Queue.front().src2_IQ].data;
        }

        else if (The_Queue.front().opcode_IQ == "modulo")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data % r[The_Queue.front().src2_IQ].data;
        }

        else if (The_Queue.front().opcode_IQ == "move")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data;
        }



      }

      else if (type == "i")
      {
        if (The_Queue.front().opcode_IQ == "add")
        {
          cout << "add is being mean" << endl;
          cout << "The destination is r[" << The_Queue.front().dest_IQ << "] uwu" << endl;
          cout << "src1_IQ  ( ie r=) " << The_Queue.front().src1_IQ << endl;
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data + The_Queue.front().src2_IQ;
        }

        else if (The_Queue.front().opcode_IQ == "subtract")
        {
          cout << "sub is being mean" << endl;
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data - The_Queue.front().src2_IQ;
        }

        else if (The_Queue.front().opcode_IQ == "multiply")
        {
          cout << "mult is being mean" << endl;
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data * The_Queue.front().src2_IQ;
        }

        else if (The_Queue.front().opcode_IQ == "divide")
        {
          cout << "div is being mean" << endl;
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data / The_Queue.front().src2_IQ;
        }

        else if (The_Queue.front().opcode_IQ == "modulo")
        {
          cout << "modulo is being mean" << endl;
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data % The_Queue.front().src2_IQ;
        }
      }

      else if (type == "p")
      {
        r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src2_IQ].data;
      }

      cout << "Execute complete!! r[" << The_Queue.front().dest_IQ << "] = " << r[The_Queue.front().dest_IQ].data << endl;

      r[The_Queue.front().dest_IQ].validity = 1;

      //pop instruction from IQ and mark as valid in ROB
      The_Queue.pop_front();
      ROB[PC].valid = 1;

    }
  }
}

void Pipeline :: Commit()
{
  if (ROB.front().valid == 1)
  {
    ROB.pop_front();
  }
  else
  {
    cout << "Error: can't commit." << endl;
  }
}

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
int main()
{
	Pipeline tester;


}
