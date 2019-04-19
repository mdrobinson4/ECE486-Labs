/*
	Author: Mark D.R. Robinson
	CWID: 11635959
	Lab: #7 - Memory Simulator
	Date: April 19, 2019
*/



#include <iostream>
#include <iomanip>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <bitset>
#include <algorithm>

using namespace std;

class CacheBlock {
private:
		int cmBlockNumber, cmSetNumber, validBit, dirtyBit, tag, mmBlockNumber, hit, data, cmBlockCount;	// Cache block elements
		int age;	// Replacement policy utilities
		char rPolicy;
	public:
		CacheBlock();
		void initBlock(int,int,char);
		int getTag();
		char getRPolicy();
		int getAge();
		int getDirtyBit();
		int getData();

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
}

int CacheBlock::getTag() {
	return this->tag;
}

int CacheBlock::getData() {
	return this->data;
}

int CacheBlock::getDirtyBit() {
	return this->dirtyBit;
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

void printLabel() {
	cout << endl;
	cout << "main memory address";
	cout << "        ";
	cout << "mm blk #";
	cout << "        ";
	cout << "cm set #";
	cout << "        ";
	cout << "cm blk #";
	cout << "        ";
	cout << "hit/miss";
	cout << endl;
	cout << "‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾";

	cout << endl;
}

void printField(int field, int consoleWidth) {
	int spaces = (field > 0 ? (int) log10 ((double) field) + 1 : 1);
	spaces = spaces / 2;
	cout << setw(22) << left << field;
	if ((spaces % 2) == 0)
		cout << " ";
}


void printChart(int mmAddress, int mmBlockNumber, int cmSetNumber, int cmBlockNumber, bool hit, int setStart, int setEnd, int mapping) {
	int spaces = 0, consoleWidth = 80;
	consoleWidth = consoleWidth / 2;

	cout<<setw(8)<<left << "";
	cout << setw(21) << left << mmAddress;
	cout << setw(17) << left << mmBlockNumber;
	cout << setw(14) << left << cmSetNumber;

	int i = 0;
	string str = "";

	if (mapping == 1)
		str = to_string(cmSetNumber);
	else
		str = to_string(setStart) + "-" + to_string(setStart + mapping - 1);

	cout << setw(17) << left << str;

	if (hit == true) cout << "hit";
	else cout << "miss";
	cout << endl;
}

unsigned int countBits(unsigned int n) {
	unsigned int count = 0;
	while (n) {
		count += n & 1;
	  n >>= 1;
	}
	return count;
}

void displayCache(CacheBlock cm[], int cmBlockCount, int tagSize, int mmBlockCount) {
	string str;
	cout << "Cache blk #     ";
	cout << "dirty bit       ";
	cout << "valid bit      ";
	cout << "tag                     ";
	cout << "Data" << endl;
	cout << "‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾" << endl;

	for (int i = 0; i < cmBlockCount; i++) {
		cout << setw(5) << "";
		cout << setw(15) << i;

		if (cm[i].checkValid() == 0)
			cout << setw(16) << "x";
		else
			cout << setw(16) << cm[i].getDirtyBit();

		cout << cm[i].checkValid();

		int tag = cm[i].getTag();

		int spaces = countBits(tag);

		str = "";
		for (int j = 0; j < tagSize; j++) {
			if (cm[i].checkValid() == 1) {
				str += to_string(tag%2);
				tag /= 2;
			}
			else
				str += 'x';
		}
		reverse(str.begin(), str.end());

		cout << setw(((15 - tagSize) / 2) + 5) << "";
		cout << str;
		cout << setw(((15 - tagSize) / 2) + 5) << "";

		str = "";
		if (cm[i].checkValid() == 1) {
			str += "mm blk # ";
			str += to_string(cm[i].getData());
		}
		else {
			for (int j = 0; j < log2(mmBlockCount); j++)
					str += "x";
			str = "xxx";
		}
		cout << setw(((15 - str.length()) / 2) + 5) << "";
		cout << str;
		cout << setw(((15 - str.length()) / 2) + 5) << "";
		cout << endl;
	}
}


int maxHitRate(int refs[], int refCount) {
	int dupCount = 0;
	int countHelp[refCount] = {0};
	for (int i = 0; i < refCount - 1; i++) {
		for (int j = i + 1; j < refCount; j++) {
			if (refs[i] == refs[j] && countHelp[j] == 0) {
				countHelp[j] = 1;
				dupCount += 1;
			}
		}
	}
	return dupCount;
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
	int i = 0, index = setStart, smallestAge = 0;
	smallestAge = cm[setStart].getAge();
	for (i = setStart; i < setEnd; i++) {
		if (smallestAge > cm[i].getAge()) {
			smallestAge = cm[i].getAge();
			index = i;
		}
	}
	return index;
}

void replaceBlock(CacheBlock cm[], int mmBlockNumber, int tag, char operation, int index, int opCount) {
	cm[index].setData(mmBlockNumber);
	cm[index].setTag(tag);
	cm[index].setValid();
	cm[index].setAge(opCount);
	if (operation == 'R')
		cm[index].setDirtyBit(0);
	else if (operation == 'W')
		cm[index].setDirtyBit(1);
}

bool performOperation(CacheBlock cm[], int mmAddress, int cbSize, int cmSetCount, int mapping, int lines, int tag, int operation, int opCount) {
	int mmBlockNumber = mmAddress / cbSize;				// Corresponding main memory block
	int cmSetNumber = mmBlockNumber % cmSetCount;	// Figure out which set we want
	int setStart = cmSetNumber * mapping;	// Get the first block in the set

	int tagValue = getTagBits(mmAddress, lines, tag);		// Get the tag bits

	char rPolicy = cm[0].getRPolicy();

	int i = 0, currTag = 0, valid = 0;
	bool hit = false;

	for (i = setStart; i < setStart + mapping; i++) {
		valid = cm[i].checkValid();

		if (valid == 1) {						// Bit is valid
			currTag = cm[i].getTag();
			//cout << endl << "CURR TAG : " << currTag << endl;
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
				printChart(mmAddress, mmBlockNumber, cmSetNumber, i, hit, setStart, setStart + mapping, mapping);
				return hit;
			}
		}
		else if (valid == 0) {
			hit = false;
			replaceBlock(cm, mmBlockNumber, tagValue, operation, i, opCount);
			printChart(mmAddress, mmBlockNumber, cmSetNumber, i, hit, setStart, setStart + mapping, mapping);
			return hit;
		}
	}
	// Set is full
	int index = getOldestBlock(cm, setStart, setStart + mapping);
	hit = false;
	replaceBlock(cm, mmBlockNumber, tagValue, operation, index, opCount);
	printChart(mmAddress, mmBlockNumber, cmSetNumber, index, hit, setStart, setStart + mapping, mapping);
	return hit;
}


