//
//  pa3.h
//  Lexical Analysis
//
//  Created by Nicholas Fata on 3/11/17.
//

#ifndef pa3_h
#define pa3_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <exception>

using namespace std;

class Stack {
private:
    // Stores the data currently on the stack.
    vector<string> stack;
    // Checks whether the stack contains a certain element.
    bool contains(string value);
    // Loops through all elements within the Stack and only parses out syntactically valid loop keywords.
    void cleanTheLoop(Stack* syntaxErrors);
public:
    // Stack constructor and destructor.
    Stack();
    ~Stack();
    
    // Pushes the specific string element onto the stack.
    void push(string value);
    // Returns the topmost element from the stack.
    string pop();
    // Returns the maximum depth of loops in the code. This method is only called on the stack containing the keywords.
    int depthOfLoop(Stack* syntaxErrors, bool misspelledFor, bool misspelledBegin, bool misspelledEnd);
    // Prints all of the unique elements from the stack to the console.
    void getUniqueElements();
};

class LexicalAnalysis {
private:
    // Stores the name of the code file.
    const string fileName;
    
    // Indicate whether a keyword was misspelled and the possible keyword that was intended.
    bool misspelledFor;
    bool misspelledBegin;
    bool misspelledEnd;
    
    // The Stacks for each of the various code elements to analyze, including: keywords, identifiers, constants, operators, delimiters, parenetheses, and syntax errors.
    Stack* keywords;
    Stack* identifiers;
    Stack* constants;
    Stack* operators;
    Stack* delimiters;
    Stack* parentheses;
    Stack* syntaxErrors;
    Stack* keywordsForLoopDepth;
    
    // Tracks the number of parentheses in the current line of code being parsed.
    int leftParentheses;
    int rightParentheses;
    
    // Takes a line of code and separates it into smaller parts based on the presence of spaces.
    void parseCode(string str);
    // Opens the specified code file and reads each individual line. This method will throw an exception if the file does not exist.
    void parseFileInput();
    // Separates each distinct character or string of characters and places it onto the corresponding code element Stack.
    void organizeElements(string str, string precedingString);
    // Checks whether a specified string contains an uppercase letter.
    bool containsUppercaseLetter(string str);
    // Searches the specified string from the specified index for the end of the identifier, that is, a lowercase letter is no longer found, and returns the corresponding index.
    int findEndOfIdentifier(string str, int startIndex);
    // Searches the specified string from the specified index for the end of the constant, that is, a digit is no longer found, and returns the corresponding index.
    int findEndOfConstant(string str, int startIndex);
    // Checks the current code line for an imbalance of parentheses. If so, the appropriate parenthesis is appended to the syntax errors Stack.
    void checkParentheses();
public:
    // LexicalAnalysis constructor and destructor. Only takes the file to open as a parameter.
    LexicalAnalysis(string fileName);
    ~LexicalAnalysis();
    
    // Returns the analysis of the specified code, including loop depth, code elements, and syntax errors.
    void getCodeAnalysis();
};

#endif /* pa3_h */
