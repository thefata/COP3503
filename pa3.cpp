//
//  pa3.cpp
//  Lexical Analysis
//
//  Created by Nicholas Fata on 3/11/17.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <exception>

#include "pa3.h"

using namespace std;

/*
 * Implementation of the Stack class, which represents a simple stack data structure.
*/

// Holds the data contained within the stack.
vector<string> stack;

/*
 This is the constructor for the Stack class.
 The constructor is empty as there is no object setup necessary when constructing a new Stack object.
 */
Stack::Stack() {}
// Corresponding destructor for the Stack class.
Stack::~Stack() {}

/*
 This method takes a parameter of type string, which is the value to add to the stack.
 Adds a new value to the "top" of the stack. In essence, by adding a new value to the stack, the value is appended to the end of the vector.
 */
void Stack::push(string value) {
    stack.push_back(value);
}

/*
 Returns the value on the "top" of the stack. In essence, it returns the last value of the string vector called stack, and deletes it accordingly.
 */
string Stack::pop() {
    // Obtains the value on the "top" of the Stack.
    string value = stack.at(stack.size() - 1);
    // Deletes the value at the end of the vector, the "top" of the Stack.
    stack.pop_back();
    // Returns the value from the top of the Stack.
    return value;
}

/*
 This method takes a parameter of type string, which is the string to find within the stack.
 Checks whether the stack contains the string specified in the parameter.
 Returns true if the stack contains the specified string, false otherwise.
*/
bool Stack::contains(string value) {
    // Retrieves the size of the Stack and assigns it to a specific variable.
    int size = int(stack.size());
    
    // Loops through all elements in the Stack.
    for (int i = 0; i < size; ++i)
    {
        // If this condition is true, the Stack does contain the string specified in the parameter. Therefore, since a match was found, the function can immediately return true;
        if (stack.at(i) == value)
            return true;
    }
    
    // If the entire Stack was searched without finding the specified string, then the Stack does not contain the the specified string, therefore the function returns false.
    return false;
}

