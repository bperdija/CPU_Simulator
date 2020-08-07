// TO DO:
// move everything to separate files
// test makefile*
// create graphs of widths
// Create audio recording
// Create individual reports

// IF WE HAVE TIME:
// more throw exceptions  -> add exception for if r[2] written to by not a p-type and r[0] constant
// change PC from being pipeline variable to being Instruction variable
// make all 'Error:' red
// fix crashing because of too much data

// move if r, i and p code blocks into functions

// IF WE HAVE FUN TIME:
// Troll mode - Laura
// rick roll
// 'nice' zooms
// debug techo

// #include <list> //I don't think we need this anymore, we don't use any lists just vectors
#include <iostream>
#include <iomanip> //needed for setprecision()
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <bitset> //needed for bitset<size>(unsigned int).to_string()

#define FETCH_WIDTH 2
#define ISSUE_WIDTH 2
#define COMMIT_WIDTH 5
#define FINITE_LIMIT 100
#define REGISTER_SIZE 15
#define EXECUTE
//#define DEBUG

using namespace std;

class Memory; //https://youtu.be/ub82Xb1C8os <--- very useful, I highly recommend watching this
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

// The Memory() function is used to construct a Memory object.
// Precondition: Object of Memory created.
// Postcondition: Default object created.
Memory::Memory()
{}

// The ~Memory() function is used to destruct a Memory object.
// Precondition: Object of Memory was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
Memory::~Memory()
{}

// The get_csv_contents() function is used to get the csv_contents from the Memory class.
// Precondition: read_csv() must have been called to create a vector of the CSV file.
// Postcondition: Returns the csv_contents from Memory class. This return is used as a parameter in uintToBinary();
vector<unsigned int> Memory::get_csv_contents()
{
  return csv_contents;
}

// The read_csv() function is used to read an inputted csv file and write to the csv_contents vector in Memory.
// Precondition: A file_name variable is passed in the parameter (which includes the .csv extension), the file must be a single line .csv file with unsigned integer elements.
// Postcondition: Records each element in the chosen csv file into the vector values, and then copies that vector into the csv_contents vector in the Memory class.
void Memory::read_csv(string file_name)
{
	string temp_element;
	// define empty vectors to store values
	vector<unsigned int> values;
	string file = file_name;
	ifstream myFile;
	myFile.open(file);

	if(myFile.is_open()) // while the file is open
	{
		while(myFile.peek() != EOF)//(!myFile.eof())
    //while(myFile.good())
		{
			getline(myFile, temp_element, ',');
			unsigned int temp = stoul((temp_element).c_str(), 0); // string to unsigned int with help from https://www.cplusplus.com/forum/beginner/148948/
			values.push_back(temp);

    /*  if (myFile.eof())
      {
        cout << "ghetto code block" << endl;
        myFile.close();
      }
      */
		}
    myFile.close();
	}

	csv_contents = values;
}

// A class that contains all information required of an instruction -- decodes the instruction;
// converts instruction from unsigned int to binary; provides various accessor functions for all
// parameters of a given instruction (i.e. get src1, get src2, get operation, get immediate etc)
class Instruction : public Memory
{
public:
  Instruction();
  ~Instruction();

private:
  void decode_opcode(string code);
  int stringToInt(string code);

protected:
  void uintToBinary(unsigned int given);
  void decode(string binary_code);

  string binary_instruction;
  string type;  //bits 0 to 1: used in r, i, p, j
  string opcode;  //bits 2 to 5: used in r, p
  int dest;  //bits 6 to 10: used in r, i, p
  int src1;  //bits 11 to 15: used in r, i
  int src2;  //bits 16 to 20: used in r
  int immediate;  //bits 16 to 31: used in i
  int address;  //bits 2 to 31: used in j
};

// The Instruction() function is used to construct a Instruction object.
// Precondition: Object of Instruction created.
// Postcondition: Default object created.
Instruction::Instruction()
{}

// The ~Instruction() function is used to destruct a Instruction object.
// Precondition: Object of Instruction was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
Instruction::~Instruction()
{}

// The uintToBinary() function is used to convert the unsigned int value from the specified CSV file (That were retrieved using read_csv()) into a string type value. The value is then written to the binary_instruction inside of the Instruction class.  Precondition: The parameter of the uintToBinary() function is a unsigned intege. Postcondition: Converts the unsigned integer element into a string and then writes the string to binary_instruction in the Instruction class.
void Instruction::uintToBinary(unsigned int given)
{
  // initialize string named binary_instruction that holds the converted string (that was once an unsigned integer).
	binary_instruction = bitset<32>(given).to_string();
}

