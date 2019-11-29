/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\ 
*                   Header File: Definition of the 'Tokenizer' Class                              *
*            CISC 3160 - Programming Languages - Fall 2019 - Toy Language Project                 *
* Author:                                                                                         *
*   Miriam Briskman                                                                               *
* Supervised by:                                                                                  *
*   Professor Neng-Fa Zhou                                                                        *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <cctype>             // ---> "Character handling functions"
#include <fstream>            // ---> Header providing file stream classes
#include <iomanip>            // ---> "parametric manipulators"
#include <iostream>           // ---> Input / Output streams controlling objects
#include <map>                // ---> The map container
#include <sstream>            // ---> Used by the 'to_string' function
#include <string>             // ---> "String types, character traits and a set of converting functions"
#include "Symbol_Table_Row.h" // ---> The "Symbol_Table_Row" Object

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                   The Token Class                                               *
* Purpose:                                                                                        *
*    To preserve both the content of a token (as a string) and its type (based on the Token       *
*    'Type' Categorizations below.)                                                               *
* Data variables (private):                                                                       *
*    1) 'type' (char) - denotes the type of the kept token (see Token 'Type' Categorizations)     *   
*    2) 'name' (string) - keeps the token's name and key to the corresponding Symbol Table value. *
* Functions (public):                                                                             *
*    1) Optional constructor.                                                                     *
*    2) 'getName' - receives no arguments and returns the content of 'name' (string).             *
*    3) 'getType' - receives no arguments and returns the content of 'type' (char).               *
* Class's Friends:                                                                                *
*    1) Tokenizer                                                                                 *
* Token 'Type' Categorizations:                                                                   *
*    1) Identifier ---> the 'type' variable contains the character '1'                            *
*    2) Literal    ---> the 'type' variable contains the character '2'                            *
*    3) Symbol     ---> if from [= ; + - * ( )], the 'type' variable contains the character '3'   *
*    4) Input End  ---> if the end of the input was reached, the 'type' variable contains '5'     *
*    5) Disallowed ---> if none of the above, the 'type' variable contains the character '0'      *
* --------------------------------------------------------                                        *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Token
{
    private:
        /* Data Fields: */
        string name;  // Contains the token's name and key to the corresponding Symbol Table value
        char type;    // Digit [0-5] categorizing the token (See "Token Categorizations" above)

    public:
        /* Optional Constructor: */
        Token (char type = '0', string name = "")
        {
            this -> name = name;
            this -> type = type;
        }
		
        /* Copy Constructor: */
        Token (const Token & t2)
        {
            this -> name = t2.name;
            this -> type = t2.type;
        }
		
        // Friend of the class:
        friend class Tokenizer;
		
        /* Accessors (Getters) Functions (all inline functions) */
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'getName':                                                                      *
        * Input: None.                                                                    *
        * Output: Return 'name' (string).                                                 *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        string getName (void) const { return name; }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'getType':                                                                      *
        * Input: None.                                                                    *
        * Output: Return 'type' (char).                                                   *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        char getType (void) const { return type; }
};  // End of the Token class

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                  The Tokenizer Class                                            *
* Purpose:                                                                                        *
*    Generation of tokens from a string of text based on the Definition of the Toy Language.      *
* Data variables (private):                                                                       *
*    1) 'iter' (long int) - iterates across the string of text to find tokens.                    *
*    2) 'code' (string) - contains the text with unprocessed tokens.                              *
*    3) 'currLine' (long int) - the number of the currently scanned line.                         *
*    4) 'currCol' (long int) - the number of the currently scanned column (character on the line.)*
*    5) 'currLinestr' (string) - copy of lines that remain to be processed.                       *
*    6) 'symbolTable' (map pointer) - pointer to the Symbol Table map object                      *
* Functions (public):                                                                             *
*    1) Optional Constructor. Arguments : name of file to process and symbol table pointer.       *
*    2) Copy Constructor.                                                                         *
*    3) 'next' - sets the Token argument's type and content to those the next token.              *
*    4) 'to_string' - returns the string object representing the parameter passed to it. Static.  *
*    5) 'getCurrLine' - returns the currently processed line.                                     *
*    6) 'getCurrCol' - returns the currently processed column.                                    *
*    7) 'getRemainingCode' - returns a string with code that remains to process.                  *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Tokenizer
{
    private:
        /* Data Fields: */
        long iter;                                  // iterates across the code source
        string code;                                // String variable containing source code
        long currLine;                              // The number of the currently scanned line
        long currCol;                               // The number of the currently scanned column
        string currLinestr;                         // Copy of the lines that remain to be processed
        map<string, Symbol_Table_Row>* symbolTable; // Pointer to the Symbol Table map object

    public:
        /* Optional Constructor: */
        Tokenizer (string fileName = "", map<string, Symbol_Table_Row>* symbolTable = 0)
        {
            this -> iter = -1;
            this -> currLine = 1;
            this -> currCol = -1;
            this -> symbolTable = symbolTable;
		   
            // Clean the filename from single or double quotes around it, and trim it from spaces:
            if (fileName != "")
            {
                int i = 0;
                while (fileName[i] == '\'' || fileName[i] == '"' || isspace(fileName[i]))
                {
                    fileName = fileName.substr(1);
                    i++;
                }
                
                i = fileName.length()-1;
                while (fileName[i] == '\'' || fileName[i] == '"' || isspace(fileName[i]))
                {
                    fileName = fileName.substr(0, fileName.length()-1);
                    i--;
                }
			   
                if (fileName.find(".txt") == string::npos)
                    fileName += ".txt"; // Add the extension '.txt' to the filename.
  			   	  
                ifstream infile(fileName.c_str());
                cout << "\t" << fileName.c_str() << endl; // Print the filename to the user.
                if (infile.is_open())
                {
                    // Copy the content of the file into 'code'.
                    code.assign ((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
                      
                    infile.close();
                    cout << endl << "The file " << endl
                         << "\t" << fileName << endl << "has successfully opened." 
                         << endl << endl << "Interpreting the code..." << endl << endl;
                }
                else
                    cout << endl << "Error: the file " << endl << "\t" << fileName << endl
                         << "whose you name you entered could not open." << endl;
            }
		   
            this -> currLinestr = this -> code;
        }    
		
        /* Copy Constructor: */
        Tokenizer (const Tokenizer & myTokenizr2)
        {
            this -> code = myTokenizr2.code;
            this -> iter = myTokenizr2.iter;
            this -> currLine = myTokenizr2.currLine;
            this -> currCol = myTokenizr2.currCol;
            this -> symbolTable = myTokenizr2.symbolTable;
            this -> currLinestr = this -> code;
        }    
		
        /* Accessors (Getters) Functions (all inline functions) */
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'getCurrLine':                                                                  *
        * Input: None.                                                                    *
        * Output: 'currLine' (long int).                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        long getCurrLine (void) { return currLine; }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'getCurrCol':                                                                   *
        * Input: None.                                                                    *
        * Output: 'currCol' (long int).                                                   *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        long getCurrCol (void) { return currCol; }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'getRemainingCode':                                                             *
        * Input: None.                                                                    *
        * Output: 'currLinestr' (string).                                                 *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        string getRemainingCode (void) { return currLinestr; }
		
        /* Function to Retrieve the Next Token */
		   		 
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'next':                                                                         *
        * Input: 't1' (Token object).                                                     *
        * Output: Put the currently processed token's information into 't1'.              *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void next (Token& t1)
        { 
            iter++; // Increment 'iter' for the next search below
            currCol++; // Increment 'currCol' as well to know on which place in the line we look
            if (iter >= code.length())
                t1.type = '5'; // Signifies the end of the token stream.
            else
            {
                map<string, Symbol_Table_Row>::iterator mapIter; // Declare map's iterator.
			   	   
                // Delete whitespace characters:
                while ((isspace(code[iter]) || code[iter] == 0) && iter < code.length())
                {
                    if (code[iter] == '\n')
                    {
                        // Increment the line number (for better error messages)			
                        currLine++;
                        // Restart the column counting
                        currCol = -1;
                        // Chop-off the old line from currLinestr:
                        currLinestr = currLinestr.substr(currLinestr.find("\n") + 1);
                    }
                    iter++;
                    currCol++;
                }
                
                // If the end of the file is reached,
                if (iter >= code.length())
                {
		   		   t1.type = '5'; // Signifies the end of the token stream.
                   return;
				}
				   
                // Search for tokens!
                switch(code[iter])
                {
                    case '=':
                    case ';':
                    case '+':
                    case '-':
                    case '*':
                    case '(':
                    case ')': t1.type = '3'; // This is a 'symbol' token!
                              t1.name = to_string(code[iter]);
                              break;
                    case '0': t1.type = '2'; // This is a 'numeric literal' token!
                              t1.name = "0";
                              break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': t1.type = '2'; // This is a 'numeric literal' token!
                              t1.name = ""; // Clean the content of the token
                              do
                              {
                                  t1.name += to_string(code[iter]);
                                  iter++;
                                  currCol++;
                              } while (iter < code.length() && isdigit(code[iter]));
                              iter--;
                              currCol--;
                              break;
                    case 'a':
                    case 'b':		
                    case 'c':	
                    case 'd':		
                    case 'e':
                    case 'f':		
                    case 'g':	
                    case 'h':	
                    case 'i':
                    case 'j':		
                    case 'k':	
                    case 'l':	
                    case 'm':
                    case 'n':		
                    case 'o':	
                    case 'p':	
                    case 'q':
                    case 'r':		
                    case 's':	
                    case 't':	
                    case 'u':
                    case 'v':		
                    case 'w':	
                    case 'x':	
                    case 'y':
                    case 'z':
                    case 'A':	
                    case 'B':	
                    case 'C':
                    case 'D':		
                    case 'E':	
                    case 'F':	
                    case 'G':
                    case 'H':		
                    case 'I':	
                    case 'J':		
                    case 'K':	
                    case 'L':	
                    case 'M':
                    case 'N':		
                    case 'O':	
                    case 'P':	
                    case 'Q':
                    case 'R':		
                    case 'S':	
                    case 'T':	
                    case 'U':	
                    case 'V':	
                    case 'W':
                    case 'X':		
                    case 'Y':	
                    case 'Z':	
                    case '_': t1.type = '1'; // This is an 'identifier' token!
                              t1.name = ""; // Clean the content of the token
                              do
                              {
                                  t1.name += to_string(code[iter]);
                                  iter++;
                                  currCol++;
                              } while (iter < code.length() && (isalnum(code[iter]) || code[iter] == '_'));
                              iter--;
                              currCol--;
		    					
                              // Check if that identifier already exists in the symbol table:
                              mapIter = (*symbolTable).find(t1.name);
                              // If not, create a new symbol table row:
                              if (mapIter == (*symbolTable).end())
                                  (*symbolTable)[t1.name] = Symbol_Table_Row ();
									    
						        break;
                    default: t1.type = '0'; // This is a disallowed symbol token!
                             t1.name = to_string(code[iter]);
                }
            }
        }
		
        // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
        // Following is a template function converting the passed parameter into a string object and     *
        //  returning it. It comes as an alternative to the C++11 'to_string' member function of         *
        //  the string object/class. The function declares an 'ostringstream' into which it streams      *
        //  the value of the generic parameter. This stream is returned from the function in a form      *
        //  of a C++ string.                                                                             *
        // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
        template <class T>
        static string to_string (T obj)
        {
            ostringstream mystream;
            mystream << obj;
            return mystream.str();
        }
};   // End of the Tokenizer class
#endif
