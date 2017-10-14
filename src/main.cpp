#include "block.h"
#include "parser.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<Block> v = EthereumParser{"../test/samples/1000000"}.parseFile();

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
}
