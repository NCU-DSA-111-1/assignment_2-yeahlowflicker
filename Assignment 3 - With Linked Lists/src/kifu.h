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


typedef struct node {
	KifuRecord record;
	struct node* prev;
	struct node* next;
} Node;
typedef struct node* NodePtr;

typedef struct {
	NodePtr head;
	NodePtr tail;
} KifuList;

// typedef struct {
// 	unsigned stackTop;
// 	KifuRecord records[KIFUSIZE];
// } KifuStack;

int IsListEmpty(KifuList* list);


void PushKifu(KifuList* list, const KifuRecord record);

KifuRecord* PopKifu(KifuList* const list);

void WriteKifuToFile(const char* fileName, KifuList* const list);

void LoadKifuFromFile(const char* fileName, KifuList* const list);

int CheckHasRevertibleSteps(KifuList* const list, const int currentPlayer);


void PrintNode(NodePtr const node);

void PrintList(KifuList* const list);
