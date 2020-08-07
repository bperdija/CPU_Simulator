#define FETCH_WIDTH 3
#define ISSUE_WIDTH 3
#define COMMIT_WIDTH 3
#define FINITE_LIMIT 100

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
