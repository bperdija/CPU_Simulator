#include "IQueue.h"

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
