#include <stdio.h>
#include "shogilib.h"
#include "kifu.h"

//	Print available command options
void PrintControlMessage(NodePtr const viewPtr, const unsigned flagStartPhase) {
	printf("\nOptions:\n");
	if (viewPtr->next)
		printf("Forward (p)\n");
	if (viewPtr->prev || !flagStartPhase)
		printf("Backward (b)\n");
	printf("\n\n>> ");
}



//	Replay one step forward
void ReplayForward(Piece* const pieces, KifuList* const list, NodePtr* const viewPtr) {

	const KifuRecord record = (*viewPtr)->record;

	//	Get the corresponding piece
	Piece* piece = GetPieceAtPosition(pieces, record.oldX, record.oldY);

	//	Directly assign the record data to that piece
	if (piece != NULL)
		AssignPiece(piece, record.player, record.type, record.newX, record.newY, record.isEaten);


	//	If the action is 'eat', replay one more time
	if (record.isEaten) {
		*viewPtr = (*viewPtr)->next;
		ReplayForward(pieces, list, viewPtr);
	}
	else
		DrawBoard(pieces, -1, -1);
}


//	Replay one step backwards
void ReplayBackward(Piece* const pieces, KifuList* const list, NodePtr* const viewPtr) {

	const KifuRecord record = (*viewPtr)->record;

	//	Get the corresponding piece
	Piece* piece = GetPieceAtPosition(pieces, record.newX, record.newY);

	//	Directly assign the record data to that piece
	if (piece != NULL)
		AssignPiece(piece, record.player, record.type, record.oldX, record.oldY, FALSE);

	//	Check to see if the previous step is "eat"
	//	if so, execute that step as well
	if ((*viewPtr)->prev) {
		//	Copy the previous pointer
		NodePtr tmp = (*viewPtr)->prev;

		//	Check for "eat"
		if (tmp->record.isEaten) {
			//	Move the pointer backwards and replay again
			*viewPtr = tmp;
			ReplayBackward(pieces, list, viewPtr);
			DrawBoard(pieces, -1, -1);
		}
	}
	else
		DrawBoard(pieces, -1, -1);
}




//	The main replay algorithm
void ShogiReplay(char* kifuFileName) {

	//	The dynamic pieces
    Piece pieces[40];

	//	Initialize the board as usual
    InitializeBoard(pieces);

	//	List for handling replay
	KifuList kifuList;
	kifuList.head = kifuList.tail = NULL;

	//	A node pointer which moves back and forth the list
	NodePtr viewPtr = NULL;

	//	A flag indicating whether at the starting phase of the game
	unsigned flagStartPhase = TRUE;
	unsigned flagEndPhase = FALSE;

	//	Load the data saved in the file to the list
	LoadKifuFromFile(kifuFileName, &kifuList);
	viewPtr = kifuList.head;


	//	Stores the user command
	char controlInput = ' ';

	//	Print the initial control messages
	PrintControlMessage(viewPtr, flagStartPhase);


	//	Main control loop
	while (TRUE) {
		if (scanf("%c", &controlInput)) {

			//	Quit
			if (controlInput == 'q')
				break;

			//	Replay forward
			else if (controlInput == 'p') {
				flagStartPhase = FALSE;

				ReplayForward(pieces, &kifuList, &viewPtr);
				printf("REPLAY FORWARD\n");
				PrintControlMessage(viewPtr, flagStartPhase);

				if (viewPtr != kifuList.tail)
					viewPtr = viewPtr->next;
				else flagEndPhase = TRUE;
			}

			//	Replay backward
			else if (controlInput == 'b') {
				if (flagEndPhase) {
					ReplayBackward(pieces, &kifuList, &viewPtr);
				}
				else {
					if (viewPtr != kifuList.head)
						viewPtr = viewPtr->prev;
					else flagStartPhase = TRUE;
					if (!flagEndPhase)
						ReplayBackward(pieces, &kifuList, &viewPtr);
				}
				flagEndPhase = FALSE;
				printf("REPLAY BACKWARD\n");
				PrintControlMessage(viewPtr, flagStartPhase);
			}
		}
	}

	//	Quit message
	printf("END REPLAY\n");
}
