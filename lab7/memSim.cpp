#include <iostream>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fstream>

using namespace std;

class CacheBlock {
private:
		int cmBlockNumber, cmSetNumber, validBit, dirtyBit, tag, mmBlockNumber, hit, data;	// Cache block elements
		int age;	// Replacement policy utilities
		char rPolicy;
	public:
		CacheBlock();
		void initBlock(int,int,char);
		int getTag();
		char getRPolicy();
		int getAge();

		void setAge(int);
		int checkValid();
		void setValid();
		void setTag(int);
		void setData(int);
		void setDirtyBit(int);
};

CacheBlock::CacheBlock() {
	cmBlockNumber = 0;
	cmSetNumber = 0;
	validBit = 0;
	dirtyBit = 0;
	tag = 0;
	mmBlockNumber = 0;
	hit = 0;
}

void CacheBlock::initBlock(int blockNum, int setNum, char rPolicy) {
	this->cmBlockNumber = blockNum;
	this->cmSetNumber = setNum;
	this->rPolicy = rPolicy;

/*
	cout << endl;
	cout << "Block Number: " << cmBlockNumber << endl;
	cout << "Set Number : " << cmSetNumber << endl;
	*/
}

int CacheBlock::getTag() {
	return this->tag;
}

int CacheBlock::getAge() {
	return this->age;
}

void CacheBlock::setAge(int age) {
	this->age = age;
}

void CacheBlock::setDirtyBit(int dirtyBit) {
	this->dirtyBit = dirtyBit;
}

char CacheBlock::getRPolicy() {
	return this->rPolicy;
}

int CacheBlock::checkValid() {
	return this->validBit;
}

void CacheBlock::setValid() {
	this->validBit = 1;
}
void CacheBlock::setTag(int tag) {
	this->tag = tag;
}
void CacheBlock::setData(int mmBlockNumber) {
	this->data = mmBlockNumber;
}

////////////////////////////////////////////


void printChart() {
	
}

int calculateLines(int mmSize) {
	return log2(mmSize);
}

int calculateOffset(int cbSize) {
	return log2(cbSize);
}

int calculateIndex(int cmSize, int cbSize, int mapping) {
	return (log2((cmSize / cbSize) / mapping));
}

int calculateTag(int mmSize, int cbSize, int cmSize, int mapping) {
	return calculateLines(mmSize) - calculateOffset(cbSize) - calculateIndex(cmSize, cbSize, mapping);
}

int calculateCmSize(int tag, int cmSize, int cbSize) {
	int overhead = (cmSize / cbSize) * (tag + 1 + 1);
	overhead = overhead / 8;
	return cmSize + overhead;
}

void displayCalculations(int lines, int offset, int index, int tag, int totalCmSize) {
	cout << endl;
	cout << "Simulator Output: " << endl;
	cout << "Total address lines required = " << lines << endl;
	cout << "Number of bits for offset = " << offset << endl;
	cout << "Number of bits for index = " << index << endl;
	cout << "Number of bits for tag = " << tag << endl;
	cout << "Total cache size required = " << totalCmSize << endl;
}

int getTagBits(int mmAddress, int lines, int tag) {
	int tagBits = mmAddress >> (lines - tag);
	return tagBits;
}

int getOldestBlock(CacheBlock cm[], int setStart, int setEnd) {
	int i = 0, index = 0, smallestAge = 0;
	smallestAge = cm[setStart].getAge();
	for (i = setStart; i < setEnd; i++) {
		if (smallestAge > cm[i].getAge()) {
			smallestAge = cm[i].getAge();
			index = i;
		}
	}
	return index;
}

void replaceBlock(CacheBlock cm[], int mmBlockNumber, int tag, char operation, int index) {
	cm[index].setData(mmBlockNumber);
	cm[index].setTag(tag);
	cm[index].setValid();
	if (operation == 'R')
		cm[index].setDirtyBit(0);
	else if (operation == 'W')
		cm[index].setDirtyBit(1);
}