int main() {
	char flag = 'y';

	while (flag == 'y' || flag == 'Y') {
		int mmSize = 0, cmSize = 0, cbSize = 0, mapping = 0;
		int lines = 0, offset = 0, index = 0, tag = 0, totalCmSize = 0;
		char rPolicy;
		string inputFile;

		flag = '0';

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

		int refCount = 0, mmAddress = 0, hitCount = 0;
		char operation = 'R';
		bool hit = false;

		string line;
		ifstream file(inputFile);
		cout << endl;

		file >> refCount;
		int refs[refCount] = {0};

		printLabel();
		for (int i = 0; i < refCount; i++) {
			file >> operation;
			operation = toupper(operation);
			file >> mmAddress;
			hit = performOperation(cm, mmAddress, cbSize, cmSetCount, mapping, lines, tag, operation, i);
			refs[i] = mmAddress / cbSize;

			if (hit == true)
				hitCount += 1;
		}
		int mHR = maxHitRate(refs, refCount);
		cout << endl;
		cout << "Highest possible hit rate = " << mHR << "/" << refCount;
		cout << " = ";
		if (mHR > 0)
			cout << (float(mHR) / refCount) * 100;
		else
			cout << 0;
		cout << "%";
		cout << endl;
		cout << "Actual hit rate = " << hitCount << "/" << refCount;
		cout << " = ";
		if (hitCount > 0)
			cout << (float(hitCount) / refCount) * 100;
		else
			cout << 0;
		cout << "%" << endl;

		cout << endl;
		cout << "Final \"status\" of the cache:" << endl;

		displayCache(cm, cmBlockCount, tag, mmSize/cbSize);
		file.close();

		do {
			cout << endl;
			cout << "Continue? (y = yes, n = no): ";
			cin >> flag;
			flag = tolower(flag);
		} while (flag != 'y' && flag != 'n');

		cout << endl;

		if (flag == 'n')
			cout << "(program exits)" << endl;
	}
}