/*
 This method takes in a Stack pointer parameter, syntaxErrors, which is the Stack object that contains all of the syntax errors.
 This method computes the depth of the nested loops in a specified piece of code. If there are any syntax errors relating to the misuse of keywords or inncorrect ordering of keywords, those syntax errors are placed onto the Stack syntaxErrors.
 Returns an integer representing the depth of the nested loop. If there is no complete loop in the code segment, 0 is returned.
 This Stack method is only called by the Stack containing the keywords of the code.
*/
int Stack::depthOfLoop(Stack* syntaxErrors, bool misspelledFor, bool misspelledBegin, bool misspelledEnd){
    
    // Retrieves the size of the Stack.
    int size = int(stack.size());
    // Tracks the keyword preceding the one being analyzed.
    string precedingString = "";
    // Tracks the keyword following the one being analyzed.
    string nextString = "";
    // Tracks the current depth of the loop.
    int depth = -1;
    
    // Checks whether the keywords Stack does not contain a keyword, since all three keywords are necessary for the creation of a loop.
    if (!contains("BEGIN") || !contains("END"))
    {
        // These statements figure out which keyword(s) are missing, and if a keyword is missing, the missing keyword is placed on the syntaxErrors Stack.
        if (!contains("BEGIN"))
            syntaxErrors->push("BEGIN");
        if (!contains("END"))
            syntaxErrors->push("END");
        
        // This statement will return -1, as the lack of any of the keywords would signify no complete loops in the code.
        return -1;
    }
    
    // Checks whether there are any incomplete loops in the code before the Stack is cleaned for loop calculation.
    for (int i = 0; i < size; ++i)
    {
        // Tracks the strings after the current string and after the next string.
        string next = "";
        string afterNext = "";
        
        // Checks whether the next string exists in the stack.
        if (i+1 < size)
            next = stack[i+1];
        else
            next = "";
        
        // Checks whether the string after the next string exists in the stack.
        if (i+2 < size)
            afterNext = stack[i+2];
        else
            afterNext = "";
        
        // Checks whether the current keyword is "FOR".
        if (stack[i] == "FOR")
        {
            // Checks whether the next string is "FOR" or it doesn't exist. If so, the "BEGIN" and "END" keywords are missing, and are thus pushed onto the syntaxErrors Stack.
            if (next == "" || next == "FOR")
            {
                syntaxErrors->push("BEGIN");
                syntaxErrors->push("END");
            }
            // If the current string is "FOR" and the two strings after the "FOR" keyword are "BEGIN". If so, then an "END" keyword is missing and is subsequently pushed onto the syntaxErrors Stack.
            else
            {
                if (next == "BEGIN" && afterNext == "BEGIN")
                    syntaxErrors->push("END");
            }
        }
    }
    
    // The keywordsForLoopDepth stack is cleaned up, that is, only the valid and complete loop declarations will remain.
    cleanTheLoop(syntaxErrors);
    
    // Retrieves the size of the Stack which was altered after the Stack was "cleaned" for effective loop calculation.
    size = int(stack.size());
     
    // Variables keep track of the current depth of a loop and the largest depth of any loop so far in the code.
    int currentDepth = -1;
    
    // Loops through the syntactically correct loops, tracking the current depth of the loop.
    for (int i = 0; i < size; ++i)
    {
        
        // If the current keyword is "FOR" then the current depth is increased by one.
        if (stack[i] == "FOR")
            ++currentDepth;
        
        // If the current keyword is "END" then the current depth is decreased by one as you exit the loop.
        if (stack[i] == "END")
            --currentDepth;
        
        // If the current depth is greater than the depth, then the current depth is assigned to the depth variable.
        if (currentDepth > depth)
            depth = currentDepth;
        
    }
    
    // If the entire stack of valid keywords is traversed and the depth is not -1, then there is an imbalance of "FOR" keywords, and the depth will be adjusted appropriately to account for the discrepency.
    if (currentDepth != -1)
    {
        // The offset is equal to the difference between the current depth and the lowest depth, -1.
        int offset = currentDepth - (-1);
        // The depth is subtracted by this offset.
        depth -= offset;
    }
     
    // Returns the depth of the most nested loop in the code.
    return depth;
}

/*
 This method prints the unique elements within the stack to the console with a space between elements.
*/
void Stack::getUniqueElements() {
    
    // Checks whether the stack is empty. If so, the string "NA" will be print and the function will cease execution.
    if (stack.size() == 0)
    {
        cout << "NA" << endl;
        return;
    }
    
    // Holds the unique elements of the Stack.
    vector<string> uniqueValues;
    
    // Retrieves all elements from the Stack until the Stack is empty.
    while (!stack.empty())
    {
        // Retrieves the topmost value from the Stack.
        string value = pop();
        
        // Tracks whether the value is truly unique. The default is true, and only becomes false if another exact element matching the current string is found.
        bool unique = true;
        // Retrieves the current size of the uniqueValues vector.
        int size = int(uniqueValues.size());
        
        // Loops through the uniqueValues vector to find if there is another element that matches the current value from the Stack.
        for (int j = 0; j < size; ++j)
        {
            // If another element in the uniqueValues vector is found to have the same value as the current element, then the element is not unique, hence unique is now false.
            if (uniqueValues[j] == value)
                unique = false;
        }
        
        // Checks whether the value is truly unique. If so, then the value is added to the uniqueValues vector. There is also a check if the uniqueValues vector is empty because if so, then the element is unique by default as there is nothing in the vector to compare the element to.
        if (unique || size == 0)
            uniqueValues.push_back(value);
    }
    
    int size = int(uniqueValues.size());
    
    // Formats the uniqueValues vector for an accurate output, as elements are retrieved from the Stack in reverse order.
    for (int i = 0; i < (size/2); ++i)
    {
        string temp = uniqueValues[i];
        uniqueValues[i] = uniqueValues[(uniqueValues.size() - 1) - i];
        uniqueValues[(uniqueValues.size() - 1) - i] = temp;
    }
    
    // Prints out the unique values.
    for (int i = 0; i < size; ++i)
        cout << uniqueValues.at(i) << " ";
}

