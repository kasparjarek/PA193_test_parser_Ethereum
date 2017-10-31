# PA193_test_parser_Ethereum

[![Build Status](https://travis-ci.org/kasparjarek/PA193_test_parser_Ethereum.svg?branch=master)](https://travis-ci.org/kasparjarek/PA193_test_parser_Ethereum)

This is university project.

```
mkdir build
cd build
cmake ..
make
```

# Useful links
[Ethereum yellow paper](http://yellowpaper.io)

Merkle Patricia Trie
* [Wiki](https://github.com/ethereum/wiki/wiki/Patricia-Tree)
* [Blogpost - Understanding the ethereum trie](https://easythereentropy.wordpress.com/2014/06/04/understanding-the-ethereum-trie/)

# TODO

4.3.4 Block Header Validity
* parent block hash
* block number
* difficulty
* gas limit
* timestamp
* (nonce a mixHash asi vynechame, je k nim potrebna cela databaza)

4.3.2 Holistic Validity
* merkle tree - do konce týdne commitnu (Jarek)
* merkle tree pouzit na validaciu zoznamu transakcii (H_t)
* (zoznam receiptov asi vynechame, potrebna databaza stavov)
* (logsBloom asi vynechame, potrebny EVM)



# Structure of block 1000000:

```
f9 | 02fd |			Node, size: 765
  f9 | 0217 |			Node, size: 535
    a0 | b4fbadf8...		Leaf, size: 32 (parentHash)
    a0 | 1dcc4de8...		Leaf, size: 32 (ommersHash)
    94 | 2a65aca4...		Leaf, size: 20 (beneficiary)
    a0 | 0e066f3c...		Leaf, size: 32 (stateRoot)
    a0 | 65ba887f...		Leaf, size: 32 (transactionsRoot)
    a0 | 20e35345...		Leaf, size: 32 (receiptsRoot)
    b9 | 0100 | 00000000...	Leaf, size: 256 (logsBloom)
    86 | 0b69de81a22b		Leaf, size: 6 (difficulty)
    83 | 0f4240			Leaf, size: 3 (number)
    83 | 2fefd8			Leaf, size: 3 (gasLimit)
    82 | c444			Leaf, size: 2 (gasUsed)
    84 | 56bfb415		Leaf, size: 4 (timestamp)
    98 | d7830103...		Leaf, size: 24 (extraData)
    a0 | 92c41292...		Leaf, size: 32 (mixHash)
    88 | cd4c55b941cf9015	Leaf, size: 8 (nonce)
  f8 | e0 |			Node, size: 224
    f8 | 6e |			Node, size: 110
      15			Leaf, size: 1 (T1, nonce)
      85 | 12bfb19e60		Leaf, size: 5 (T1, gasPrice)
      83 | 01f8dc		Leaf, size: 3 (T1, gasLimit)
      94 | c083e994...		Leaf, size: 20 (T1, to)
      89 | 056bc75e...		Leaf, size: 9 (T1, value)
      80 |			Leaf, size: 0 (T1, data)
      1c			Leaf, size: 1 (T1, v)
      a0 | a254fe08...		Leaf, size: 32 (T1, r)
      a0 | 30b7835b...		Leaf, size: 32 (T1, s)
    f8 | 6e |			Node, size: 110
      82 | 43eb			Leaf, size: 2 (T2, nonce)
      85 | 0df8475800		Leaf, size: 5 (T2, gasPrice)
      82 | c350			Leaf, size: 2 (T2, gasLimit)
      94 | df190dc7...		Leaf, size: 20 (T2, to)
      88 | 06113a84987be800	Leaf, size: 8 (T2, value)
      80 |			Leaf, size: 0 (T2, data)
      1c			Leaf, size: 1 (T2, v)
      a0 | 3b08715b...		Leaf, size: 32 (T2, r)
      a0 | 10b9afb6...		Leaf, size: 32 (T2, s)
  c0 |				Node: size: 0 (empty ommers list)
```