// The decode() function is used to determine what type of instruction format (R,I,J, or P type) each element in the vector string from the Instruction class is. After that, this function will write to the "type" and "opcode" string variables in the Instruction class, as well as the "dest", "src1", "src2", "immediate" and "address" integer values in the instruction class.
//Precondition: A string is passed as the parameter.
//Postcondition: Determined what type of instruction format (R,I,J, or P type) each element in the vector string was. Wrote to the "type" and "opcode" string variables in the Instruction class, as well as the "dest", "src1", "src2", "immediate" and "address" integer values in the instruction class. If an invalid instruction type an error is printed.
void Instruction::decode(string binary_code)
{
		string type_bits = binary_code.substr(0,2);

    // type r
		if (type_bits == "00")
		{
			type = "r";
			string opcode_bits = binary_code.substr(2,4);
      string dest_bits = binary_code.substr(6,5);
      string src1_bits = binary_code.substr(11,5);
      string src2_bits = binary_code.substr(16,5);
			decode_opcode(opcode_bits);
      dest = stringToInt(dest_bits);
      src1 = stringToInt(src1_bits);
      src2 = stringToInt(src2_bits);
      immediate = -1;
		}

    // type i. type i DOES NOT have an src2.
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
      //cout << "Enter parameter for program: ";
      //cin >> immediate;

      while(true)
      {

          try
          {
            cout << "Enter parameter for program: ";
            cin >> immediate;

            if (immediate < 0) throw Exceptions("negative");
            if (immediate == 0) throw Exceptions("zero");
            if (immediate > 31) throw Exceptions("Number over 31");
            if (cin.fail()) throw Exceptions("word");
            else break;
          }

          catch(Exceptions out)
          {
            cout << "Can't enter a " << out.getMsg_parameter() << "!" << endl;
            cin.clear();
            cin.ignore(256, '\n');
          }
        }
		}

		else
    {
			cout << "Error: invalid instruction type." << endl << endl;
		}
}

// The decode_opcode() function is used to simply write to opcode in the class Instruction based on what parameter was passed. Precondition: A parameter from the function decode()  Postcondition: Determined what type of instruction format (R,I,J, or P type) each element in the vector string was. Wrote to the "type" and "opcode" string variables in the Instruction class, as well as the "dest", "src1", "src2", "immediate" and "address" integer values in the instruction class.
// Precondition: A string passed as the parameter.
// Postcondition: If the string matches an opcode operation save the opcode name to opcode variable, if it doesn't match print an error.
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

	else if (code == "0111") // move
	{
		opcode = "move";
	}

  else if (code == "1000" ) // print
  {
    opcode = "print";
  }

	else
	{
		cout << "Error: Invalid opcode value." << endl << endl;
	}
}

// The stringToInt() function is used to convert the binary string code to its decimal value.
// Precondition: A string is passed as the parameter.
// Postcondition: The string converted to decimal its representation as integer and the integer is returned.
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
  ~RegisterFile();

protected:
  vector<Register_File> r;
};

// The RegisterFile() function is used to construct a RegisterFile object.
// Precondition: Object of RegisterFile created.
// Postcondition: The r vector size is set to REGISTER_SIZE and each element in r has data set to 0 and validity set to 1 (valid).
RegisterFile::RegisterFile()
{
  // WHAT ABOUT R2?
  r.resize(REGISTER_SIZE);
  //fill(r.validity.begin(), r.validity.end(), -1);

  for (int i = 0; i <= REGISTER_SIZE; i++)
  {
    r[i].data = 0;
    r[i].validity = 1;
  }

}

// The ~RegisterFile() function is used to destruct a RegisterFile object.
// Precondition: Object of RegisterFIle was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
RegisterFile::~RegisterFile()
{}

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

//STL structure used to maintain a vector in the pipeline, such that the instructions are inserted and retired inorder. Instructions are inserted at fetch, removed at commit when valid. An example of the contents in the ROB are shown below.
class ReorderBuffer
{
public:
  ReorderBuffer();
  ~ReorderBuffer();
  // void Access_ROB_element(int validity);
  //void set_ROB_element(string inst, int PC_num);

protected:
  deque<ROB_element> ROB;
  void set_ROB_element(string inst, int PC_num);
};

