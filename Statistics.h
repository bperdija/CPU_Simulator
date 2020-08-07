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
#include <iomanip> //needed for setprecision()

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
