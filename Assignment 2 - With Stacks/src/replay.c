#include <stdio.h>
#include "shogilib.h"
#include "kifu.h"


//	Print available command options
void PrintControlMessage(KifuStack* stack1, KifuStack* stack2) {
	printf("\nOptions:\n");
	if (!IsStackEmpty(stack1))
		printf("Forward (p)\n");
	if (!IsStackEmpty(stack2))
		printf("Backward (b)\n");
	printf("\n\n>> ");
}



//	Replay one step forward
void ReplayForward(Piece* pieces, KifuStack* stack1, KifuStack* stack2) {

	//	Pop stack1
	KifuRecord* record_ptr = PopKifu(stack1);
	if (record_ptr == NULL) {
		printf("No more forwardable steps!\n");
		return;
	}

	//	Push to stack2
	PushKifu(stack2, *record_ptr);

	//	Get the corresponding piece
	Piece* piece = GetPieceAtPosition(pieces, record_ptr->oldX, record_ptr->oldY);
	
	//	Directly assign the record data to that piece
	if (piece != NULL)
		AssignPiece(piece, record_ptr->player, record_ptr->type, record_ptr->newX, record_ptr->newY, record_ptr->isEaten);
	
	//	If the action is 'eat', replay one more time
	if (record_ptr->isEaten)
		ReplayForward(pieces, stack1, stack2);
	else
		DrawBoard(pieces, -1, -1);
}


//	Replay one step backwards
void ReplayBackward(Piece* pieces, KifuStack* stack1, KifuStack* stack2) {

	// Pop stack2
	KifuRecord* record_ptr = PopKifu(stack2);
	if (record_ptr == NULL) {
		printf("No more revertible steps!\n");
		return;
	}

	//	Push to stack1
	PushKifu(stack1, *record_ptr);

	//	Get the corresponding piece
	Piece* piece = GetPieceAtPosition(pieces, record_ptr->newX, record_ptr->newY);
	
	//	Directly assign the record data to that piece
	if (piece != NULL)
		AssignPiece(piece, record_ptr->player, record_ptr->type, record_ptr->oldX, record_ptr->oldY, record_ptr->isEaten);
		
	//	If the action is 'eat', replay one more time
	if (record_ptr->isEaten)
		ReplayBackward(pieces, stack1, stack2);
	else
		DrawBoard(pieces, -1, -1);
}


//	The steps are inversed when read from the file
//	this function reverses them back to normal
void ReverseStack(KifuStack* from, KifuStack* to) {
	while (from->stackTop > 0)
		PushKifu(to, *PopKifu(from));
}



//	The main replay algorithm
void ShogiReplay(char* kifuFileName) {

	//	The dynamic pieces
    Piece pieces[40];

	//	Initialize the board as usual
    InitializeBoard(pieces);

	//	Stacks for handling replay
	KifuStack kifuStack1, kifuStack2;
	kifuStack1.stackTop = kifuStack2.stackTop = 0;

	//	Load the data saved in the file to stack2
	LoadKifuFromFile(kifuFileName, &kifuStack2);

	//	The loaded data is reversed, so reverse is and push to stack1
	ReverseStack(&kifuStack2, &kifuStack1);

	//	Stores the user command
	char controlInput = ' ';

	PrintControlMessage(&kifuStack1, &kifuStack2);


	//	Main control loop
	while (TRUE) {
		if (scanf("%c", &controlInput)) {

			//	Quit
			if (controlInput == 'q')
				break;

			//	Replay forward
			else if (controlInput == 'p') {
				ReplayForward(pieces, &kifuStack1, &kifuStack2);

				printf("REPLAY FORWARD\n");
				PrintControlMessage(&kifuStack1, &kifuStack2);
			}

			//	Replay backward
			else if (controlInput == 'b') {
				ReplayBackward(pieces, &kifuStack1, &kifuStack2);

				printf("REPLAY BACKWARD\n");
				PrintControlMessage(&kifuStack1, &kifuStack2);
			}
		}
	}

	//	Quit message
	printf("END REPLAY\n");
}