// The ReorderBuffer() function is used to construct a ReorderBufferobject.
// Precondition: Object of ReorderBuffer created.
// Postcondition: Default object created.
ReorderBuffer :: ReorderBuffer()
{}

// The ~ReorderBuffer() function is used to destruct a ReorderBuffer object.
// Precondition: Object of ReorderBuffer was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
ReorderBuffer::~ReorderBuffer()
{}

// The set_ROB_element() function is used to push an instruction onto the ROB.
// Precondition: The binary string representing the instruction and integer for the instruction's PC are passed as parameters.
// Postcondition: new element of ROB deque pushed to back with valid set to 0 (invalid), inst set to the binary instruction string, and ROB_ID set to the PC
void ReorderBuffer::set_ROB_element(string inst, int PC_num)
{
  ROB.push_back(ROB_element(0, inst, PC_num)); //create new ROB_element in the ROB queue
}

struct IQ
{

  IQ() : opcode_IQ(), src1_IQ(), src1_valid(), src2_IQ(), src2_valid(), immediate_IQ(), dest_IQ(), ROB_ID_IQ(), type_IQ() {} //default constructor
  IQ(string op, int sr1, int val1, int sr2, int val2, int immed, int des, int num, string type_given) : opcode_IQ(op), src1_IQ(sr1), src1_valid(val1), src2_IQ(sr2), src2_valid(val2), immediate_IQ(immed), dest_IQ(des), ROB_ID_IQ(num), type_IQ(type_given){} // Does all types

  string opcode_IQ;
  int src1_IQ;
  int src1_valid;
  int src2_IQ;
  int src2_valid;
  int immediate_IQ;
  int dest_IQ;
  int ROB_ID_IQ;
  string type_IQ;
};

// a class that implements the IQ. It instantiate a (finite) queue; inserts an instruction into queue;
// check if instruction is ready for execution; printIQ() for debugging; delete an entry of the IQ during read and execute;
class IQueue : public Instruction, public RegisterFile
{
public:
  IQueue();
  ~IQueue();

protected:
  deque<IQ> The_Queue;
  void set_IQ_element(int PC_num);
};

// The IQueue() function is used to construct a IQueue object.
// Precondition: Object of IQueue created.
// Postcondition: Default object created.
IQueue::IQueue()
{}

// The ~IQueue() function is used to destruct a IQueue object.
// Precondition: Object of IQueue was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
IQueue::~IQueue()
{}

  // Precondition: The integer representing PC number passed as parameter, RegisterFile vector r has been initialized,  and the instruction has been decoded in decode() in Instruction Class.
  // Postcondition: If the type of instruction is r, i, or p, push instruction onto IQ using overloaded IQ constructor, if not print an error.
void IQueue::set_IQ_element(int PC_num)
{
  if (type == "r")
  {
    The_Queue.push_back(IQ(opcode, src1, r[src1].validity, src2, r[src2].validity, -1, dest, PC_num, type)); //first constructor
  }

  else if (type == "i")
  {
    The_Queue.push_back(IQ(opcode, src1, r[src1].validity, -1, 1, immediate, dest, PC_num, type)); //first constructor
  }

  else if (type == "p")
  {
    The_Queue.push_back(IQ(opcode, -1, 1, -1, 1, immediate, dest, PC_num, type));//second constructor
  }

  else
  {
    cout << "Error: Instruction could not be added to the IQueue." << endl;
  }
}

// A class that calls the memory class to i) initialize fetch (i.e. initialize the memory) to enforce encapsulation of instruction memory, and to ii) fetch and instruction from memory with the PC parameter passed
class FetchUnit : public IQueue
{
public:
  FetchUnit();
  ~FetchUnit();
  virtual void Fetch(unsigned int){}

protected:
  void jtype();
  void BNE();
  void BEQ();
  int PC;
};

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

//a class used to track and calculate latency and throughput statistics of the running program. Statistics are provided to the user via standard output at the end of a program’s execution.
class Statistics
{
public:
  Statistics();
  ~Statistics();

private:
  void calc_latency();
  void calc_throughput();

protected:
  void print_Statistics();

  double total_calls; //sum of instructions that enter a stage in the pipeline (for fetch, execute and commit)
  double total_commits; //sum of all instructions that are committed

  double total_execute_calls;
  double total_instr_exec;

  double latency;
  double throughput;
};

