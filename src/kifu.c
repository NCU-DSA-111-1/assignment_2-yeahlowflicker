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
	printf("READ FILE: %s\n", fileName);
	assert(file != NULL);
	
	for (int i = 0; i < stack->stackTop; ++i) {
		const KifuRecord record = (stack->records)[i];
		fprintf(file, "%d %d %d %d %d %d %d\n", record.oldX, record.oldY, record.newX, record.newY, record.type, record.isEaten, record.player);
	}

	fclose(file);
}


void LoadKifuFromFile(const char* fileName, KifuStack* const stack) {
	FILE* file = fopen(fileName, "r");
	assert(file != NULL);

	KifuRecord record;
	int i = 0;
	int type = 0;

	while (fscanf(file, "%d %d %d %d %d %d %d", &(record.oldX), &(record.oldY), &(record.newX), &(record.newY), &type, &(record.isEaten), &(record.player)) != EOF) {
		record.type = (PieceType)type;
		PushKifu(stack, record);
		i++;
	}

	printf("LOAD FILE: %s\n", fileName);
}


void PushKifu(KifuStack* const stack, const KifuRecord record) {
	if (IsStackFull(stack)) return;

	(stack->records)[stack->stackTop] = record;
	(stack->stackTop)++;
}


KifuRecord* PopKifu(KifuStack* const stack) {
	if (IsStackEmpty(stack)) return NULL;

	KifuRecord* result = &(stack->records)[stack->stackTop-1];
	(stack->stackTop)--;

	return result;
}
