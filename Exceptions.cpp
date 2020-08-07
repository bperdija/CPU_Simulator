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
