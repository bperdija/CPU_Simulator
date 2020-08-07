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
