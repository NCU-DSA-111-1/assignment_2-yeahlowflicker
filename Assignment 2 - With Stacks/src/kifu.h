#ifndef KIFU
#define KIFU
#endif

#include <stdio.h>

#define KIFUSIZE 128


typedef struct {
	PieceType type;
	int oldX;
	int oldY;
	int newX;
	int newY;
	int player;
	int isEaten;
} KifuRecord;


typedef struct {
	unsigned stackTop;
	KifuRecord records[KIFUSIZE];
} KifuStack;

int IsStackEmpty(KifuStack* stack);

int IsStackFull(KifuStack* stack);

void PushKifu(KifuStack* stack, const KifuRecord record);

KifuRecord* PopKifu(KifuStack* const stack);

void WriteKifuToFile(const char* fileName, KifuStack* const stack);

void LoadKifuFromFile(const char* fileName, KifuStack* const stack);
