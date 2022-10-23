#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shogilib.h"


char* GetHans(PieceType type) {
    switch (type) {
        case King:     return "王"; break;
        case Rook:     return "飛"; break;
        case Bishop:   return "角"; break;
        case Gold:     return "金"; break;
        case Silver:   return "銀"; break;
        case Knight:   return "桂"; break;
        case Lance:    return "香"; break;
        case Pawn:     return "步"; break;
        default:       return "步"; break;
    }
}

void DrawBoard(Piece* const pieces, int highlightX, int highlightY) {
    system("clear");

    // Draw the pieces captured by Player 1
    printf("———————————————————————————\n");
    for (int i = 0; i < 40; ++i) {
        if (pieces[i].isEaten && pieces[i].player == 0)
            printf(" \x1b[32m%s\x1b[0m", GetHans(pieces[i].type));
    }
    printf("\n———————————————————————————\n");


    // Draw the top scales
    printf("\n 9  8  7  6  5  4  3  2  1");
    printf("\n+——+——+——+——+——+——+——+——+——+\n");

    // Draw the main grid
    for (int y = 1; y <= 9; ++y) {
        for (int x = 9; x > 0; --x) {

            int isEmpty = TRUE;

            for (int i = 0; i < 40; ++i) {
                if (pieces[i].x == x && pieces[i].y == y) {
                    if (pieces[i].isEaten)
                        continue;

                    if (pieces[i].x == highlightX && pieces[i].y == highlightY)
                        printf("|\x1b[33m%s\x1b[0m", GetHans(pieces[i].type));
                    else if (pieces[i].player == 0)
                        printf("|\x1b[32m%s\x1b[0m", GetHans(pieces[i].type));
                    else
                        printf("|\x1b[31m%s\x1b[0m", GetHans(pieces[i].type));
                    isEmpty = FALSE;
                    break;
                }
            }
            if (isEmpty)
                printf("|  ");
        }

        printf("| %d", y);
        // printf("| %d", y);
        printf("\n+——+——+——+——+——+——+——+——+——+\n");
    }


    // Draw the pieces captured by Player 0
    printf("\n———————————————————————————\n");
    for (int i = 0; i < 40; ++i) {
        if (pieces[i].isEaten && pieces[i].player == 1)
            printf(" \x1b[31m%s\x1b[0m", GetHans(pieces[i].type));
    }
    printf("\n———————————————————————————\n");
}


// Directly assign data to a piece. Used for initialization
void AssignPiece(Piece* piece, int player, PieceType type, int x, int y) {
    piece->player = player;
    piece->type = type;
    piece->x = x;
    piece->y = y;
    piece->isEaten = FALSE;
}


