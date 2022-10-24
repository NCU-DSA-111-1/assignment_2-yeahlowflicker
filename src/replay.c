#include <stdio.h>
#include "shogilib.h"
#include "kifu.h"


void ReplayForward(Piece* pieces, KifuStack* stack1, KifuStack* stack2) {
	KifuRecord* record_ptr = PopKifu(stack1);
	if (record_ptr == NULL) {
		printf("No more forwardable steps!\n");
		return;
	}

	PushKifu(stack2, *record_ptr);

	Piece* piece = NULL;
	if (record_ptr->isEaten)
		piece = GetEatenPieceAtPosition(pieces, record_ptr->oldX, record_ptr->oldY);
	else
		piece = GetPieceAtPosition(pieces, record_ptr->oldX, record_ptr->oldY);
	
	if (piece != NULL)
		AssignPiece(piece, record_ptr->player, record_ptr->type, record_ptr->newX, record_ptr->newY, record_ptr->isEaten);
	
	DrawBoard(pieces, record_ptr->newX, record_ptr->newY);
}


void ReplayBackward(Piece* pieces, KifuStack* stack1, KifuStack* stack2) {
	KifuRecord* record_ptr = PopKifu(stack2);
	if (record_ptr == NULL) {
		printf("No more revertible steps!\n");
		return;
	}

	PushKifu(stack1, *record_ptr);

	Piece* piece = NULL;
	if (record_ptr->isEaten)
		piece = GetEatenPieceAtPosition(pieces, record_ptr->newX, record_ptr->newY);
	else
		piece = GetPieceAtPosition(pieces, record_ptr->newX, record_ptr->newY);
	
	if (piece != NULL)
		AssignPiece(piece, record_ptr->player, record_ptr->type, record_ptr->oldX, record_ptr->oldY, record_ptr->isEaten);
	
	DrawBoard(pieces, record_ptr->oldX, record_ptr->oldY);
}


//	The steps are inversed when read from the file
//	this function reverses them back to normal
void ReverseStack(KifuStack* from, KifuStack* to) {
	while (from->stackTop > 0) {
		PushKifu(to, *PopKifu(from));
	}
}



void ShogiReplay(char* kifuFileName) {

    Piece pieces[40];
    InitializeBoard(pieces);

	KifuStack kifuStack1, kifuStack2;
	kifuStack1.stackTop = kifuStack2.stackTop = 0;
	LoadKifuFromFile(kifuFileName, &kifuStack2);
	ReverseStack(&kifuStack2, &kifuStack1);

	char controlInput = ' ';
	printf("\nOptions:\nForward (p)\nBackward (b)\n\n>> ");

	while (TRUE) {
		if (scanf("%c", &controlInput)) {
			if (controlInput == 'q')
				break;
			else if (controlInput == 'p') {
				ReplayForward(pieces, &kifuStack1, &kifuStack2);
				printf("REPLAY FORWARD\n");
				printf("\nOptions:\nForward (p)\nBackward (b)\n\n>> ");
			}
			else if (controlInput == 'b') {
				ReplayBackward(pieces, &kifuStack1, &kifuStack2);
				printf("REPLAY BACKWARD\n");
				printf("\nOptions:\nForward (p)\nBackward (b)\n\n>> ");
			}
		}
	}

	printf("END REPLAY\n");
}
