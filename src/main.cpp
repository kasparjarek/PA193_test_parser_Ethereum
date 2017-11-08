#include "parser.h"

#include <iostream>
#include <vector>
#include"validator.h"
#include<stdexcept>

using namespace std;

int main(int argc, char **argv)
{
	/*vector<uint8_t> d1{4,5,6,23,56,4,5,4,3,6,7,23,6,9};
	vector<uint8_t> d2{4,5,23,6,9};
	vector<uint8_t> d3{};
	vector<uint8_t> d4{4,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9,5,6,23,56,4,5,4,3,6,7,23,6,9};
	vector<uint8_t> d5{128};
	vector<uint8_t> d6{127};

	cout << "a" << endl;
	//vector<uint8_t> serialized = RLP::serialize(vector<vector<uint8_t> > {d1, d2, d3, d4, d5, d6});
	cout << "b" << endl;
	//for (auto x : serialized) {
	//	printf("%d ", x);
	//}
	printf("\n");

	//RLP r{serialized};
	//printf("num_items: %d\n", r.numItems());
	//for (unsigned i = 0; i < r.numItems(); ++i) {
		//for (unsigned j = 0; j < r[i].dataLength(); ++j) {
			//printf("%d ", serialized[r[i].dataOffset() + j]);
		//}
		//printf("\n");
	//}
	 * */
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

    vector<uint8_t> b = parentblock.toRLP();
    for (auto x : b)
	    cout << hex << int(x) << " ";
    cout <<endl;

    return 0;
}


