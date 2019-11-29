/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\ 
*                                    Program's Main File                                          *
*            CISC 3160 - Programming Languages - Fall 2019 - Toy Language Project                 *
* Author:                                                                                         *
*   Miriam Briskman                                                                               *
* Supervised by:                                                                                  *
*   Professor Neng-Fa Zhou                                                                        *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ctime>              // ---> Definitions of functions to get and manipulate date and time
#include <iostream>           // ---> Input / Output streams controlling objects
#include <map>                // ---> The map container
#include <string>             // ---> "String types, character traits and a set of converting functions"
#include "Parser.h"           // ---> The "Parser" Object
#include "Symbol_Table_Row.h" // ---> The "Symbol_Table_Row" Object

using namespace std;

/* Function Prototypes: */
void printInterpreterHeader (void);

int main (int argc, char *argv[])
{
     // A Symbol Table map:
     //     The name of an identifier (string) is the key, and a Symbol_Table_Row is a value.
     map <string, Symbol_Table_Row> symbolTable;
     // String holding the name of the file that the user wants to interpret:
     string fileName;
     // Two time types:
     time_t timer1, timer2;
     
     printInterpreterHeader(); // Print the Interpreter's Header
 	 
     if (argc > 1)
     {  
         fileName = argv[1];
         cout << endl << "The entered filename is:" << endl;
     }
     else
     {
         cout << "Hello! Please enter the name of the text file containing a" << endl 
              << "code to interpret and press ENTER. You can enter the file's" << endl 
			  << "name without the .txt extension: >> ";
         getline (cin, fileName); // Read the name of the file into 'filename'.
         cout << endl << "Thank you! The entered filename is:" << endl;
	 }


     // Assign the current time into timer1:
     time(&timer1);
 	 
     // Call the Parser (which calls the Tokenizer and the Semantic Analyzer.)
     Parser (fileName, &symbolTable);
     
     // Assign the current time into timer2:
     time(&timer2);
     
     // Print how many milliseconds the Parser worked:
     cout << endl << "(" << difftime(timer2,timer1) << " milliseconds.)" << endl;
     return 0;	
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
* 'printInterpreterHeader':                                                       *
* Input: None.                                                                    *
* Output: Print the header of the program at its launch.                          *
*         No object is returned.                                                  *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void printInterpreterHeader (void)
{
    cout << endl
	     << "~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl
         << "~*                                                         *~" << endl
         << "~*                Toy Language Interpreter                 *~" << endl
         << "~*                                                         *~" << endl
         << "~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl
         << "~*                                                         *~" << endl
         << "~*      CISC 3160 - Programming Languages - Fall 2019      *~" << endl
         << "~*                                                         *~" << endl
         << "~*                 Professor: Neng-Fa Zhou                 *~" << endl
         << "~*                                                         *~" << endl
         << "~*               Written by: Miriam Briskman               *~" << endl
         << "~*                                                         *~" << endl
         << "~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl << endl;
}
