#include <stdio.h>
#include <assert.h>
#include "shogilib.h"
#include "kifu.h"


int IsStackEmpty(KifuStack* stack) {
	return (stack->stackTop == 0);
}

int IsStackFull(KifuStack* stack) {
	return (stack->stackTop == KIFUSIZE+1);
}


void WriteKifuToFile(const char* fileName, KifuStack* const stack) {
	FILE* file = fopen(fileName, "w+");
	printf("FILE: %s\n", fileName);
	assert(file != NULL);
	
	for (int i = 0; i < stack->stackTop; ++i) {
		const KifuRecord record = (stack->records)[i];
		fprintf(file, "%d %d %d %d %d %d\n", record.oldX, record.oldY, record.newX, record.newY, record.type, record.player);
	}

	fclose(file);
}


void PushKifu(KifuStack* const stack, const KifuRecord record, const char* fileName) {
	if (IsStackFull(stack)) return;

	(stack->records)[stack->stackTop] = record;
	(stack->stackTop)++;

//	AppendKifuToFile(fileName, record);
	WriteKifuToFile(fileName, stack);
}


KifuRecord* PopKifu(KifuStack* const stack, const char* fileName) {
	if (IsStackEmpty(stack)) return NULL;

	KifuRecord* result = &(stack->records)[stack->stackTop-1];
	(stack->stackTop)--;

	WriteKifuToFile(fileName, stack);

	return result;
}
