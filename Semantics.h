/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\ 
*                     Header File: Definition of the 'Semantics' Class                            *
*            CISC 3160 - Programming Languages - Fall 2019 - Toy Language Project                 *
* Author:                                                                                         *
*   Miriam Briskman                                                                               *
* Supervised by:                                                                                  *
*   Professor Neng-Fa Zhou                                                                        *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <cstdlib>            // ---> Standard General Utilities Library
#include <iostream>           // ---> Input / Output streams controlling objects
#include <map>                // ---> The map container
#include <stack>              // ---> The stack container
#include <string>             // ---> "String types, character traits and a set of converting functions"
#include "Symbol_Table_Row.h" // ---> The "Symbol_Table_Row" Object
#include "Tokenizer.h"        // ---> The "Tokenizer" Object

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                   The Semantics Class                                           *
* Purpose:                                                                                        *
*    To perform computations and variable initializations and detect semantic errors.             *
* Data Fields (private):                                                                          *
*    1) 'symbolTable' (map pointer) - pointer to the Symbol Table map object.                     *
*    2) 'myTokenizer' (Tokenizer pointer) - pointer to the Tokenizer that the Parser uses.        *
*    3) 'errors' (int pointer) - pointer to the  number of errors detected by the parser.         *
*    4) 'tokens' (Token stack) - Tokens processed by the semantics analyzer.                      *
*    5) 'precedence' (short stack) - integers representing the precedence of Tokens in 'tokens'.  *
* Functions (public):                                                                             *
*    1) Optional Constructor. Arguments: map<string, Symbol_Table_Row>*, Tokenizer*, int* .       *
*    2) Copy Constructor.                                                                         *
*    3) 'initialize' - Empty the 'tokens' and 'precedence' stacks and push 4 into 'precedence'.   *
*    4) 'process' (bool) - process tokens; do necessary computations; print semantic errors.      *
* Functions (private):                                                                            *
*    1) 'toggleUnary' - evaluate a sequence of unary operations; turn two "-" into "+".           *
*    2) 'processBinary' (bool) - add, subtract or multiply two numbers; print errors, if found.   *
*    3) 'processUnary' (bool) - negate a number; print errors, if found.                          *
*    4) 'printSemanticError' - print an error message regarding uninitialized variable 'name'.    *
* --------------------------------------------------------                                        *
* Precedence Levels:                                                                              *
* 0 ---> Opening Parenthesis (                                                                    *
* 1 ---> Unary Operations +, -                                                                    *
* 2 ---> Multiplication *                                                                         *
* 3 ---> Binary Addition and Subtraction +, -                                                     *
* 4 ---> Closing Parenthesis ) or Semicolon ;                                                     *
* -1 ---> Unknown Precedence Yet                                                                  *
* --------------------------------------------------------                                        *
* Bottom of Stack Sentinel: 4                                                                     *
* --------------------------------------------------------                                        *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Semantics
{
    private:
        /* Data Fields: */
        map<string, Symbol_Table_Row>* symbolTable; // Pointer to the Symbol Table map object
        Tokenizer* myTokenizer;                     // Pointer to the Tokenizer that the Parser uses
        int* errors;                                // Number of errors detected by the parser
        stack<Token> tokens;                        // Tokens processed by the semantics analyzer
        stack<short> precedence;                    // Precedence of corresponding Tokens in 'tokens'.
	    
    public:
        /* Optional Constructor: */
        Semantics (map<string, Symbol_Table_Row>* symbolTable = 0, 
                   Tokenizer* myTokenizer = 0, int* errors = 0)
        {
            this -> symbolTable = symbolTable;
            this -> myTokenizer = myTokenizer;
            this -> errors = errors;
            precedence.push (4); // Push 4 to the bottom of the stack (sentinel)
        }
        
        /* Copy Constructor: */
        Semantics (const Semantics & analyzer)
        {
            this -> symbolTable = analyzer.symbolTable;
            this -> myTokenizer = analyzer.myTokenizer;
            this -> errors = analyzer.errors; 
            this -> tokens = analyzer.tokens;
            this -> precedence = analyzer.precedence;
        }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'initialize':                                                                   *
        * Input: None.                                                                    *
        * Output: Clear both 'tokens' and 'precedence' by copying empty stacks to them.   *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void initialize (void)
        {
            stack<Token> s1;
            tokens = s1;
            stack<short> s2;
            precedence = s2;
            precedence.push(4); // Push 4 to the bottom of the stack (sentinel)
        }
        
        /* Computational Functions */
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'process':                                                                      *
        * Input: None.                                                                    *
        * Output: Process a token. Print a semantic error if detected.                    *
        *         Return True if no semantic error detected; return False otherwise.      *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool process (const Token & t1, short precedenceOfToken)
        {	 
            // As long as the precedence of the argument is not 4, and if it is less than this
            //    at the top of the stack or the precedence at the top of the stack is zero:
            if ((precedenceOfToken < precedence.top() || precedence.top() == 0) && precedenceOfToken != 4)
            {
                if (t1.getName() != "(") // If the argument is not an opening parenthesis,
                    tokens.push(t1); // push the token into the stack.
                if (precedenceOfToken != -1) // If the precedence is not -1 (denoting a variable),
                    precedence.push(precedenceOfToken); // push the precedence into the stack.
            }
            else // Otherwise,
            {
                switch (precedenceOfToken)
                {
                    case 1: if (t1.getName() == "-") // If we have a unary minus symbol,
                                toggleUnary(); // Call 'toggleUnary' (see below)
                            break;
                    case 2: if (!processUnary ()) // If a multiplication *, process unary symbols,
                                return false;
                            while (precedence.top() == 2) // Compute all the previous multiplications,
                                if (!processBinary ())
                                    return false;
                            tokens.push (t1); // and push the multiplication sign * token into the stack
                            precedence.push(2); // along with the correpsonding precedence.
                            break;
                    case 3: while (precedence.top() == 1) // If a unary + or -,
                            {
                                if (!processUnary ()) // Process any unary symbol already in the stack,
                                    return false;
                            }
                            while (precedence.top() == 2) // Also process any multiplications in the stack,
                                if (!processBinary ())
                                    return false;
							  	     
                            while (precedence.top() == 3) // Finally, process additions and subtractions
                                if (!processBinary ())
                                    return false; 
                            tokens.push (t1); // and push the binary + or - token into the stack.
                            precedence.push(3); // along with the correpsonding precedence.
                            break;
                    case 4: // As long as we haven't reached an opening parens or the stack's bottom,
                            while (precedence.top() != 0 && precedence.top() != 4)
                            {
                                // Compute all the binary operations,
                                if (precedence.top() == 3 || precedence.top() == 2)
                                {
                                    if (!processBinary ())
                                        return false;
                                }
                                else if (precedence.top() == 1) // and unary operations in the stack.
                                    if (!processUnary ())
                                        return false;
                            }
								  
                            if (precedence.top() == 0) // Pop the opening parenthesis,
                                precedence.pop();
                            else // precedence.top() == 4 (got to bottom of stack)
                            {
                                Token temp1 = tokens.top(); // The r-value
                                tokens.pop(); // pop it.
                                Token temp2 = tokens.top(); // The l-value
                                tokens.pop(); // pop it. Now the stack of tokens is empty.
                                long long tempNum1 = 0;
					   	             
                                // Retrieve the value of the token of the r-value.
                                if (temp1.getType() == '1') // If an identifier, consult the Symbol Table.
                                {
                                    if (((*symbolTable)[temp1.getName()]).isInitialized())
                                        tempNum1 = ((*symbolTable)[temp1.getName()]).getValue();
                                    else
                                    {
                                        printSemanticError (temp1.getName());
                                        return false;
                                    }
                                }
                                else // Otherwise, convert the numeric literal into an integer.
                                    tempNum1 = atol (temp1.getName().c_str());
						             
                                ((*symbolTable)[temp2.getName()]).initialize();
                                ((*symbolTable)[temp2.getName()]).setValue(tempNum1); // Set the l-value!
                            }
                }
            }
            return true;
        }
	    
    private:
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'toggleUnary':                                                                  *
        * Input: None.                                                                    *
        * Output: Delete a unary minus if another unary minus is added (becomes plus).    *
        *         Otherwise, add that minus back (since + * - = -).                       *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void toggleUnary (void)
        {
            Token temp = tokens.top();
            tokens.pop();
            if (temp.getName() == "-")
                precedence.pop();
            else // temp.getName() == "+"
                tokens.push( Token('3', "-") );
        }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'processBinary':                                                                *
        * Input: None.                                                                    *
        * Output: Process a binary operation: add, subtract or multiply two numbers.      *
        *         Return the answer as a literal into the stack.                          *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool processBinary (void)
        {
            Token temp1 = tokens.top(); // Operand 1
            tokens.pop();
            Token temp2 = tokens.top(); // Binary Operation
            tokens.pop();
            Token temp3 = tokens.top(); // Operand 2
            tokens.pop();
            long long tempNum1 = 0, tempNum2 = 0;
		 	 
            if (temp1.getType() == '1') // If an identifier, consult the Symbol Table.
            {
                if (((*symbolTable)[temp1.getName()]).isInitialized())
                    tempNum1 = ((*symbolTable)[temp1.getName()]).getValue();
                else
                {
                    printSemanticError (temp1.getName());
                    return false;
                }
            }
            else // temp1.getType() == '2' [Otherwise, convert the numeric literal into an integer.]
                tempNum1 = atol(temp1.getName().c_str());
                 
            if (temp3.getType() == '1') // If an identifier, consult the Symbol Table.
            {
                if (((*symbolTable)[temp3.getName()]).isInitialized())
                    tempNum2 = ((*symbolTable)[temp3.getName()]).getValue();
                else
                {
                    printSemanticError (temp3.getName());
                    return false;
                }
            }
            else // temp3.getType() == '2' [Otherwise, convert the numeric literal into an integer.]
                tempNum2 = atol(temp3.getName().c_str());
		 	 
            switch (temp2.getName()[0]) // Push the computed literal into the stack, based on the operation.
            {
                case '+': tokens.push(Token ('2', Tokenizer::to_string(tempNum2 + tempNum1)));
                          break;
                case '-': tokens.push(Token ('2', Tokenizer::to_string(tempNum2 - tempNum1)));
                          break;
                case '*': tokens.push(Token ('2', Tokenizer::to_string(tempNum2 * tempNum1)));
            }
			      
            precedence.pop();
            return true;
        }
		
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'processBinary':                                                                *
        * Input: None.                                                                    *
        * Output: Process a binary operation: add, subtract or multiply two numbers.      *
        *         Return the answer as a literal into the stack.                          *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool processUnary (void)
        {
            Token temp1 = tokens.top();
            tokens.pop();
            Token temp2 = tokens.top(); // Unary Op
            tokens.pop();
            long long tempNum1 = 0;
		 	 
            if (temp1.getType() == '1') // If an identifier, consult the Symbol Table.
            {
                if (((*symbolTable)[temp1.getName()]).isInitialized())
                    tempNum1 = ((*symbolTable)[temp1.getName()]).getValue();
                else
                {
                    printSemanticError (temp1.getName());
                    return false;
                }
            }
            else // temp1.getType() == '2' [Otherwise, convert the numeric literal into an integer.]
                tempNum1 = atol(temp1.getName().c_str());
                 
            tokens.push(Token ('2', Tokenizer::to_string(0-tempNum1))); // Push the negation!
             
            precedence.pop();
            return true;
        }
		
        /* Semantic Error Printing Function */
		
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'printSemanticError':                                                           *
        * Input: 'name' (string) - the name of the uninitialized identifier.              *
        * Output: Print the corresponding semantic error.                                 *
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void printSemanticError (string name)
        {
            if (*errors == 0)
                cout << "------------------------ ERRORS ------------------------" << endl;
            (*errors)++;
            cout << endl << *errors << ") Semantic Error:" << endl
                 << "The variable" << endl << endl << "\t" << name
                 << endl << endl << "on line " << (*myTokenizer).getCurrLine() << ":" << endl << endl
                 << "\t" << (*myTokenizer).getRemainingCode()
                                             .substr(0, (*myTokenizer).getRemainingCode().find("\n")) 
                 << endl << endl << "has not been initialized before the assignment!" << endl
                 << endl << "--------------------------------------------------------" << endl;
        }
};   // End of the Semantics class

#endif
