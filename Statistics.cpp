#include "Statistics.h"

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