void performOperation(CacheBlock cm[], int mmAddress, int cbSize, int cmSetCount, int mapping, int lines, int tag, int operation, int opCount) {
	int mmBlockNumber = mmAddress / cbSize;				// Corresponding main memory block
	int cmSetNumber = mmBlockNumber % cmSetCount;	// Figure out which set we want
	int setStart = cmSetNumber * mapping;	// Get the first block in the set

	int tagValue = getTagBits(mmAddress, lines, tag);		// Get the tag bits

	char rPolicy = cm[0].getRPolicy();

	/*
	cout << endl;
	cout << "Main Memory Block Number : " << mmBlockNumber << endl;
	cout << "Set Number : " << cmSetNumber << endl;
	cout  << "First Index In Set : " << setStart << endl;
	cout << "Tag Value : " << tagValue << endl;
	*/

	int i = 0, currTag = 0, valid = 0;
	bool hit = false;

	for (i = setStart; i < setStart + mapping; i++) {
		valid = cm[i].checkValid();

		if (valid == 1) {						// Bit is valid
			currTag = cm[i].getTag();
			if (tagValue == currTag) {	// We found the tag that we're looking for
				hit = true;
				if (operation == 'R') {
					if (rPolicy == 'L') {	// For LRU we have to update the age since we used the block
						cm[i].setAge(opCount);
					}
				}
				else if (operation == 'W') {
					if (rPolicy == 'L') {	// For LRU we update the age and set the dirty bit
						cm[i].setAge(opCount);
						cm[i].setDirtyBit(1);
					}
					else if (rPolicy == 'F') {	// For FIFO we set the dirty bit
						cm[i].setDirtyBit(1);
					}
				}
			}
			// Print Chart
			return;
		}
		else if (valid == 0) {
			hit = false;
			replaceBlock(cm, mmBlockNumber, tag, operation, i);
			// Print Chart
			return;
		}
	}
	// Set is full
	int index = getOldestBlock(cm, setStart, setStart + mapping);
	hit = false;
	replaceBlock(cm, mmBlockNumber, tag, operation, index);
	// Print Chart
	return;
}


int main() {
	int mmSize = 0, cmSize = 0, cbSize = 0, mapping = 0;
	int lines = 0, offset = 0, index = 0, tag = 0, totalCmSize = 0;
	char rPolicy;
	string inputFile;

	cout << "Enter the size of main memory in bytes: ";
	cin >> mmSize;
	cout << "Enter the size of the cache in bytes: ";
	cin >> cmSize;
	cout << "Enter the cache block/line size: ";
	cin >> cbSize;
	cout << "Enter the degree of set-associativity (input an n-way set-associative mapping): ";
	cin >> mapping;
	cout << "Enter the replacement policy (L = LRU, F = FIFO): ";
	cin >> rPolicy;
	cout << "Enter the name of the input file containing the list of memory references generated by the CPU: ";
	cin >> inputFile;

	rPolicy = toupper(rPolicy);

	lines = calculateLines(mmSize);
	offset = calculateOffset(cbSize);
	index = calculateIndex(cmSize, cbSize, mapping);
	tag = calculateTag(mmSize, cbSize, cmSize, mapping);
	totalCmSize = calculateCmSize(tag, cmSize, cbSize);

	displayCalculations(lines, offset, index, tag, totalCmSize);

	int cmBlockCount = cmSize / cbSize;
	int cmSetCount = cmBlockCount / mapping;

	CacheBlock cm[cmBlockCount];

	// Initialize the sets
	int i = 0, setNum = 0, blockNum = 0;
	for (setNum = 0; setNum < cmSetCount; setNum++) {		// Iterate the sets
		for (i = 0; i < mapping; i++) {										// Iterate the blocks in each set
			cm[blockNum].initBlock(blockNum, setNum, rPolicy);
			blockNum += 1;
		}
	}

	//
	int refCount = 0;
	int opCount = 0;
	char operation = 'R';
	int mmAddress = 0;
	//

	string line;
	ifstream file(inputFile);
	cout << endl;

	file >> refCount;

	for (int i = 0; i < refCount; i++) {
		file >> operation;
		operation = toupper(operation);
		file >> mmAddress;
		performOperation(cm, mmAddress, cbSize, cmSetCount, mapping, lines, tag, operation, i);

		//cout << "Operation: " << operation << endl;
		//cout << "MM Address: " << mmAddress << endl;
	}


	// performOperation(cm, mmAddress, cbSize, cmSetCount, mapping, lines, tag, operation, opCount);
}