// Initialize the behaviors of each type
void InitializeBehaviors(PieceBehavior* pieceBehaviors) {
    for (int i = 0; i < PIECE_TYPE_COUNT; ++i) {
        pieceBehaviors[i].type = (PieceType)i;
        switch (pieceBehaviors[i].type) {
            case King:
                pieceBehaviors[i].n = TRUE;
                pieceBehaviors[i].ne = TRUE;
                pieceBehaviors[i].e = TRUE;
                pieceBehaviors[i].se = TRUE;
                pieceBehaviors[i].s = TRUE;
                pieceBehaviors[i].sw = TRUE;
                pieceBehaviors[i].w = TRUE;
                pieceBehaviors[i].nw = TRUE;
                pieceBehaviors[i].allowMulti = FALSE;
                pieceBehaviors[i].allowJump = FALSE;
                break;

            case Rook:
                pieceBehaviors[i].n = TRUE;
                pieceBehaviors[i].ne = FALSE;
                pieceBehaviors[i].e = TRUE;
                pieceBehaviors[i].se = FALSE;
                pieceBehaviors[i].s = TRUE;
                pieceBehaviors[i].sw = FALSE;
                pieceBehaviors[i].w = TRUE;
                pieceBehaviors[i].nw = FALSE;
                pieceBehaviors[i].allowMulti = TRUE;
                pieceBehaviors[i].allowJump = FALSE;
                break;

            case Bishop:
                pieceBehaviors[i].n = FALSE;
                pieceBehaviors[i].ne = TRUE;
                pieceBehaviors[i].e = FALSE;
                pieceBehaviors[i].se = TRUE;
                pieceBehaviors[i].s = FALSE;
                pieceBehaviors[i].sw = TRUE;
                pieceBehaviors[i].w = FALSE;
                pieceBehaviors[i].nw = TRUE;
                pieceBehaviors[i].allowMulti = FALSE;
                pieceBehaviors[i].allowJump = FALSE;
                break;

            case Gold:
                pieceBehaviors[i].n = TRUE;
                pieceBehaviors[i].ne = TRUE;
                pieceBehaviors[i].e = TRUE;
                pieceBehaviors[i].se = FALSE;
                pieceBehaviors[i].s = TRUE;
                pieceBehaviors[i].sw = FALSE;
                pieceBehaviors[i].w = TRUE;
                pieceBehaviors[i].nw = TRUE;
                pieceBehaviors[i].allowMulti = FALSE;
                pieceBehaviors[i].allowJump = FALSE;
                break;

            case Silver:
                pieceBehaviors[i].n = TRUE;
                pieceBehaviors[i].ne = TRUE;
                pieceBehaviors[i].e = FALSE;
                pieceBehaviors[i].se = TRUE;
                pieceBehaviors[i].s = FALSE;
                pieceBehaviors[i].sw = TRUE;
                pieceBehaviors[i].w = FALSE;
                pieceBehaviors[i].nw = TRUE;
                pieceBehaviors[i].allowMulti = FALSE;
                pieceBehaviors[i].allowJump = FALSE;
                break;

            case Knight:
                pieceBehaviors[i].n = FALSE;
                pieceBehaviors[i].ne = TRUE;
                pieceBehaviors[i].e = FALSE;
                pieceBehaviors[i].se = FALSE;
                pieceBehaviors[i].s = FALSE;
                pieceBehaviors[i].sw = FALSE;
                pieceBehaviors[i].w = FALSE;
                pieceBehaviors[i].nw = TRUE;
                pieceBehaviors[i].allowMulti = TRUE;
                pieceBehaviors[i].allowJump = TRUE;
                break;

            case Lance:
                pieceBehaviors[i].n = TRUE;
                pieceBehaviors[i].ne = FALSE;
                pieceBehaviors[i].e = FALSE;
                pieceBehaviors[i].se = FALSE;
                pieceBehaviors[i].s = FALSE;
                pieceBehaviors[i].sw = FALSE;
                pieceBehaviors[i].w = FALSE;
                pieceBehaviors[i].nw = FALSE;
                pieceBehaviors[i].allowMulti = TRUE;
                pieceBehaviors[i].allowJump = FALSE;
                break;

            case Pawn:
                pieceBehaviors[i].n = TRUE;
                pieceBehaviors[i].ne = FALSE;
                pieceBehaviors[i].e = FALSE;
                pieceBehaviors[i].se = FALSE;
                pieceBehaviors[i].s = FALSE;
                pieceBehaviors[i].sw = FALSE;
                pieceBehaviors[i].w = FALSE;
                pieceBehaviors[i].nw = FALSE;
                pieceBehaviors[i].allowMulti = FALSE;
                pieceBehaviors[i].allowJump = FALSE;
                break;
        }
    }
}


// Initialize the board. Generates the pieces and layout at the start
void InitializeBoard(Piece* const pieces) {
    for (int x = 9; x > 0; --x) {
        AssignPiece(&pieces[x-1], 0, Pawn, x, 7);
        AssignPiece(&pieces[19+x], 1, Pawn, x, 3);
    }

    // Player 0 (Bottom)
    AssignPiece(&pieces[9], 0, Bishop, 8, 8);
    AssignPiece(&pieces[10], 0, Rook, 2, 8);
    AssignPiece(&pieces[11], 0, Lance, 9, 9);
    AssignPiece(&pieces[12], 0, Knight, 8, 9);
    AssignPiece(&pieces[13], 0, Silver, 7, 9);
    AssignPiece(&pieces[14], 0, Gold, 6, 9);
    AssignPiece(&pieces[15], 0, King, 5, 9);
    AssignPiece(&pieces[16], 0, Gold, 4, 9);
    AssignPiece(&pieces[17], 0, Silver, 3, 9);
    AssignPiece(&pieces[18], 0, Knight, 2, 9);
    AssignPiece(&pieces[19], 0, Lance, 1, 9);

    //  Player 1 (Top)
    AssignPiece(&pieces[29], 1, Rook, 2, 2);
    AssignPiece(&pieces[30], 1, Bishop, 8, 2);
    AssignPiece(&pieces[31], 1, Lance, 9, 1);
    AssignPiece(&pieces[32], 1, Knight, 8, 1);
    AssignPiece(&pieces[33], 1, Silver, 7, 1);
    AssignPiece(&pieces[34], 1, Gold, 6, 1);
    AssignPiece(&pieces[35], 1, King, 5, 1);
    AssignPiece(&pieces[36], 1, Gold, 4, 1);
    AssignPiece(&pieces[37], 1, Silver, 3, 1);
    AssignPiece(&pieces[38], 1, Knight, 2, 1);
    AssignPiece(&pieces[39], 1, Lance, 1, 1);

    DrawBoard(pieces, -1, -1);
}


