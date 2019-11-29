/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\ 
*             Header File: Definition of the 'Symbol_Table_Row' Class                             *
*        CISC 3160 - Programming Languages - Fall 2019 - Toy Language Project                     *
* Author:                                                                                         *
*   Miriam Briskman                                                                               *
* Supervised by:                                                                                  *
*   Professor Neng-Fa Zhou                                                                        *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SYMBOL_TABLE_ROW_H
#define SYMBOL_TABLE_ROW_H

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                               The Symbol_Table_Row Class                                        *
* Purpose:                                                                                        *
*    To preserve the attributes of identifiers. Each object is a row in the Symbol Table.         *
* Data variables (private):                                                                       * 
*    1) 'isInit' (bool) - tells if a variable was initialized.                                    *
*    2) 'intValue' (long long) - keeps the value that corresponds to identifier.                  *
* Functions (public):                                                                             *
*    1) Constructor.                                                                              *
*    2) 'isInitialized' -  returns 'true' if variable was initialized and false otherwise (bool). *
*    3) 'getValue' - returns the value of the variable (long long).                               *
*    4) 'initialize' - updates that the variable was initialized (void).                          *
*    5) 'setValue' - changes the value of the variable (void).                                    *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Symbol_Table_Row
{
	private:
        /* Data Fields: */
        bool isInit;         // Has the variable (identifier) been initialized before use?
        long long intValue;  // The value that corresponds to an identifier or literal

	public:
        /* Constructor */
        Symbol_Table_Row (bool isInitialized = false, long long intValue = 0)
        {
            this -> isInit = isInitialized; 
            this -> intValue = intValue;
        }
		
        /* Accessors (Getters) Functions (all inline functions) */
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'isInitialized':                                                                *
        * Input: None.                                                                    *
        * Output: True if 'isInit' is True; False otherwise.                              *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        bool isInitialized (void) { return isInit; }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'getValue':                                                                     *
        * Input: None.                                                                    *
        * Output: the content of 'intValue' (long long int).                              *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        long long getValue (void) { return intValue; }
		
        /* Mutators (Setters) Functions (all inline functions) */
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'initialize':                                                                   *
        * Input: None.                                                                    *
        * Output: Sets 'isInit' to True.                                                  * 
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void initialize (void) { isInit = true; }
        
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
        * 'setValue':                                                                     *
        * Input: 'intValue' (long long int).                                              *
        * Output: Sets the object's 'intValue' to the content of the argument.            * 
        *         No object is returned.                                                  *
        \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        void setValue (long long intValue) { this -> intValue = intValue; }
}; // End of the Symbol_Table_Row class

#endif