/*
 This method is called by the calculateLoopDepth() method only. This method searches the current keywordsForLoopDepth Stack and finds the valid loop declaration. These valid loop declarations are then placed into a new string vector, a new stack, and this new vector becomes the curent stack.
*/
void Stack::cleanTheLoop(Stack* syntaxErrors) {
    
    // Tracks the previous and next strings in the stack.
    string previousString = "";
    string nextString = "";
    
    // Tracks the current number of valid "FOR" and "END" keywords. This serves to make sure there are not more "END" keywords than "FOR" keywords, which would augment the loop calculation.
    int forNumber = 0;
    int endNumber = 0;
    
    // Holds the valid loop keywords, that is, those that conform to the syntax rules of the language.
    vector<string> newStack;
    
    // Holds the size of the Stack.
    int size = int(stack.size());
    
    // Loops through the entire keywordsForLoopDepth Stack.
    for (int i = 0; i < size; ++i)
    {
        // Checks whether the nextString exists. If so, the string following the current one being checked is assigned to nextString.
        if (i+1 < size)
            nextString = stack[i+1];
        else
            nextString = "";
        
        // These statements figure out the current keyword being checked, whether it is "FOR", "BEGIN", or "END".
        if (stack[i] == "FOR")
        {
            // If the current keyword is "FOR" and there is not a "BEGIN" following it, then the loop it declares is invalid and the loop iterates to the next keyword in the Stack.
            if (nextString != "BEGIN")
                continue;
            // If the condition is not met, then the loop seems valid and the "FOR" keyword is placed onto the new Stack.
            else
            {
                // Places the keyword into the newStack vector.
                newStack.push_back(stack[i]);
                // Increments the forNumber, indicating that a valid "FOR" keyword was placed onto the Stack.
                ++forNumber;
            }
            
            // Assigns "FOR" as the previous string before the next loop iteration.
            previousString = "FOR";
        }
        else if (stack[i] == "BEGIN")
        {
            // If the current keyword is "BEGIN" and it did not follow a "FOR" keyword, or does not precede a "BEGIN" keyword, then this keyword is invalid, and the loop iterates to the next keyword in the Stack.
            if (previousString != "FOR" || nextString == "BEGIN")
                continue;
            // If the condition is not met, then it is valid and the "BEGIN" keyword is placed onto the Stack.
            else
                // Places the keyword into the newStack vector.
                newStack.push_back(stack[i]);
            
             // Assigns "BEGIN" as the previous string before the next loop iteration.
            previousString = "BEGIN";
        }
        else
        {
            // If the current keyword is "END", then it is not valid if it followed a "FOR" keyword, the next string is not "END" or "FOR" (which means it is "BEGIN"), or the number of valid "END" keywords is greater than or equal to the number of valid "FOR" keywords. Thus, if any of these conditions are met, the keyword is invalid, and the loop iterates to the nex keyword in the Stack.
            if (previousString == "FOR" || nextString == "BEGIN" || endNumber >= forNumber)
                continue;
            else
            {
                // Places the keyword into the newStack vector.
                newStack.push_back(stack[i]);
                // Increments the endNumber, indicating that a valid "END" keyword was placed onto the Stack.
                ++endNumber;
            }
            
             // Assigns "END" as the previous string before the next loop iteration.
            previousString = "END";
        }
    }
    
    // Assigns the newStack vector of valid loop keywords to the stack vector which represents the keywordsForLoopDepth Stack instance.
    stack = newStack;
    
    // If there are a greater amount of "FOR" keywords than "END" keywords, then "END" will be pushed onto the Stack of syntaxErrors, indicating that the code needs more "END" keyowrds.
    if (forNumber > endNumber)
        syntaxErrors->push("END");
}

/*
 * Implementation of the LexicalAnalysis class, which handles the input of code to be checked, and the output of various parameters of the code analysis.
*/

// Holds the name of the file containing the code to analyze.
const string fileName;

// Holds the boolean values signifying if a misspelled keyword was found, based on the intended keyword.
bool misspelledFor;
bool misspelledBegin;
bool misspelledEnd;