// The Statistics() function is used to construct a Statistics object.
// Precondition: Object of Statistics created.
// Postcondition: Set total_calls, total_commits, total_execute_calls, total_instr_exec, and latency set to zero.
Statistics :: Statistics()
{
  total_calls = 0;
  total_commits = 0;
  total_execute_calls = 0;
  total_instr_exec = 0;
  latency = 0;
}

// The ~Statistics() function is used to destruct a Statistics object.
// Precondition: Object of Statistics was created.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted
Statistics :: ~Statistics()
{}

// The calc_latency() function is used to calculate the latency of a file run through the pipeline.
// Precondition: A Statistics object has been constructed.
// Postcondition: The latency is calculated from total_calls and total_commits  and is then saved to latency variable.
void Statistics :: calc_latency()
{
  latency = total_calls / total_commits;
}

// The calc_throughput() function is used to calculate the throughput of a file run through the pipeline.
// Precondition: A Statistics object has been constructed.
// Postcondition: The throughput is calculated from total_instr_exec and total_execute_calls and is then saved to throughput variable
void Statistics::calc_throughput()
{
  throughput = total_instr_exec / total_execute_calls;
}

// The print_Statistics() function is used to retrieve the calculated latency and throughput, and print them to the screen.
// Precondition: A Statistics object has been constructed.
// Postcondition: The latency and throughput are calcualted and, when in DEBUG mode, printed to screen.
void Statistics::print_Statistics()
{
  calc_latency();
  calc_throughput();

  #ifdef EXECUTE
  cout << "Average latency per instruction: " << setprecision(2) << latency <<  fixed << endl;
  cout << "Average throughput per instruction: " << setprecision(2) << throughput << endl << endl;
  #endif
}

// A class that implements the commit, execute and fetch functions, such that main()
// simply instantiates the pipeline class and any necessary structures, and then continuously
// calls pipeline fetch, execute and commit stages until the program has finished executing.
class Pipeline :  public ReorderBuffer, public Statistics, public Exceptions, public FetchUnit
{
public:
  Pipeline();
  ~Pipeline();

private:
  void run(int PC_number);
  void select_file();
  virtual void Fetch(unsigned int); // passing a binary value here
  void Execute();
  void Commit();

protected:
  int inst_count;
};

// The Pipeline() function is used to construct a pipeline object.
// Precondition: Object of Pipeline created.
// Postcondition: PC and inst_count set to zero, select_file() and run() called.
Pipeline::Pipeline()
{
  PC = 0;
  inst_count = 0;
  select_file();
  run(PC);
}

// The ~Pipeline() function is used to destruct a pipline object.
// Precondition: Default destructor for the Pipeline Class.
// Postcondition: No dynamically allocated data to delete, stack variables automatically deleted.
Pipeline::~Pipeline()
{}

// The selct_file() function is used to ask the user which file they would like to run and reads the file.
// Precondition: function called //  THIS SKETCH NEED TO CHECK AGAIN
// Postcondition: User prompted to select file to run, if selection matches a file call read_csv() with file name as parameter, if user enters 'exit' terminate program, else throw error and let user try again
void Pipeline :: select_file()
{
  string user_choice;

  cin.clear();
  try
  {
    cout << endl << "Type 'exit' to terminate this application" << endl;
    cout << "Enter a program for execution (Fibonacci, Factorial, Test, Life): ";

    cin >> user_choice;
    user_choice[0] = toupper(user_choice[0]);

    if (user_choice == "Fibonacci" || user_choice == "Fibonacci.csv" )
    {

      read_csv("fibonacci.csv");

      #ifdef EXECUTE
      cout << "reading file fibonacci.csv" << endl << endl;
      #endif
    }

    else if (user_choice == "Factorial" || user_choice == "Factorial.csv")
    {
      read_csv("factorial.csv");

      #ifdef EXECUTE
      cout << "reading file factorial.csv" << endl << endl;
      #endif
    }

    else if (user_choice == "Test" || user_choice == "inst_mem.csv")
    {
      read_csv("inst_mem.csv");

      #ifdef EXECUTE
      cout << "reading file inst_mem.csv" << endl << endl;
      #endif
    }

    else if (user_choice == "Life" || user_choice == "meaning_of_life.csv")
    {
      read_csv("meaning_of_life.csv");

      #ifdef EXECUTE
      cout << "reading file meaning_of_life.csv" << endl << endl;
      #endif
    }

    else if (user_choice == "Exit")
    {
      cout << "Good bye." << endl;
      exit(0);
    }

    else throw Exceptions("Error");

  }

  catch (Exceptions element)
  {
    cout << element.getMsg_parameter() << ": Invalid file name." << endl << endl;
    select_file();
  }

  PC = 0;
}

