#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;
using namespace sf;

enum PieceColor{WHITE, BLACK};
enum PieceType{PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};
enum Screen{LOADING, MAIN, THEME, GAME, CHECKMATEBLACK, CHECKMATEWHITE};
enum Theme{BROWN, GREEN, BLUE};

// ===================== FILE HANDLING FUNCTIONS =====================

void saveResult(string player1, string player2, string winner, int moves);
void showResults();
void announceWinner(string winner);

class Piece
{
    private:
        PieceColor color;
        PieceType type;
        int x;
        int y;
        bool IsMoved;

    public:
        Piece(PieceColor color, PieceType type, int x, int y);
        void setX(int x);
        void setY(int y);
        void setIsMoved(bool moved);
        PieceColor getColor();
        PieceType getType();
        int getX();
        int getY();
        bool getIsMoved();
        // function for the check's logic of inherited classes // implement later

        virtual bool isValidMove(int X, int Y, Piece* grid[8][8]) = 0;
        virtual ~Piece();
};

class Pawn : public Piece
{
    private:
        Texture promotionTex;
        Sprite promotion;

    public:
        Pawn(PieceColor color, int x, int y);
        // void promotionScreen(Sprite promotion, RenderWindow& window);
        bool isValidMove(int X, int Y, Piece* grid[8][8]) override;
};

class Rook : public Piece
{
    private:

    public:
        Rook(PieceColor color, int x, int y);
        bool isValidMove(int X, int Y, Piece* grid[8][8]) override;
};

class Bishop : public Piece
{
    private:

    public:
        Bishop(PieceColor color, int x, int y);
        bool isValidMove(int X, int Y, Piece* grid[8][8]) override;
};

class Knight : public Piece
{
    private:

    public:
        Knight(PieceColor color, int x, int y);
        bool isValidMove(int X, int Y, Piece* grid[8][8]) override;
};

class Queen : public Piece
{
    private:

    public:
        Queen(PieceColor color, int x, int y);
        bool isValidMove(int X, int Y, Piece* grid[8][8]) override;
};

class King : public Piece
{
    private:

    public:
        King(PieceColor color, int x, int y);
        bool isValidMove(int X, int Y, Piece* grid[8][8]) override;
};

class Board
{
    private:
        RectangleShape block;
        const int height;
        const int width;
        const int cell_size;
        Sprite pawn;
        Sprite rook;
        Sprite knight;
        Sprite bishop;
        Sprite queen;
        Sprite king;
        Piece* grid[8][8];
        Piece* save;
        int moveCount;     
        
    public:
        Board(const int hght, const int wid, const int cell, RectangleShape blockSprite, Sprite pawn, Sprite rook,
                Sprite knight, Sprite bishop, Sprite queen, Sprite king);
        void fillPieces();
        void move(int X1, int Y1, int X2, int Y2);
        int getMoveCount();
        Piece* getPiece(int x, int y);
        bool isValidMove(int x1, int y1, int x2, int y2);
        bool isCheck(PieceColor color);
        bool isCheckMate(PieceColor color);
        void reset();
        void displayBoard(RenderWindow& window, int currentTheme);
};
