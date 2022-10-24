#ifndef SHOGILIB
#define SHOGILIB
#endif

#include <stdio.h>

#define FALSE 0
#define TRUE 1

#define PIECE_TYPE_COUNT 8

#define MIN(a, b) (a < b ? (a) : (b))
#define MAX(a, b) (a > b ? (a) : (b))


typedef enum { King, Rook, Bishop, Gold, Silver, Knight, Lance, Pawn } PieceType;

typedef struct {
    PieceType type;
    int n;
    int ne;
    int e;
    int se;
    int s;
    int sw;
    int w;
    int nw;
    int allowMulti;
    int allowJump;
} PieceBehavior;

typedef struct {
    PieceType type;
    int player;
    int x;
    int y;
    int isEaten;
} Piece;


char* GetHans(PieceType type);

void DrawBoard(Piece* const pieces, int highlightX, int highlightY);

void AssignPiece(Piece* piece, int player, PieceType type, int x, int y, int isEaten);

void InitializeBehaviors(PieceBehavior* pieceBehaviors);

void InitializeBoard(Piece* const pieces);

Piece* GetPieceAtPosition(Piece* pieces, int x, int y);

Piece* GetEatenPieceAtPosition(Piece* pieces, int x, int y);

PieceBehavior* GetPieceBehavior(PieceBehavior* pieceBehaviors, PieceType type);

int CheckMoveRule(Piece* pieces, PieceBehavior* pieceBehaviors, PieceType type, int oldX, int oldY, int newX, int newY, int currentPlayer);

void MovePiece(Piece* piece, int x, int y);