// Gets a pointer to a piece sitting at a specific location
Piece* GetPieceAtPosition(Piece* pieces, int x, int y) {
    for (int i = 0; i < 40; ++i) {
        if (pieces[i].x == x && pieces[i].y == y) {
            if (pieces[i].isEaten) continue;
            return &pieces[i];
        }
    }
    return NULL;
}


// Gets the corresponding behavior for the type
PieceBehavior* GetPieceBehavior(PieceBehavior* pieceBehaviors, PieceType type) {
    for (int i = 0; i < PIECE_TYPE_COUNT; ++i) {
        // printf("%s\n", GetHans(pieceBehaviors[i].type));
        if (pieceBehaviors[i].type == type)
            return &pieceBehaviors[i];
    }
    return NULL;
}



// Validate if a move is legal
int CheckMoveRule(Piece* pieces, PieceBehavior* pieceBehaviors, PieceType type, int oldX, int oldY, int newX, int newY, int currentPlayer) {
    // Calculate the horizontal and vertical move delta
    int dx = oldX - newX;
    int dy = oldY - newY;

    // Inverse directions for player 1
    if (currentPlayer == 1) { dx *= -1; dy *= -1; }

    int direction = -1;

    // Determine the move direction based on dx and dy
    if      (dx == 0 && dy > 0)     direction = 0;
    else if (dx > 0 && dy > 0)      direction = 1;
    else if (dx > 0 && dy == 0)     direction = 2;
    else if (dx > 0 && dy < 0)      direction = 3;
    else if (dx == 0 && dy < 0)     direction = 4;
    else if (dx < 0 && dy < 0)      direction = 5;
    else if (dx < 0 && dy == 0)     direction = 6;
    else if (dx < 0 && dy > 0)      direction = 7;


    // Catch unrecognized types
    if (GetPieceBehavior(pieceBehaviors, type) == NULL) {
        printf("Type not found.\n");
        return FALSE;
    }


    // Get the behavior for the given type
    PieceBehavior behavior = *GetPieceBehavior(pieceBehaviors, type);

    //  Invalidate if multi-cell movements is not allowed, but delta > 1
    if (!behavior.allowMulti) {
        if (abs(dx) > 1 || abs(dy) > 1)
            return FALSE;
    }

    //  Check if the piece is jumping and whether it is allowed to jump
    if (!behavior.allowJump) {
		//  Scan horizontally
		if (dy == 0)
	    	for (int x = MIN(oldX, newX)+1; x < MAX(oldX, newX); ++x)
				if (GetPieceAtPosition(pieces, x, oldY) != NULL)
		    		return FALSE;

		//  Scan vertically
		if (dx == 0) 
	    	for (int y = MIN(oldY, newY)+1; y < MAX(oldY, newY); ++y)
				if (GetPieceAtPosition(pieces, oldX, y) != NULL)
		    		return FALSE;
    }


    //  Return whether the direction is allowed
    switch(direction) {
        case 0: return behavior.n;
        case 1: return behavior.ne;
        case 2: return behavior.e;
        case 3: return behavior.se;
        case 4: return behavior.s;
        case 5: return behavior.sw;
        case 6: return behavior.w;
        case 7: return behavior.nw;
    }

    return FALSE;
}


// Move a piece from one location to another
void MovePiece(Piece* piece, int x, int y) {
    printf("MOVE %s FROM (%d, %d) TO (%d, %d)\n", GetHans(piece->type), piece->x, piece->y, x, y);
    piece->x = x;
    piece->y = y;
}

