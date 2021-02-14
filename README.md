Pulls input from a text file of binary values, grabs the first 4 length combination (ignoring what remains in the text file). A new text file is outputted which has the binary swapped via a simple form of encryption (hash-mapped key-value swaps).

## How does it work?
There are documenting comments in the `BinaryReplacement.cpp` file.

## How to use it?
1. Or, if you use VSCode, open this code in the app and use the "Run" command (tested on OSX and Ubuntu)

2. If not, you can do it manually. Run the `make` command in your favorite terminal:
```
make
```
2. After installing, you use the `BinaryReplacement` command as shown below:
```
./BinaryReplacement {{test.txt}}
```