// Tracks the number of parentheses in the current line of code being parsed by the parseCode() method.
int leftParentheses = 0;
int rightParentheses = 0;

// Reference to the Stack containing the keywords of the analyzed code.
Stack* keywords;
// Reference to the Stack containing the identifiers of the analyzed code.
Stack* identifiers;
// Reference to the Stack containing the constants of the analyzed code.
Stack* constants;
// Reference to the Stack containing the operators of the analyzed code.
Stack* operators;
// Reference to the Stack containing the delimiters of the analyzed code (, and ;).
Stack* delimiters;
// Reference to the Stack containing the syntax errors of the analyzed code.
Stack* syntaxErrors;
// Reference to the Stack containing the parentheses of the analyzed code. This particular stack is not printed to the console, but is necessary for determining syntax errors within the code.
Stack* parentheses;

Stack* keywordsForLoopDepth;

/*
LexicalAnalysis constructor which takes a parameter of type string called fileName, which is the name of the text file containing the code to be analyzed.
fileName is a constant, so it is initialized to the constant instance variable fileName in a special part of the constructor.
In addition to the specification of the fileName, all of the Stacks to be used in the lexical analysis of the code are defined by-reference.
After initializing all of the Stacks, the parseFileInput() method is called, which reads the text, the code, in the text file.
*/
LexicalAnalysis::LexicalAnalysis(string fileName)
:fileName(fileName)
{
    // Initialization of all of the Stacks.
    keywords = new Stack();
    identifiers = new Stack();
    constants = new Stack();
    operators = new Stack();
    delimiters = new Stack();
    syntaxErrors = new Stack();
    parentheses = new Stack();
    keywordsForLoopDepth = new Stack();
    
    // Initializes the value of the misspelled booleans, which is initially false, as there are no misspelled keywords, yet.
    misspelledFor = false;
    misspelledBegin = false;
    misspelledEnd = false;
    
    // Begins the parsing and analysis of code elements and syntax errors in the specified code file.
    parseFileInput();
}
/*
Destructor of the LexicalAnalysis class.
*/
LexicalAnalysis::~LexicalAnalysis() {}

/*
This method opens the text file with the fileName specified in the LexicalAnalysis constructor, and then reads each line of text within the file until the end of the text file is reached, that is, all of the code has been read and analyzed accordingly. For each line of text, the parseCode() method is called, which sorts the various code elements, placing them in the appropriate Stack.
This method will throw an exception if the text file could not not opened, that is the specified file could not be found.
*/
void LexicalAnalysis::parseFileInput() {
    
    // Declares an ifstream instance for opening the text file containing the code to be analyzed.
    ifstream fileStream;
    
    // Opens the specified file.
    fileStream.open(fileName);
    
    // If the open operation failed, then the file does not exist, thus an exception is thrown.
    if (!fileStream.is_open())
        throw invalid_argument("The specified file does not exist.");
    
    // Variable which holds the current line of the file being checked.
    string line;
    
    // Loops through all of the lines of code in the file until the end of the file is reached.
    while (!fileStream.eof()) {
        // Gets the current line of text (code).
        getline(fileStream, line);
        // Passes the current line into the parseCode() method, which further divides it for analysis.
        parseCode(line);
    }
}

/*
This method takes a parameter of type string, and checks whether the specified string contains an uppercase letter.
Returns true if the specified string contains an uppercase letter, false otherwise.
*/
bool LexicalAnalysis::containsUppercaseLetter(string str) {
    // Loops through each character of the entire string.
    for (int i = 0; i < int(str.size()); ++i)
    {
        // If an uppercase letter is found, then the string does contain an uppercase letter, so the method returns true.
        if (isupper(str[i]))
            return true;
    }
    
    // If an uppercase letter is not found, then there is not one in the string, so false is returned.
    return false;
}

