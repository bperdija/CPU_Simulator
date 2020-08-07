#include "Pipeline.h"

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