// The run() function is used to move instructions through the pipeline.
// Precondition: The ReorderBuffer, Memory, and Statistics classes have been initialized.
// Postcondition: If there are still more instructions to fetch or to commit, the pipeline is looped through. When all instructions are completed, the statistics are printed to the screen.
void Pipeline::run(int PC_number)
{
    while ((PC < csv_contents.size()) || !ROB.empty())
    {
      Fetch(csv_contents[PC_number]);
      Execute();
      Commit();
    }
    print_Statistics();
}

// The Fetch() function is used to decode instructions and add them to the IQ and ROB if not detected as j-type, BNE, and BEQ instructions
// Precondition: unsigned integer passed as parameter, IQueue, ReorderBuffer, Statistics, RegisterFile and Instruction classes initialized
// Postcondition: If there is space in the The_Queue and ROB, call uintToBinary() to convert instruction from unsigned integer to binary string, call decode() to decode instruction, check if instruction is a j-type, BNE, or BEQ (divert/stall pipeline and call respective function if yes), add instruction to ROB and IQ, increment inst_count and PC, loop again if more space in FETCH_WIDTH, increment total_calls
void Pipeline::Fetch(unsigned int inst)
{

#ifdef DEBUG
cout << endl << "---------------------------- FETCH ----------------------------" << endl;
#endif

  for (int i = 0; (i < FETCH_WIDTH && PC < csv_contents.size()); i++, total_calls++)
  {
      if ((The_Queue.size() < FINITE_LIMIT) || (ROB.size() < FINITE_LIMIT))
      {
        uintToBinary(csv_contents[PC]);
        decode(binary_instruction);

        #ifdef DEBUG
        cout << "\033[1;31mPC:\033[0m" << PC << endl;
        cout << "b" << binary_instruction << endl;
        #endif


        if (type == "j")
        {
          jtype();
        //  PC = address;
          continue;
        }

        else if (opcode == "BEQ")
        {

          if (!The_Queue.empty())
          {
            break;
          }

          BEQ(); // This functions checks if (r[src1].data != r[dest].data)
          continue;
        }


        else if (opcode == "BNE")
        {
          if (!The_Queue.empty())
          {
            break;
          }

          BNE(); // This functions checks if (r[src1].data == r[dest].data)
          continue;
        }

        else
        {
          set_ROB_element(binary_instruction, inst_count);
          set_IQ_element(inst_count);

          inst_count++;
          PC++;
        }
      }

  }
}


