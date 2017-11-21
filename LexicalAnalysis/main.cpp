//
//  main.cpp
//  Lexical Analysis
//
//  Created by Nicholas Fata on 3/11/17.
//  Copyright © 2017 Nicholas Fata. All rights reserved.
//

#include <iostream>
#include <exception>

#include "pa3.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    // The variable that is to hold the name of the text file containing the code the that user would like to analyze.
    string fileName;
    
    // Asks the user to input the name of the text file containing the code.
    cout << "INPUT>> Please enter the name of the input file:" << endl;
    cin >> fileName;
    
    
    // An exception could be thrown if the specified file does not exist, thus the code is placed into a try-catch block.
    try {
        // Constructs a new LexicalAnalysis reference with the specified name of the file to be analyzed. With a call to the constructor, all of the file parsing, code analysis, and code element sorting occurs. After the call is finished, all code elements and syntax errors will be found within the text file.
        LexicalAnalysis* analyzer = new LexicalAnalysis(fileName);
        // Returns the analysis of the code, which details all of the keywords, identifiers, constants, operators, delimiters, and syntax errors within the analyzed code.
        analyzer->getCodeAnalysis();
    } catch (invalid_argument e) {
        // Prints out the specifications of the exception thrown (file not found).
        cerr << e.what() << endl;
    }
    
    // Indicates successful program execution.
    return 0;
}
