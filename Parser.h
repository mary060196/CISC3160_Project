/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\ 
*                      Header File: Definition of the 'Parser' Class                              *
*            CISC 3160 - Programming Languages - Fall 2019 - Toy Language Project                 *
* Author:                                                                                         *
*   Miriam Briskman                                                                               *
* Supervised by:                                                                                  *
*   Professor Neng-Fa Zhou                                                                        *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PARSER_H
#define PARSER_H

#include <iostream>           // ---> Input / Output streams controlling objects
#include <map>                // ---> The map container
#include <string>             // ---> "String types, character traits and a set of converting functions"
#include "Semantics.h"        // ---> The "Semantics" (Semantic Analyzer) Object
#include "Symbol_Table_Row.h" // ---> The "Symbol_Table_Row" Object
#include "Tokenizer.h"        // ---> The "Tokenizer" Object

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                   The Parser Class                                              *
* Purpose:                                                                                        *
*    To create a parse tree and detect syntax errors.                                             *
* Data Fields (private):                                                                          *
*    1) 'symbolTable' (map pointer) - pointer to the Symbol Table map object.                     *
*    2) 't1' (Token) - a Token object holding the currently processed token by the Parser.        *
*    3) 'myTokenizer' (Tokenizer) - the Tokenizer that the Parser uses.                           *
*    4) 'sAnalyzer' (Semantics) - the Semantic Analyzer that the Parser uses.                     *
*    5) 'errors' (int) - total number of errors detected by Parser and Semantic Analyzer.         *
*    6) 'OpenParens' (int) - counts the number of open parentheses.                               *
* Functions (public):                                                                             *
*    1) Constructor. Arguments: fileName(string), symbolTable (pointer to / address of map).      *
* Functions (private):                                                                            *
*    1) 'skipAfterNextSemicolon' - panic mode error recovery: parse after next semicolon.         *
*    2) 'printErrorsHeader' - print the title of the error displaying section.                    *
*    3) 'printErrorsFooter' - print the total number of errors and footer of the error section.   *
*    4) 'printSyntaxError' - print a syntax error based on the passed encoded 'message'.          *
*    5) 'printErrorLine' - print the line and column on which the syntax error occured.           *
*    6) 'matchID' - match a l-value of an assignment.                                             *
*    7) 'matchAssign' - match an assignment symbol.                                               *
*    8) 'matchExp' - match an expression.                                                         *
*    9) 'matchSemicolon' - match a semicolon.                                                     *
*    10) 'matchTerm' - match a term.                                                              *
*    11) 'matchExpPrime' - match expression prime.                                                *
*    12) 'matchFact' - match a factor.                                                            *
*    13) 'matchTermPrime' - match term prime.                                                     *
*    14) 'printValuesOfVariables' - print all the variables's values at end of interpretation.    *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Parser
{
    private:
        /* Data Fields: */
        map<string, Symbol_Table_Row>* symbolTable; // Pointer to the Symbol Table map object
        Token t1;                                   // The token whose info the parser will check at a time
        Tokenizer myTokenizer;                      // The Tokenizer that the Parser uses
        Semantics sAnalyzer;                        // The Semantic Analyzer that the Parser uses
        int errors;                                 // Number of errors detected by the parser
        int OpenParens;                             // Counts the number of open parentheses
	    
	    
    public:
        /* Constructor */
        Parser (string fileName, map<string, Symbol_Table_Row>* symbolTable)
        {
             this -> symbolTable = symbolTable;
             this -> errors = 0;
             this -> OpenParens = 0;
             this -> t1 = Token ('0', "");
             this -> myTokenizer = Tokenizer (fileName, symbolTable);
             
             this -> sAnalyzer = Semantics (this -> symbolTable, &myTokenizer, &errors);
             
             this -> myTokenizer.next (t1); // Retrieve the first token.
             
             
             while (this -> t1.getType () != '5')
                 if (matchID () && matchAssign () && matchExp () && matchSemicolon ())
                 {
                      if (OpenParens > 0)
                            printSyntaxError (10);
		      myTokenizer.next(t1); // Retrieve the next token.
                 }
                 else
                      skipAfterNextSemicolon ();
			 
             if (this -> errors > 0)
                 printErrorsFooter ();
             else
                 printValuesOfVariables ();
        }
	    
    private:
			
		/* Error Recovery Function */	
			
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'skipAfterNextSemicolon':                                                       *
        * Input: None.                                                                    *
        * Output: Panic Mode Error Recovery: Continue Parsing after Next Semicolon.       *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void skipAfterNextSemicolon (void)
        {
             while (t1.getType() != '5' && t1.getName()[0] != ';')
                myTokenizer.next(t1); // Retrieve the next token.
             myTokenizer.next(t1); // We need the token that is right after the semicolon.
             sAnalyzer.initialize(); // Clear the information in the Semantic Analyzer.
             OpenParens = 0;
        }
           
        /* Syntax Errors Printing Functions */
	    
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'printErrorsHeader':                                                            *
        * Input: None.                                                                    *
        * Output: Print the header of the error messages' section                         *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void printErrorsHeader (void)
        {
             cout << "------------------------ ERRORS ------------------------" << endl;
        }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'printErrorsFooter':                                                            *
        * Input: None.                                                                    *
        * Output: Print the footer of the error messages' section                         *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void printErrorsFooter (void)
        {
             cout << "Totally: " << errors << " Errors."
                  << endl << "--------------------------------------------------------" << endl;
        }

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'printSyntaxError':                                                             *
        * Input: 'message' (char) - the type of compilation syntax error                  *
        *        'type' (char) - either 's' (for sybmol) or 'n' (for number)              *
        *        'side' (char) - either 'l' (for left) or 'r' (for right)                 *
        * Output: Display of the corresponding syntax error on the screen.                *
        *         No object is returned.                                                  *
        * Message encoding:                                                               *
        * '0' -> If a prohibited character (symbol) was encountered                       *
        * '1' -> If the left side of the assignment contains a symbol or literal          *
        * '2' -> If the left side of the assignment contains an extra variable            *
        * '3' -> If the right side of the assignment contains an extra expression         *
        * '4' -> If there is an extra closing parenthesis                                 *
        * '5' -> If there is an extra semicolon                                           *
        * '6' -> If there is an inappropriate expression before the semicolon             *
        * '7' -> If the semicolon is missing                                              *
        * '8' -> If there is a lone identifier without an assignment                      *
        * '9' -> If there is no expression to the right of the assignment symbol          *
        * '10' -> If there are extra opening parentheses                                  *
        * '11' -> If there is an opening parenthesis without a binary symbol before it    *
        * '12' -> If there are too many assignment operators                              *
        * '13' -> If there is a '*' after a binary operator                               *
        * '14' -> If there is an inappropriate expression before a closing parenthesis    *
        * '15' -> If a semicolon comes before an assignment symbol                        *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void printSyntaxError (char message, char type = 's')
        {
             if (errors == 0)
                 printErrorsHeader();
             errors++;
             cout << endl << errors << ") Syntax Error:" << endl;
		 	 
             switch (message)
             {
                    case 0: cout << "You are using a prohibited symbol: " 
                                 << endl << endl << "\t" << t1.getName()[0] << " (ASCII " 
                                 << (short)t1.getName()[0] << ")";
                            break;
                    case 1: cout << "The left hand side of an assignment cannot contain";
                            switch (type)
                            {
                                case 's': cout << endl << endl << "\t" << t1.getName() << " (ASCII "
                                               << (short)t1.getName()[0] << ")" << endl << endl
                                               << "(a symbol)";
                                          break;
                                case 'n': cout << endl << endl << "\t" << t1.getName() << endl << endl
                                               << "(a literal)";
                            }
                            break;
                    case 2: cout << "There is an extra variable:" << endl << endl << "\t" << t1.getName();
                            break;
                    case 3: cout << "There is an extra expression:" << endl << endl << "\t" << t1.getName();
                            break;
                    case 4: cout << "There is an extra closing parenthesis" << endl << endl << "\t)";
                            break;
                    case 5: cout << "There is an extra semicolon" << endl << endl << "\t;";
                            break;
                    case 6: cout << "A semicolon" << endl << endl << "\t;" << endl << endl
                                 << "cannot come after an inappropriate expression";
                            break;
                    case 7: cout << "A semicolon" << endl << endl << "\t;" << endl << endl
                                 << "is missing";
                            break;
                    case 8: cout << "There is a lone identifier";
                            break;
                    case 9: cout << "There is no expression to the right of the assignment symbol";
                            break;
                    case 10: cout << "There are " << OpenParens << " extra opening parentheses";
                             break;
                    case 11: cout << "An opening parenthesis" << endl << endl << "\t(" << endl << endl
                                  << "cannot follow an expression without a binary operation";
                             break;
                    case 12: cout << "There is an extra assignment operator" 
                                  << endl << endl << "\t=";
                             break;
                    case 13: cout << "A multiplication symbol" << endl << endl << "\t*" << endl << endl
                                  << "cannot follow another binary operation"; 
                             break;
                    case 14: cout << "A closing parenthesis" << endl << endl << "\t)" << endl << endl
                                  << "cannot come after an inappropriate expression";
                             break;
                    case 15: cout << "A semicolon" << endl << endl << "\t;" << endl << endl
                                  << "must not come before an assignment operator =";
             }
		 	 
             cout << endl << endl << "on line " << myTokenizer.getCurrLine() << ":" << endl << endl;
             if (message != 10) // Because we do not know which '(' is the excessive one.
                 printErrorLine ();
             else 
                 cout << "\t" 
                      << myTokenizer.getRemainingCode().substr(0, myTokenizer.getRemainingCode().find("\n")) 
                      << endl;
             cout << endl << "--------------------------------------------------------" << endl;
        }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'printErrorLine':                                                               *
        * Input: None.                                                                    *
        * Output: Display of the corresponding syntax error's line on the screen.         *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void printErrorLine (void)
        { 	         
             // Extract the first line from the remaining code:
             string firstLine 
                 = myTokenizer.getRemainingCode().substr(0, myTokenizer.getRemainingCode().find("\n"));
			      
             // Replace tab characters with spaces (since we do not know how long is a tab):
             while (firstLine.find("\t") != string::npos)
                 firstLine[firstLine.find("\t")] = ' ';
			 			
             // Print that line:
             cout << "\t" << firstLine << endl;
 	         
             // Also, print a caret under the column of the error:
             cout << "\t";
             for (int i = 0; i < myTokenizer.getCurrCol(); i++)
                 cout << " ";
             cout << "^" << endl;
        }
	    
        /* Token Matching Functions */
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'matchID':                                                                      *
        * Input: None.                                                                    *
        * Output: Match the l-value of the assignment (must be an identifier.)            *
        *         Return 'true' if no errors were detected; return false otherwise.       *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool matchID (void)
        {
            switch (t1.getType())
            {
                 case '1': sAnalyzer.process(t1, -1); // Call the Semantic Analyzer to process this.
                           myTokenizer.next(t1); // Retrieve the next token.
                           return true;
                 case '0': printSyntaxError (0);
                           break; 
                 case '2': printSyntaxError (1, 'n');
                           break;
                 case '3': switch (t1.getName()[0])
                           {
                                case ';': printSyntaxError (5);
                                          return false;
                                default:  printSyntaxError (1, 's');
                           }
                           break;
            }
            myTokenizer.next(t1); // Retrieve the next token.
            return false;
        }
		
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'matchAssign':                                                                  *
        * Input: None.                                                                    *
        * Output: Match the assignment symbol.                                            *
        *         Return 'true' if no errors were detected; return false otherwise.       *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool matchAssign (void)
        {
            switch (t1.getType())
            {
                 case '1': printSyntaxError (2);
                           break;
                 case '0': printSyntaxError (0);
                           break; 
                 case '2': printSyntaxError (1, 'n');
                           break;
                 case '3': switch (t1.getName()[0])
                           {
                                case '=': myTokenizer.next(t1); // Retrieve the next token.
                                          return true;
                                case ';': printSyntaxError (15);
                                          return false;
                                default:  printSyntaxError (1);
                           }
                           break;
                 case '5': printSyntaxError (8);
            }
            myTokenizer.next(t1); // Retrieve the next token.
            return false;
        }
		
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'matchExp':                                                                     *
        * Input: None.                                                                    *
        * Output: Match an expression.                                                    *
        *         Return 'true' if no errors were detected; return false otherwise.       *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool matchExp (void)
        {
            if (t1.getType() == '5')
            {
                 printSyntaxError (9);
                 return false;		
            }
			   
            if (matchTerm () && matchExpPrime ())
                 return true;
            return false;
        }
		
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'matchSemicolon':                                                               *
        * Input: None.                                                                    *
        * Output: Match a semicolon symbol.                                               *
        *         Return 'true' if no errors were detected; return false otherwise.       *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool matchSemicolon (void)
        {
            sAnalyzer.process(t1, 4); // Call the Semantic Analyzer to process this.
            return true;
        }
		
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'matchTerm':                                                                    *
        * Input: None.                                                                    *
        * Output: Match a term.                                                           *
        *         Return 'true' if no errors were detected; return false otherwise.       *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool matchTerm (void)
        {
            if (matchFact () && matchTermPrime ())
                 return true;
            return false;
        }
	    
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'matchExpPrime':                                                                *
        * Input: None.                                                                    *
        * Output: Match a pattern following an expression (expression prime).             *
        *         Return 'true' if no errors were detected; return false otherwise.       *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool matchExpPrime (void)
        {
            switch (t1.getType())
            {
                 case '0': printSyntaxError (0);
                           break;
                 case '1': 
                 case '2': printSyntaxError (3);
                           break;
                 case '3': switch (t1.getName()[0])
                           {
                                case ';': 
                                case '*': return true;
                                case '(': printSyntaxError (11);
                                          break;
                                case ')': if (OpenParens > 0)
                                          {
                                               OpenParens--;
                                               sAnalyzer.process(t1, 4); // Call the Semantic Analyzer.
                                               myTokenizer.next(t1); // Retrieve the next token.
                                               return true;
                                          }
                                          else
                                          {
                                               printSyntaxError (4);
                                               return false;
                                          }
                                case '+':
                                case '-': sAnalyzer.process(t1, 3); // Call the Semantic Analyzer.
                                          myTokenizer.next(t1); // Retrieve the next token.
                                          if (matchTerm () && matchExpPrime ())
                                               return true;
                                          return false;
                                case '=': printSyntaxError (12);
                                // No default case since we covered all possible cases for type 3.
                           }
                           break;
                 case '5': printSyntaxError (7);
            }
            return false;
        }
	    
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'matchFact':                                                                    *
        * Input: None.                                                                    *
        * Output: Match a factor.                                                         *
        *         Return 'true' if no errors were detected; return false otherwise.       *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool matchFact (void)
        {
            switch (t1.getType())
            {
                 case '1': 
                 case '2': sAnalyzer.process(t1, -1); // Call the Semantic Analyzer to process this.
                           myTokenizer.next(t1); // Retrieve the next token.
                           return true;
                 case '0': printSyntaxError (0);
                           break;
                 case '3': switch (t1.getName()[0])
                           {
                                case ';': printSyntaxError (6);
                                          break;
                                case ')': printSyntaxError (14);
                                          break;
                                case '(': OpenParens++;
                                          sAnalyzer.process(t1, 0); // Call the Semantic Analyzer.
                                          myTokenizer.next(t1); // Retrieve the next token.
                                          if (matchExp ())
                                               return true;
                                          return false;
                                case '+':
                                case '-': sAnalyzer.process(t1, 1); // Call the Semantic Analyzer.
                                          myTokenizer.next(t1); // Retrieve the next token.
                                          if (matchFact ())
                                               return true;
                                          return false;
                                case '=': printSyntaxError (12);
                                          break;
                                case '*': printSyntaxError (13);
                                // No default case since we covered all possible cases for type 3.
                           }
                           break;
                 case '5': printSyntaxError (7);
            }
            return false;
        }
	    
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'matchTermPrime':                                                               *
        * Input: None.                                                                    *
        * Output: Match a pattern following a term (term prime).                          *
        *         Return 'true' if no errors were detected; return false otherwise.       *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool matchTermPrime (void)
        {
            switch (t1.getType())
            {
                 case '0': printSyntaxError (0);
                           break;
                 case '1': 
                 case '2': printSyntaxError (3);
                           break;
                 case '3': switch (t1.getName()[0])
                           {
                                case '*': sAnalyzer.process(t1, 2); // Call the Semantic Analyzer.
                                          myTokenizer.next(t1); // Retrieve the next token.
                                          if (matchFact () && matchTermPrime ())
                                               return true;
                                          return false;
                                case ')': if (OpenParens > 0)
                                          {
                                               OpenParens--;
                                               sAnalyzer.process(t1, 4); // Call the Semantic Analyzer.
                                               myTokenizer.next(t1); // Retrieve the next token.
                                               return true;
                                          }
                                          else
                                          {
                                               printSyntaxError (4);
                                               return false;
                                          }
                                case '+':
                                case '-':
                                case ';': return true;
                                case '(': printSyntaxError (11);
                                          break;
                                case '=': printSyntaxError (12);
                                // No default case since we covered all possible cases for type 3.
                           }
                           break;
                 case '5': printSyntaxError (7);
            }
            return false;
        }
	    
        /* Variables's Values Printing Function */
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'printValuesOfVariables':                                                       *
        * Input: None.                                                                    *
        * Output: Print the value of all the variables after the assignments were made.   *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void printValuesOfVariables (void)
        {
            for (map<string, Symbol_Table_Row>::iterator it = (*symbolTable).begin(); 
                 it != (*symbolTable).end(); 
                 it++)
                     cout << it->first << " = " << (it->second).getValue() << endl;
        }
};   // End of the Parser class

#endif