// The Execute() function is used to check if instructions in the IQ are ready for execution and if they are, continues with execution.
// Precondition: decode() from the instruction class must have written to it's class variables, which then must have been pushed into the dequeue "The_queue" inside of the class IQueue.
// Postcondition: If both Src1 and Src2 in The_Queue are valid, write the results to the appropriate register file. Set the register at the location that was written to as valid. Pop the oldest element that was successfully read in The_queue off the queue, and set the element in the ROB as executed (valid = 1). If one of src1 or src2 are invalid, cout and error statement. Incrememnt total_instr_exec for every instruction that was executed, and total_execute_calls for every time execute is called.
void Pipeline :: Execute()
{

  #ifdef DEBUG
  deque<IQ>::const_iterator it;
  it = The_Queue.cbegin();
  if (it != The_Queue.cend())
  {
    cout << endl << "---------------------- INSTRUCTION QUEUE ----------------------" << endl;
    cout <<"Dest    V       Src1    V       Src2    Immediate    ROB ID" << endl;

    //deque<IQ>::const_iterator it;

    for (it = The_Queue.cbegin(); it != The_Queue.cend(); ++it)
    {
      cout << left << it->dest_IQ << "\t" << it->src1_valid << "\t" << it->src1_IQ << "\t" << it-> src2_valid << "\t" << it -> src2_IQ << "\t" << it -> immediate_IQ << "\t     " << it -> ROB_ID_IQ << endl;
    }

    cout << endl << "--------------------------- EXECUTE ---------------------------" << endl;
  }
  #endif

  for (int i = 0; i < ISSUE_WIDTH && The_Queue.size() != 0; i++)
  {
    #ifdef DEBUG
    cout << "checking status src1 R" << The_Queue.front().src1_IQ << ", V = " << The_Queue.front().src1_valid;
    cout << " checking status src2 R" << The_Queue.front().src2_IQ << ", V = " << The_Queue.front().src2_valid << endl;
    #endif

    // Checks the IQ's here for validity
    if (The_Queue.front().src1_valid == 1 && The_Queue.front().src2_valid == 1)
    {
      #ifdef DEBUG
      cout << "Instruction: " << The_Queue.front().ROB_ID_IQ << " - Ready for execution" << endl;
      cout << "Operation: " << The_Queue.front().opcode_IQ << endl;
      #endif

      if (The_Queue.front().type_IQ == "r")
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

        else if (The_Queue.front().opcode_IQ == "print")
        {
          #ifdef EXECUTE
          if (r[The_Queue.front().src1_IQ].data == 42)
          {
            cout << "\033[1;36mThe Meaning of Life is \033[0m" << r[The_Queue.front().src1_IQ].data << endl;
          }

          else
          {
            cout << "Final Result is: " << r[The_Queue.front().src1_IQ].data << endl;
          }

          #endif
        }
      }

      else if (The_Queue.front().type_IQ == "i")
      {
        if (The_Queue.front().opcode_IQ == "add")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data + The_Queue.front().immediate_IQ;
        }

        else if (The_Queue.front().opcode_IQ == "subtract")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data - The_Queue.front().immediate_IQ;
        }

        else if (The_Queue.front().opcode_IQ == "multiply")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data * The_Queue.front().immediate_IQ;
        }

        else if (The_Queue.front().opcode_IQ == "divide")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data / The_Queue.front().immediate_IQ;
        }

        else if (The_Queue.front().opcode_IQ == "modulo")
        {
          r[The_Queue.front().dest_IQ].data = r[The_Queue.front().src1_IQ].data % The_Queue.front().immediate_IQ;
        }
      }

      else if (The_Queue.front().type_IQ == "p")
      {
        r[The_Queue.front().dest_IQ].data = The_Queue.front().immediate_IQ;
      }

      #ifdef DEBUG
      cout << "Executed" << endl;
      #endif


      r[The_Queue.front().dest_IQ].validity = 1;

      //pop instruction from IQ and mark as valid in ROB
      for (auto it = ROB.begin(); it!=ROB.end(); it++)
      {
        if (it->ROB_ID == The_Queue.front().ROB_ID_IQ)
          {
            it -> valid = 1;
          }
      }

      #ifdef DEBUG
      cout << "ROB " << The_Queue.front().ROB_ID_IQ << " marked." << endl << endl;
      #endif

      The_Queue.pop_front();
    }

    else if (The_Queue.front().src1_valid != 1 || The_Queue.front().src2_valid != 1)
    {
      cout << "Error: Instruction sources not valid yet" << endl;
    }

    total_calls++;
    total_instr_exec++;
  }
  total_execute_calls++;
}

// The Commit() function is used to check if instruction in the ROB are ready to be committed, and if they are, continues with committing.
// Precondition: The ReorderBuffer class has been initialized
// Postcondition:  If the head of the ROB is valid, commit the instruction and pop it off the ROB. If the head of the ROB is not valid cout an error statement. Increment total_calls every time an instrution is attempted to commit and total_commits every time an instruction is committed.
void Pipeline :: Commit()
{
  #ifdef DEBUG
  cout << "--------------------------- COMMIT ----------------------------" << endl;
  #endif

  for (int i = 0; i < COMMIT_WIDTH && ROB.size() != 0; i++)
  {
    total_calls++;
    if (ROB.front().valid == 1)
    {
      #ifdef DEBUG
      cout <<"Commiting ROB ID " << ROB.front().ROB_ID << endl;
      #endif

      ROB.pop_front();
      total_commits++;
    }

    else
    {
      cout << "Error: can't commit." << endl;
    }
  }
  #ifdef DEBUG
  cout << "_______________________________________________________________" << endl;
  #endif
}

// Basic main() function which initializes a Pipeline object.
// Precondition: None.
// Postcondition: initializes a Pipeline obect, whos constructor will call the necessary functions to run the program. Latency and throuput are also provided inside the constructor. Loops until user wishes to exit.
int main()
{

  bool loop = true;
  while(loop)
  {

      Pipeline tester;

      string response;
      cout << "Would you like to test another program?" << endl;
      cout << "If yes, type 'y',  otherwise press any other key:" << endl;
      cout << "Response: ";
      cin >> response;


      if (response == "y")
      {
        loop = true;
      }
      else
      {
        loop = false;
        cout << "Goodbye!" << endl << endl;
      }
  }

  return 0;
}