/*
 This method takes in two parameters, one of type string representing the string to be checked, and startIndex, which is the index of the string where the search for the end of the identifier will start.
 This method checks a specific line of code found to contain an identifier, a lowercase letter or word, and checks where the identifier text ends in the string. This method helps the parseCode() method to obtain the entire identifier string via information about where the indentifier text ends within the line of code.
 Returns an integer representing the position within the string where the identifier text ends, returns -1 otherwise, that is the entire search portion of the string contains the text of the identifier.
*/
int LexicalAnalysis::findEndOfIdentifier(string str, int startIndex) {
    
    // Loops through the specified string starting at the specified index.
    for (int i = startIndex; i < int(str.size()); ++i)
    {
        // If a character that is not a lowercase letter is found, the end of the identifier is found and its index is thus returned.
        if (!islower(str[i]))
            return i;
    }
    
    // If the identifier spans the entire portion of the checked string, -1 is returned to signify that the entire portion of the checked string is part of the identifier.
    return -1;
}

/*
This method takes in two parameters, one of type string representing the string to be checked, and startIndex, which is the index of the string where the search for the end of the constant will start.
This method checks a specific line of code found to contain a constant, a number, and checks where the numerals of the constant ends in the string. This method helps the parseCode() method to obtain the entire constant string via information about where the constant numerals ends within the line of code.
 Returns an integer representing the position within the string where the constant (numerical) text ends, returns -1 otherwise, that is the entire search portion of the string contains the numerals of the constant.
*/
int LexicalAnalysis::findEndOfConstant(string str, int startIndex) {
    
    // Loops through the specified string starting at the specified index.
    for (int i = startIndex; i < int(str.size()); ++i)
    {
        // If a character that is not a digit is found, the end of the constant is found and its index is thus returned.
        if (!isdigit(str[i]))
            return i;
    }
    
    // If the constant spans the entire portion of the checked string, -1 is returned to signify that the entire portion of the checked string is part of the constant.
    return -1;
}

/*
This method takes in a parameter of type string, which is the line of code that is to be analyzed.
This method separates the parts of a line based on the spaces that exist within the line of code, and places those parts into a vector. Then, each element of the vector, the parts of the line, is traversed and passed to the organizeElements() method, which traverse each character of those parts to determine the code element(s) it contains, and organizes them by placing them into the appropriate Stack. This method also keeps track of the preceding string to be passed to the organizeElements() method for syntax checking purposes.
*/
void LexicalAnalysis::parseCode(string str) {
    
    // Vector which holds the various pieces of code separated by a space.
    vector<string> partsOfLine;
    
    // This loop executes at least once and continues until a space character is not found.
    while (str.find(" ") != string::npos)
    {
        // Finds the end of the part of the line, which is where a space would be located.
        int end = int(str.find(" "));
        
        // Extracts the part of the line before the next space.
        string part = str.substr(0, end);
        
        // The rest of the line then becomes the remaining string to analyze.
        str = str.substr(end+1);
        
        // The part of the line found is placed into the partsOfLine vector.
        partsOfLine.push_back(part);
    }
    
    // When the loop terminates and there is more than one part of the line, the remaining string without a space after will be remaining, so it must be appended to the partsOfLine vector.
    partsOfLine.push_back(str);
    
    // Tracks the preceding string that was already checked, which is initially empty.
    string precedingString = "";
    
    // Loops through each of the parts of the line, then passes the current part and the preceding string to the organizeElements() method for organization into the various Stacks of code elements.
    for (int i = 0; i < int(partsOfLine.size()); ++i)
    {
        // Retrieves the element.
        string element = partsOfLine.at(i);
        // Passes the element and preceding string to the organizeCode() method.
        organizeElements(element, precedingString);
        // After organization, the current element becomes the preceding element right before the loop increments.
        precedingString = element;
    }
    
    //Checks the balances of parentheses in the current line of code before the function ceases execution.
    checkParentheses();
}

