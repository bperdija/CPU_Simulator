#include "Pipeline.h"

// Basic main() function which initializes a Pipeline object.
// Precondition: None.
// Postcondition: initializes a Pipeline obect, whos constructor will call the necessary functions to run the program. Latency and throuput are also provided inside the constructor. Loops until user wishes to exit.
int main()
{

  bool loop = true;
  while(loop)
  {

      Pipeline tester;

      // ask user if they want to run another file
      string response;
      cout << "Would you like to test another program?" << endl;
      cout << "If yes, type 'y',  otherwise press any other key:" << endl;
      cout << "Response: ";
      cin >> response;

      // if yes loop again and create a new pipeline instance
      if (response == "y")
      {
        loop = true;
      }

      // if not yes, end the loop
      else
      {
        loop = false;
        cout << "Goodbye!" << endl << endl;
      }
  }

  return 0;
} //youtube.com/watch?v=oHg5SJYRHA0 <--- very useful, I highly recommend watching this
