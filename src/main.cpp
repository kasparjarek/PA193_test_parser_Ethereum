#include "parser.h"

#include <iostream>
#include <vector>
#include"validator.h"

using namespace std;

int main(int argc, char **argv)
{
/*	vector<Block> v = EthereumParser{"../test/samples/1000000"}.parseFile();

	Block b = v.front();

	for (auto x : b.header().parentHash())
		cout << hex << (unsigned int) x << " ";
	cout << endl;
	for (auto x : b.header().ommersHash())
		cout << hex << (unsigned int) x << " ";
	cout << endl;

	for (auto x : b.header().receiptsRoot())
		cout << hex << (unsigned int) x << " ";
	cout << endl;

	cout << dec << b.header().difficulty() << endl;

	for (unsigned i = 0; i < b.transactions().size(); ++i) {
		for (auto x : b.transactions()[i].to())
			cout << hex << (unsigned) x << " ";
		cout << endl;
	}

	cout << dec << b.ommers().size() << endl;
	return 0;
	*/
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
    EthereumParser parser;
    parser.parseFile(argv[1]);
    Block parentblock = parser.blocks()[0];
    parser.parseFile(argv[2]);
    Block childblock = parser.blocks()[0];
    validateAll(parentblock, childblock);

    return 0;
}