/*
This method checks whether there is an imbalance of parentheses in the current line of code. This method is called after each complete line of code is parsed and organized appropriately.
*/
void LexicalAnalysis::checkParentheses() {
    // Positive excess will mean abundance of left parentheses and negative excess will mean an abundance of right parentheses. A zero value will indicate no syntax errors regarding the use of parentheses.
    int excess = leftParentheses - rightParentheses;
    
    // Checks whether the excess of parentheses is positive or negative, pushing the corresponding parenthesis in excess onto the syntax errors Stack. If there is no excess, or no parentheses, these conditions will not be satisfied.
    if (excess > 0)
        syntaxErrors->push("(");
    if (excess < 0)
        syntaxErrors->push(")");
    
    if (excess != 0)
        keywordsForLoopDepth->pop();
    
    // Resets the values of the number of parentheses before proceeding to the next line of code.
    leftParentheses = 0;
    rightParentheses = 0;
}

/*
This method takes two parameters of type string, the string to be analyzed for code elements, and the string preceding the string to be analyzed.
This method first checks whether the string contains an uppercase letter, which would indicate a possible keyword, eleiminating any necessity for traversing the string, since the string can only be a keyword. If the string is not a keyword, then each character of the string is traversed. For each iteration, the character is compared to various logical statements to determine what code element it is or is a part of.
 The result of this method is all code elements within the string being placed into the appropriate Stack.
*/
void LexicalAnalysis::organizeElements(string str, string precedingString) {
    // Checks whether the string contains an uppercase letter. If so, then it is a keyword. If the keyword is spelled correctly, then it is placed on the keywords Stack. If not spelled correctly, then the "keyword" is placed on the syntax error Stack.
    if (containsUppercaseLetter(str))
    {
        if (str == "FOR" || str == "BEGIN" || str == "END")
        {
            keywords->push(str);
            keywordsForLoopDepth->push(str);
        }
        else
        {
            syntaxErrors->push(str);
            
            // Attempts to figure out what the misspelled keyword was meant to be. This logic is not perfect, but will classify simple misspellings and push a suggested fix to the syntaxErrors Stack.
            if (str[0] == 'F')
            {
                misspelledFor = true;
                syntaxErrors->push("(did you mean FOR)");
            }
            else if (str[0] == 'B')
            {
                misspelledBegin = true;
                syntaxErrors->push("(did you mean BEGIN)");
            }
            else if (str[0] == 'E')
            {
                misspelledEnd = true;
                syntaxErrors->push("(did you mean END)");
            }
            else
            {
                if (str.length() == 5)
                {
                    misspelledBegin =  true;
                    syntaxErrors->push("(did you mean BEGIN)");
                }
                else if (str[2] == 'R')
                {
                    misspelledFor = true;
                    syntaxErrors->push("(did you mean FOR)");
                }
                else
                {
                    misspelledEnd = true;
                    syntaxErrors->push("(did you mean END)");
                }
            }
        }
    }
    else
    {
        // Loops through each character in the string.
        for (int i = 0; i < int(str.length()); ++i)
        {
            // Stores the current character being checked.
            char c = str[i];
            
            // Checks whether the current character is a semicolon or a comma (located within parentheses). If the character is either of these, then the character is placed on the delimiters Stack, else it is placed on the syntaxErrors Stack.
            if (c == ',' || c == ';')
            {
                if (c == ';' && i == int(str.size() - 1))
                    delimiters->push(string(1,c));
                else if (c == ',' && (str.find('(') != string::npos || str.find(')' != string::npos)))
                    delimiters->push(string(1, c));
                else
                    syntaxErrors->push(string(1, c));
            }
            // Checks whether the current character is a plus, minus, multiplication, division, modulus, or equals sign.
            else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=')
            {
                // If the character is a plus or minus sign, then this condition checks if the loop is not at the end of the string, then the next character will be checked to see if it matches the current character. If so, then there is either an increment or decrement operator, and it will added to the operators Stack accordingly.
                if (i != int(str.length() - 1) && (c == '+' || c == '-'))
                {
                    // If the next string is a plus sign, then the operator is an increment operator.
                    if (str[i+1] == '+')
                    {
                        operators->push("++");
                        i += 1;
                        continue;
                    }
                    // If the next string is a minus sign, then the operator is a decrement operator.
                    if (str[i+1] == '-')
                    {
                        operators->push("--");
                        i += 1;
                        continue;
                    }
                    // None of the above conditions are satisfied, then there is only a regular addition or substraction sign and it is subsequently pushed onto the operators Stack.
                    if (str[i+1] != '+' || str[i+1] != '-')
                    {
                        operators->push(string(1,c));
                    }
                }
                // If the operator is not a plus or minus sign, but some other one character operator, then the current character is added to the operators Stack.
                else
                {
                    operators->push(string(1, c));
                }
            }
            // Checks whether the current character is a lowercase letter. If so, then the character is part of an identifier.
            else if (islower(c))
            {
                // Finds the index where the identifier ends, that is, the first index where there is not a lowercase letter.
                int end = findEndOfIdentifier(str, i);
                // Will hold the identifier string.
                string identifier;
                
                // Checks whether the end of the identifier does not exist. If it doesn't then the rest of the string is the identifier.
                if (end == -1)
                {
                    // The entire string is the identifier.
                    identifier = str.substr(i);
                    // The loop variable is set to an out of bounds location to signal the end of the loop since the entire string is the identifier.
                    i = int(str.length());
                }
                else
                {
                    // Finds the size of the identifier string based on the difference between the end index and the current index.
                    int size = end - i;
                    // Extracts the identifier string based on the current index and the calculated size of the identifier string.
                    identifier = str.substr(i, size);
                    // Increments the loop variable so that the characters of the identifier are not accessed again by the loop.
                    i += (size-1);
                }
                
                // Pushes the identifier string on the identifier Stack.
                identifiers->push(identifier);
                
            }
            // Checks whether the current character is a numerical digit. If so, then a constant has been found.
            else if (isdigit(c))
            {
                // Finds the index where the constant ends, that is, the first index where there is not a numerical digit.
                int end = findEndOfConstant(str, i);
                // Calculates the size of the constant string.
                int size = end - i;
                // Extracts the constant string based on the current index and the calculated size of the constant string.
                string constant = str.substr(i, size);
                // Pushes the constant string onto the constants Stack/
                constants->push(constant);
                // Increments the loop variable by the size of the constant minus 1 so that the loop does not check the other digits of the constant.
                i += (size-1);
            }
            // Checks whether the current character is a parenthesis.
            else if (c == '(' || c == ')')
            {
                // If a left parenthesis is not after a "FOR" keyword, then the left parenthesis is not in the correct place, thus it is a syntax error. Else, the parenthesis is pushed onto the parentheses Stack.
                if (c == '(' && precedingString != "FOR")
                    syntaxErrors->push(string(1,c));
                else
                {
                    // Increments the corresponding counter variable based on the type of parenthesis.
                    if (c == '(')
                        ++leftParentheses;
                    else
                        ++rightParentheses;
                    
                    parentheses->push(string(1,c));
                }
            }
            // This else statement is never executed. It should only execute if something has gone terribly wrong and only exists to complete the conditional statement, as all situations should have been covered before this statement.
            else
            {
                cout << "This condition should never be executed. If so, something is wrong!" << endl;
            }
        }
    }
}

/*
This public method returns the analysis of the code in the specified text file. When this method is called, the depth of the nested loops is computed and printed to the console, and the getUniqueElements() method is called upon each Stack, except the parentheses Stack, which print the unique elements of each Stack to the console.
*/
void LexicalAnalysis::getCodeAnalysis() {
    cout << "OUTPUT> The depth of the nested loop(s) is " << keywordsForLoopDepth->depthOfLoop(syntaxErrors, misspelledFor, misspelledBegin, misspelledEnd) << endl << endl;
    cout << "Keywords: ";
    keywords->getUniqueElements();
    cout << endl;
    cout << "Identifiers: ";
    identifiers->getUniqueElements();
    cout << endl;
    cout << "Constants: ";
    constants->getUniqueElements();
    cout << endl;
    cout << "Operators: ";
    operators->getUniqueElements();
    cout << endl;
    cout << "Delimiters: " ;
    delimiters->getUniqueElements();
    cout << endl << endl;
    
    cout << "Syntax Error(s): ";
    syntaxErrors->getUniqueElements();
    cout << endl;
}
