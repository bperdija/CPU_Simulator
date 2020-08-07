#include "Instruction.h"

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
			#ifdef DEBUG
			cout << "Error: invalid instruction type." << endl << endl;
			#endif
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
		#ifdef DEBUG
		cout << "Error: Invalid opcode value." << endl << endl;
		#endif
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
