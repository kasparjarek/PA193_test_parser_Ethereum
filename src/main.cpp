#include "parser.h"

#include <iostream>
#include <vector>
#include"validator.h"
#include<stdexcept>

using namespace std;

int main(int argc, char **argv)
{
	    if (argc != 3) {
            cout << "The program expects exactly 2 arguments." << endl << "Both arguments have to be valid file paths to Ethereum binary blocks." << endl;
            return 1;
    }
    EthereumParser parser;
    try {
            parser.parseFile(argv[1]);
    }
    catch (runtime_error &e) {
            cout << "The file with parent block could not be read." << endl;
            return 2;
    }
    Block parentblock = parser.blocks()[0];
    try {
            parser.parseFile(argv[2]);
    }
    catch (runtime_error &e) {
            cout << "The file with child block could not be read." << endl;
            return 3;
    }
    Block childblock = parser.blocks()[0];
    validateAll(parentblock, childblock);
    return 0;
}


