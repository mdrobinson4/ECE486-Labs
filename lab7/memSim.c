#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "memSimFuncs.h"

int main() {
	char replacement, assDegree;
	FILE *fp;

	assDegree = getAssociativity();
	replacement = getReplacementPolicy();
	fp = getFile();
	MEMORY *m = readFile(fp);
	return 0;
}
