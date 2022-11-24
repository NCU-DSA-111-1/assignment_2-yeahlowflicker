#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "shogilib.h"
#include "kifu.h"


void PrintNode(NodePtr const node) {
	// const KifuRecord record = node->record;
	// printf("%d %d %d %d %d %d %d\n", record.oldX, record.oldY, record.newX, record.newY, record.type, record.isEaten, record.player);
	printf("%16p\t%16p\t%16p\n", node->prev, node, node->next);
	if (node->next != NULL)
		PrintNode(node->next);
}

void PrintList(KifuList* const list) {
	PrintNode(list->head);
}


int IsListEmpty(KifuList* const list) {
	return (list->head == NULL && list->tail==NULL);
}


int GetListCount(NodePtr const node) {
	if (node == NULL)
		return 0;
	if (node->next != NULL)
		return GetListCount(node->next) + 1;
	return 1;
}




void PushKifu(KifuList* const list, const KifuRecord record) {
	NodePtr newNode = (NodePtr)calloc(1, sizeof(Node));
	newNode->record = record;
	newNode->next = NULL;

	if (IsListEmpty(list)) {
		list->head = newNode;
		list->tail = newNode;
		newNode->prev = NULL;
	}
	else {
		newNode->prev = list->tail;
		list->tail->next = newNode;
		list->tail = newNode;
	}
}



KifuRecord* PopKifu(KifuList* const list) {
	if (IsListEmpty(list)) return NULL;

	NodePtr const tailTemp = list->tail;
	KifuRecord* result = &(tailTemp->record);
	tailTemp->prev->next = NULL;
	list->tail = tailTemp->prev;

	return result;
}


int HasPreviousStepForPlayer(NodePtr const node, const int player) {
	if ((node->record).player == player) return TRUE;
	if (node->prev)
		return HasPreviousStepForPlayer(node->prev, player);
	else return FALSE;
}

int CheckHasRevertibleSteps(KifuList* const list, const int currentPlayer) {
    // NodePtr currentNode = list->tail;
    // while (currentNode != list->head) {
    //     if (currentNode->record.player == currentPlayer)
    //         return TRUE;
    //     currentNode = currentNode->prev;
    // }
    // return FALSE;
	return HasPreviousStepForPlayer(list->tail, currentPlayer);
}


void WriteNodeToFileRecursive(FILE* const file, NodePtr const node) {
	const KifuRecord record = node->record;
	fprintf(file, "%d %d %d %d %d %d %d\n", record.oldX, record.oldY, record.newX, record.newY, record.type, record.isEaten, record.player);
	if (node->next)
		WriteNodeToFileRecursive(file, node->next);
}

void WriteKifuToFile(const char* fileName, KifuList* const list) {
	FILE* file = fopen(fileName, "w+");
	printf("READ FILE: %s\n", fileName);
	assert(file != NULL);

	WriteNodeToFileRecursive(file, list->head);

	fclose(file);
}


void LoadKifuFromFile(const char* fileName, KifuList* const list) {
	FILE* file = fopen(fileName, "r");
	assert(file != NULL);

	KifuRecord record;
	int i = 0;
	int type = 0;

	while (fscanf(file, "%d %d %d %d %d %d %d", &(record.oldX), &(record.oldY), &(record.newX), &(record.newY), &type, &(record.isEaten), &(record.player)) != EOF) {
		record.type = (PieceType)type;
		PushKifu(list, record);
		i++;
	}
	printf("Count: %d\n", i);

	printf("LOAD FILE: %s\n", fileName);
}
