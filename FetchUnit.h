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
