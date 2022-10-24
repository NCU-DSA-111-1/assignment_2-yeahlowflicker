#include <stdio.h>
#include "shogilib.h"
#include "kifu.h"


// Request input from the user about the next action
void RequestInput(Piece* pieces, PieceBehavior* pieceBehaviors, int currentPlayer, KifuStack* kifuStack, const char* kifuFileName) {

    // Print the current player's name
    if (currentPlayer == 0)
        printf("\n\x1b[32mPlayer %d\x1b[0m:\n", currentPlayer);
    else
        printf("\n\x1b[31mPlayer %d\x1b[0m:\n", currentPlayer);

	char command;
	scanf("%c", &command);

	if (command == 's' || command == 'S') {
		WriteKifuToFile(kifuFileName, kifuStack);
		RequestInput(pieces, pieceBehaviors, currentPlayer, kifuStack, kifuFileName);
		return;
	}



    //  Ask the current player which piece to select
    int oldX = -1;
    int oldY = -1;
    printf("Select piece (x, y) >> ");    
	scanf("%d", &oldX);

	if (oldX == 0) {
		//	Catch empty stack
		if (IsStackEmpty(kifuStack)) {
			printf("Kifu is empty and is not revertible.");
			RequestInput(pieces, pieceBehaviors, currentPlayer, kifuStack, kifuFileName);
			return;
		}
		//	Pop again and see if player has revertible steps
		else if (kifuStack->stackTop - currentPlayer == 0) {
			printf("You have no revertible steps.");
			RequestInput(pieces, pieceBehaviors, currentPlayer, kifuStack, kifuFileName);
			return;
		}


		//	Get the previous move by this player
		//	The opponent's move has to be popped and pushed back
		KifuRecord prev1 = *PopKifu(kifuStack);
		KifuRecord prev2 = *PopKifu(kifuStack);

		MovePiece(GetPieceAtPosition(pieces, prev1.newX, prev1.newY), prev1.oldX, prev1.oldY);

		MovePiece(GetPieceAtPosition(pieces, prev2.newX, prev2.newY), prev2.oldX, prev2.oldY);

    	DrawBoard(pieces, oldX, oldY);
		return;
	}

	scanf("%d", &oldY);

    //  Check if there is a piece at the position
    if (GetPieceAtPosition(pieces, oldX, oldY) == NULL) {
        printf("Piece not found.\n\n");
        RequestInput(pieces, pieceBehaviors, currentPlayer, kifuStack, kifuFileName);
        return;
    }

    //  Get reference to an existing piece
    Piece* piece_ptr = GetPieceAtPosition(pieces, oldX, oldY);

    //  Check whether the selected piece belongs to the current player
    if (piece_ptr->player != currentPlayer) {
        printf("You cannot select a piece which does not belong to you!\n\n");
        RequestInput(pieces, pieceBehaviors, currentPlayer, kifuStack, kifuFileName);
        return;
    }
    DrawBoard(pieces, oldX, oldY);

    //  Request new position and move the piece
    int newX = -1;
    int newY = -1;
    printf("[%s] -> Move piece (x, y) >> ", GetHans(piece_ptr->type));
    scanf("%d %d", &newX, &newY);


    //  Validate the intended move
    if (CheckMoveRule(pieces, pieceBehaviors, piece_ptr->type, oldX, oldY, newX, newY, currentPlayer) == FALSE) {
        printf("This move is illegal!\n\n");
        RequestInput(pieces, pieceBehaviors, currentPlayer, kifuStack, kifuFileName);
        return;
    }


    //  Check for overlaps. Determine whether to prohibit the move or issue an 'eat'
    if (GetPieceAtPosition(pieces, newX, newY) != NULL) {
        Piece destinationPiece = *GetPieceAtPosition(pieces, newX, newY);
        if (destinationPiece.player == currentPlayer) {
            printf("That space has already been occupied!\n\n");
            RequestInput(pieces, pieceBehaviors, currentPlayer, kifuStack, kifuFileName);
            return;
        }
        else {
	    	Piece* destinationPiece_ptr = GetPieceAtPosition(pieces, newX, newY);
		    destinationPiece_ptr->isEaten = TRUE;

			//	Record the "eat" action
			KifuRecord eatRecord;
			eatRecord.type = destinationPiece_ptr->type;
			eatRecord.oldX = destinationPiece_ptr->x;
			eatRecord.oldY = destinationPiece_ptr->y;
			eatRecord.newX = destinationPiece_ptr->x;
			eatRecord.newY = destinationPiece_ptr->y;
			eatRecord.isEaten = destinationPiece_ptr->isEaten;
			eatRecord.player = destinationPiece_ptr->player;
			PushKifu(kifuStack, eatRecord);
        }
    }


    //  Move the piece to the intended location
    MovePiece(piece_ptr, newX, newY);


	//	Record the move
	KifuRecord moveRecord;
	moveRecord.type = piece_ptr->type;
	moveRecord.oldX = oldX;
	moveRecord.oldY = oldY;
	moveRecord.newX = newX;
	moveRecord.newY = newY;
	moveRecord.isEaten = piece_ptr->isEaten;
	moveRecord.player = currentPlayer;
	PushKifu(kifuStack, moveRecord);


    //  Redraw the board without highlights
    DrawBoard(pieces, -1, -1);


    //  Log the move
    printf("Moved %s to (%d, %d)\n", GetHans(piece_ptr->type), piece_ptr->x, piece_ptr->y);
}


// Entry point to a single game
void ShogiGame(char* kifuFileName) {
    Piece pieces[40];
    PieceBehavior pieceBehaviors[PIECE_TYPE_COUNT];
    int currentPlayer = 0;

	KifuStack kifuStack;
	kifuStack.stackTop = 0;

    InitializeBehaviors(pieceBehaviors);
    InitializeBoard(pieces);

    while (TRUE) {
        RequestInput(pieces, pieceBehaviors, currentPlayer, &kifuStack, kifuFileName);

        // Change the current player;
        currentPlayer = currentPlayer == 0 ? 1 : 0;
    }
}




