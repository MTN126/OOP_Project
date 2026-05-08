#include "chess.h"

// ===================== FILE HANDLING FUNCTIONS =====================
void saveResult(string player1, string player2, string winner, int moves)
{
    ofstream file("chess_results.txt", ios::app);


    if (!file.is_open())
    {
        cout << "Error: Could not open file to save result!\n";
        return;
    }

    file << "==========================\n";
    file << "Player 1 : " << player1 << "\n";
    file << "Player 2 : " << player2 << "\n";
    file << "Moves    : " << moves << "\n";
    file << "WINNER   : " << winner << "\n";
    file << "==========================\n\n";

    file.close();
    cout << "\nResult saved to chess_results.txt!\n";
}

void showResults()
{
    ifstream file("chess_results.txt");
    string line;

    if (!file.is_open())
    {
        cout << "No results found!\n";
        return;
    }

    cout << "\n--- Saved Results ---\n";
    while (getline(file, line))
    {
        cout << line << "\n";
    }

    file.close();
}

void announceWinner(string winner)
{
    cout << "\n==========================\n";
    cout << "   GAME OVER!\n";
    cout << "   WINNER: " << winner << "\n";
    cout << "==========================\n";
}

//################################ PIECES CLASS ##################################################################
Piece::Piece(PieceColor color, PieceType type, int x, int y)
{
    this->color = color;
    this->type = type;
    this->x = x;
    this->y = y;
}

void Piece::setX(int x)
{
    this->x = x;
}

void Piece::setY(int y)
{
    this->y = y;
}

void Piece::setIsMoved(bool moved)
{
    IsMoved = moved;
}

PieceColor Piece::getColor()
{
    return color;
}

PieceType Piece::getType()
{
    return type;
}

int Piece::getX()
{
    return x;
}

int Piece::getY()
{
    return y;
}

bool Piece::getIsMoved()
{
    return IsMoved;
}

Piece::~Piece() {}

//################################ PAWN CLASS ##################################################################
Pawn::Pawn(PieceColor color, int x, int y) : Piece(color, PAWN, x, y) {}
bool Pawn::isValidMove(int X, int Y, Piece* grid[8][8])
{

    if (X < 0 || X >= 8 || Y < 0 || Y >= 8)
        return false;

    int direction = (getColor() == WHITE) ? -1 : 1; // WHITE moves up (decreasing row)
    int startRow = (getColor() == WHITE) ? 6 : 1;

    int diffX = X - getX();
    int diffY = abs(Y - getY());


    // One step forward into empty square
    if (diffX == direction && diffY == 0 && grid[X][Y] == nullptr)
        return true;

    // Two steps forward from starting row into empty squares
    if (getX() == startRow && diffX == 2 * direction && diffY == 0
        && grid[getX() + direction][getY()] == nullptr  // path must be clear
        && grid[X][Y] == nullptr)
        return true;

    // Diagonal capture
    if (diffX == direction && diffY == 1
        && grid[X][Y] != nullptr
        && grid[X][Y]->getColor() != getColor())
        return true;

    return false;
}

//################################ BISHOP CLASS ##################################################################
Bishop::Bishop(PieceColor color, int x, int y) : Piece(color, BISHOP, x, y) {}
bool Bishop::isValidMove(int X, int Y, Piece* grid[8][8])
{
    if (X < 0 || X >= 8 || Y < 0 || Y >= 8)
        return false;

    if (grid[X][Y] != nullptr && grid[X][Y]->getColor() == getColor())
        return false;

    int diffX = abs(X - getX());
    int diffY = abs(Y - getY());

    // Must move diagonally (equal row and column distance)
    if (diffX != diffY || diffX == 0)
        return false;

    // Check path is clear
    int stepX = (X > getX()) ? 1 : -1;
    int stepY = (Y > getY()) ? 1 : -1;
    int curX = getX() + stepX;
    int curY = getY() + stepY;

    while (curX != X && curY != Y)
    {
        if (grid[curX][curY] != nullptr) return false;
        curX += stepX;
        curY += stepY;
    }

    return true;
}

//################################ ROOk CLASS ##################################################################
Rook::Rook(PieceColor color, int x, int y) : Piece(color, ROOK, x, y) {}
bool Rook::isValidMove(int X, int Y, Piece* grid[8][8])
{
    if (X < 0 || X >= 8 || Y < 0 || Y >= 8)
        return false;

    if (grid[X][Y] != nullptr && grid[X][Y]->getColor() == getColor())
        return false;

    // Must stay on same row OR same column
    if (X != getX() && Y != getY())
        return false;

    // Check path is clear
    if (X == getX()) // moving along columns (horizontal)
    {
        int minY = min(Y, getY());
        int maxY = max(Y, getY());
        for (int col = minY + 1; col < maxY; col++)
            if (grid[X][col] != nullptr) return false;
    }
    else             // moving along rows (vertical)
    {
        int minX = min(X, getX());
        int maxX = max(X, getX());
        for (int row = minX + 1; row < maxX; row++)
            if (grid[row][Y] != nullptr) return false;
    }

    return true;
}

//################################ KNIGHT CLASS ##################################################################
Knight::Knight(PieceColor color, int x, int y) : Piece(color, KNIGHT, x, y) {}
bool Knight::isValidMove(int X, int Y, Piece* grid[8][8]) 
{
    int currX = abs(X - getX());
    int currY = abs(Y - getY());

    if(X < 0 || X >= 8 || Y < 0 || Y >= 8) 
    {
        return false;
    }
 
    if(grid[X][Y] != nullptr && grid[X][Y]->getColor() == getColor())
    {
        return false;
    }
    
    if(currX == 2 && currY == 1 || currX == 1 && currY == 2)  //(2, 1) means differnce is X = 2 and Y = 1 in up and down position 
    {                                                         //(1, 2) means differnce is X = 1 and Y = 2 in left and right position
         return true;
    }

    return false;
}

//################################ QUEEN CLASS ##################################################################
Queen::Queen(PieceColor color, int x, int y) : Piece(color, QUEEN, x, y) {}
bool Queen::isValidMove(int X, int Y, Piece* grid[8][8])
{
    int currX = getX();
    int currY = getY();

    if(X < 0 || X >= 8 || Y < 0 || Y >= 8) 
    {
        return false;
    }

    if(grid[X][Y] != nullptr && grid[X][Y]->getColor() == getColor()) 
    {
        return false;
    }

    // if(currX == X)
    // {

    // }
    // else if(currY == Y)  
    // {

    // }
    // else if(abs(X - currX) == abs(Y - currY))
    // {
        
    // }
    // else
    // {
    //     return false;
    // }

    // up and down
    if(currY == Y) //Queen's destiantion should be in the same column
    {
        if(X < currX) //destination is above the current position
        {    
            for (int i = currX - 1; i > X ; i--) // getX() as x changes from top to bottom  // - 1 for the position below the queen box;
            {
                if(grid[i][currY] != nullptr) 
                {
                    return false;
                    // continue;
                }
                // else
                // {
                // }

                // if(grid[i][currY]->getColor() == getColor())
                // {
                //     return false;
                // }
                // else
                // {
                //     return true; 
                // }
            }
            return true;
        }  
        
        if(X > currX) //destination is below the current position
        {
            for (int i = currX + 1; i < X; i++) // + 1 for the position above the queen box;
            {
                if(grid[i][currY] != nullptr) 
                {
                    return false;
                    // continue;
                }
                // // else
                // {
                // }

                // if(grid[i][currY]->getColor() == getColor())
                // {
                //     return false;
                // }
                // else
                // {
                //     return true;
                // }
            }
            return true;
        }
    }
    
    // left and right
    else if(currX == X) //Queen's destiantion should be in the same row
    {
        if(Y < currY) //destination is left to the current position
        {
                for (int i = currY - 1; i > Y; i--) // getY() as y changes from left to right  // - 1 for the position left the queen box;
                {
                    if(grid[currX][i] != nullptr)
                    {
                        return false;
                        // continue;
                    }
                    // else
                    // {
                    // }
                    
                // if(grid[currX][i]->getColor() == getColor())
                // {
                //     return false;
                // }
                // else
                // {
                //     return true;
                // }
            }
            return true;
        }

        if(Y > currY) //destination is right to the current position
        {
            for (int i = currY + 1; i < Y; i++) // + 1 for the position right the queen box;
            {
                if(grid[currX][i] != nullptr)
                {
                    return false;
                }
                // else
                // {
                //     continue;
                // }
                
                // if(grid[currX][i]->getColor() == getColor())
                // {
                //     return false;
                // }
                // else
                // {
                //     return true;
                // }
            }
            return true;
        }
    }

    else if(abs(X - currX) == abs(Y - currY))
    {

        //diagonal moving top left and top right 
        if(X < currX && Y > currY)
    {
        for(int i = currX - 1, j = currY + 1; (i > X && j < Y); i--, j++)
        {
            if(grid[i][j] != nullptr)
            {
                return false;
                // continue;
            }
            // else
            // {
            // }
            
            // if(grid[i][j]->getColor() == getColor())
            // {
            //     return false;
            // }
            // else
            // {
                //     return true;
                // }
            }
            return true;
        }
        
    if(X < currX && Y < currY)
    {
        for (int i = currX - 1, j = currY - 1; (i > X && j > Y) ; i--, j--)
        {
            if(grid[i][j] != nullptr)
            {
                return false;
                // continue;
            }
            // else
            // {
            // }
            
            // if(grid[i][j]->getColor() == getColor())
            // {
            //     return false;
            // }
            // else
            // {
                //     return true;
                // }
            }
            return true;
        }
        
        //diagonal moving bottom left and top right 
        if(X > currX && Y > currY)
        {
            for (int i = currX + 1, j = currY + 1; (i < X && j < Y); i++, j++)
        {
            if(grid[i][j] != nullptr)
            {
                return false;
                // continue;
            }
            // else
            // {
            // }
            
            // if(grid[i][j]->getColor() == getColor())
            // {
            //     return false;
            // }
            // else
            // {
                //     return true;
                // }
            }
            return true;
        }
        
        if(X > currX && Y < currY)
        {
            for (int i = currX + 1, j = currY - 1; (i < X && j > Y); i++, j--)
            {
                if(grid[i][j] != nullptr)
            {
                return false;
                // continue;
            }
            // else
            // {
            // }
            
            // if(grid[i][j]->getColor() == getColor())
            // {
            //     return false;
            // }
            // else
            // {
                //     return true;
                // }
            }
            return true;
        }
    }
    else 
    {
        return false;
    }
}
    
    //################################ KING CLASS ##################################################################
    King::King(PieceColor color, int x, int y) : Piece(color, KING, x, y) {}
    bool King::isValidMove(int X, int Y, Piece* grid[8][8]) 
    {
        int currX = abs(X - getX());
        int currY = abs(Y - getY());

    if(X < 0 || X >= 8 || Y < 0 || Y >= 8) 
    {
        return false;
    }
 
    if(grid[X][Y] != nullptr && grid[X][Y]->getColor() == getColor())
    {
        return false;
    }
    
    if(currX == 1 && currY == 0 || currX == 0 && currY == 1 || currX == 1 && currY == 1) //(1, 0) means differnce is X = 1 and Y = 0 in up and down position
    {                                                                                   //(0, 1) means differnce is X = 0 and Y = 1 in left and right position
         return true;                                                                  //(1, 1) means differnce is X = 1 and Y = 1 in diagonal position
    }

    return false;
}

//############################### BOARD CLASS ####################################################################
Board::Board(const int hght, const int wid, const int cell, RectangleShape blockSprite, Sprite pawn, Sprite rook,
                Sprite knight, Sprite bishop, Sprite queen, Sprite king) : height(hght), width(wid), cell_size(cell)
{
    block = blockSprite;
    this->pawn = pawn;
    this->rook = rook;
    this->bishop = bishop;
    this->knight = knight;
    this->queen = queen;
    this->king = king;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grid[i][j] = nullptr;
        }
    }

    fillPieces();
}

void Board::fillPieces()
{
    //black
    grid[0][0] = new Rook(BLACK, 0, 0);
    grid[0][7] = new Rook(BLACK, 0, 7);
    grid[0][1] = new Knight(BLACK, 0, 1);
    grid[0][6] = new Knight(BLACK, 0, 6);
    grid[0][2] = new Bishop(BLACK, 0, 2);
    grid[0][5] = new Bishop(BLACK, 0, 5);
    grid[0][3] = new Queen(BLACK, 0, 3);
    grid[0][4] = new King(BLACK, 0, 4);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(i == 1)
            {
                grid[i][j] = new Pawn(BLACK, i, j);
            }
        }   
    }
    
    //white
    grid[7][0] = new Rook(WHITE, 7, 0);
    grid[7][7] = new Rook(WHITE, 7, 7);
    grid[7][1] = new Knight(WHITE, 7, 1);
    grid[7][6] = new Knight(WHITE, 7, 6);
    grid[7][2] = new Bishop(WHITE, 7, 2); 
    grid[7][5] = new Bishop(WHITE, 7, 5); 
    grid[7][3] = new Queen(WHITE, 7, 3);
    grid[7][4] = new King(WHITE, 7, 4);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(i == 6)
            {
                grid[i][j] = new Pawn(WHITE, i, j);
            }
        }
    }

}

void Board::move(int X1, int Y1, int X2, int Y2)  // (X2,Y2) ===> MEANS TO(NEW POSITION) AND (X1,Y1) ===> MEANS FROM(CURRENT POSITION)
{
    grid[X2][Y2] = grid[X1][Y1];
    grid[X1][Y1] = nullptr;
    grid[X2][Y2]->setX(X2);
    grid[X2][Y2]->setY(Y2);
    moveCount++;
}

int Board::getMoveCount()
{
    return moveCount;
}

Piece* Board::getPiece(int x, int y)
{
    return grid[x][y];
}

bool Board::isCheck(PieceColor color)
{
    int KingX = 0;
    int KingY = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(grid[i][j] != nullptr && grid[i][j]->getColor() == color && grid[i][j]->getType() == KING)
            {
                //if king's position is found for the indiviual color storing it's current position on the board in sepearte variables
                KingX = i;
                KingY = j;
                break;
            }
        }
    }    

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(grid[i][j] == nullptr)
            {
                continue;
            }

            if(grid[i][j]->getColor() == color)
            {
                continue;
            }

            if(grid[i][j]->isValidMove(KingX, KingY, grid))
            {
                return true;
            }
        }
    }

    return false;
}

bool Board::isCheckMate(PieceColor color)
{
    if(!isCheck(color))
    {
        return false;
    }

    else
    {
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if(grid[i][j] == nullptr)
                    {
                        continue;
                    }

                    if(grid[i][j]->getColor() != color)
                    {
                        continue;
                    }

                        for (int x = 0; x < height; x++)
                        {
                            for (int y = 0; y < width; y++)
                            {
                                if(!grid[i][j]->isValidMove(x, y, grid))
                                {
                                    continue;
                                }

                                save = grid[x][y];
                                grid[x][y] = grid[i][j];
                                grid[i][j] = nullptr;
                                grid[x][y]->setX(x);
                                grid[x][y]->setY(y);

                                if(!isCheck(color))
                                {
                                    grid[i][j] = grid[x][y];
                                    grid[x][y] = save;
                                    grid[i][j]->setX(i);
                                    grid[i][j]->setY(j);

                                    return false;
                                }

                                    grid[i][j] = grid[x][y];
                                    grid[x][y] = save;
                                    grid[i][j]->setX(i);
                                    grid[i][j]->setY(j);

                            }
                        }   
                }
            }
    }

    return true;
}

void Board::reset()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j] != nullptr)
            {
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
    }

    fillPieces();
}

bool Board::isValidMove(int x1, int y1, int x2, int y2)
{
    return grid[x1][y1]->isValidMove(x2, y2, grid);
}

void Board::displayBoard(RenderWindow& window, int currentTheme)
{
    //displaying the board
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
                if ((i + j) % 2 == 0)
                {
                    block.setFillColor(Color(250, 243, 221)); // biege color
                    block.setPosition(j * cell_size, i * cell_size);
                    window.draw(block);
                }
                else 
                {
                  if(currentTheme == BROWN)
                  {
                    block.setFillColor(Color(107, 79, 58)); //brown color
                  }

                  if (currentTheme == GREEN)
                  {
                    block.setFillColor(Color(139, 189, 120)); //green color
                  }

                  if (currentTheme == BLUE)
                  {
                    block.setFillColor(Color(152, 182, 212)); //blue color
                  }

                    block.setPosition(j * cell_size, i * cell_size);
                    window.draw(block);
                }
        }
    }

    // displaying the pieces
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //drawing pawns
            if(grid[i][j] != nullptr && grid[i][j]->getType() == PAWN)
            {
                if(grid[i][j]->getColor() == WHITE)
                {
                    pawn.setColor(Color(220, 213, 191));
                }
                else
                {
                  if(currentTheme == BROWN)
                  {
                    pawn.setColor(Color(87, 59, 38)); //brown color
                  }

                  if (currentTheme == GREEN)
                  {
                    pawn.setColor(Color(89, 139, 70)); //green color
                  }

                  if (currentTheme == BLUE)
                  {
                    pawn.setColor(Color(70, 100, 130)); //blue color
                  }
                }
                pawn.setPosition(j * cell_size + 20, i * cell_size - 50);
                pawn.setScale(1.5f, 1.5f);
                window.draw(pawn);
            }

            //drawing rooks
            if(grid[i][j] != nullptr && grid[i][j]->getType() == ROOK)
            {
                if(grid[i][j]->getColor() == WHITE)
                {
                    rook.setColor(Color(220, 213, 191));
                }
                else
                {
                  if(currentTheme == BROWN)
                  {
                    rook.setColor(Color(87, 59, 38)); //brown color
                  }

                  if (currentTheme == GREEN)
                  {
                    rook.setColor(Color(89, 139, 70)); //green color
                  }

                  if (currentTheme == BLUE)
                  {
                    rook.setColor(Color(70, 100, 130)); //blue color
                  }
                }

                rook.setPosition(j * cell_size + 20, i * cell_size - 50);
                rook.setScale(1.5f, 1.5f);
                window.draw(rook);
            }

             //drawing knights
            if(grid[i][j] != nullptr && grid[i][j]->getType() == KNIGHT)
            {
                if(grid[i][j]->getColor() == WHITE)
                {
                    knight.setColor(Color(220, 213, 191));
                }
                else
                {
                  if(currentTheme == BROWN)
                  {
                    knight.setColor(Color(87, 59, 38)); //brown color
                  }

                  if (currentTheme == GREEN)
                  {
                    knight.setColor(Color(89, 139, 70)); //green color
                  }

                  if (currentTheme == BLUE)
                  {
                    knight.setColor(Color(70, 100, 130)); //blue color
                  }
                }

                knight.setPosition(j * cell_size + 20, i * cell_size - 36);
                knight.setScale(1.4f, 1.4f);
                window.draw(knight);
            }

            //drawing bishops
            if(grid[i][j] != nullptr && grid[i][j]->getType() == BISHOP)
            {
                if(grid[i][j]->getColor() == WHITE)
                {
                    bishop.setColor(Color(220, 213, 191));
                }
                else
                {
                  if(currentTheme == BROWN)
                  {
                    bishop.setColor(Color(87, 59, 38)); //brown color
                  }

                  if (currentTheme == GREEN)
                  {
                    bishop.setColor(Color(89, 139, 70)); //green color
                  }

                  if (currentTheme == BLUE)
                  {
                    bishop.setColor(Color(70, 100, 130)); //blue color
                  }
                }

                bishop.setPosition(j * cell_size + 24, i * cell_size - 2);
                bishop.setScale(1.3f, 1.1f);
                window.draw(bishop);
            }
            
            //drawing queens
            if(grid[i][j] != nullptr && grid[i][j]->getType() == QUEEN)
            {
                if(grid[i][j]->getColor() == WHITE)
                {
                    queen.setColor(Color(220, 213, 191));
                }
                else
                {
                  if(currentTheme == BROWN)
                  {
                    queen.setColor(Color(87, 59, 38)); //brown color
                  }

                  if (currentTheme == GREEN)
                  {
                    queen.setColor(Color(89, 139, 70)); //green color
                  }

                  if (currentTheme == BLUE)
                  {
                    queen.setColor(Color(70, 100, 130)); //blue color
                  }
                }

                queen.setPosition(j * cell_size + 24, i * cell_size - 2);
                queen.setScale(1.3f, 1.1f);
                window.draw(queen);
            }

             //drawing kings
            if(grid[i][j] != nullptr && grid[i][j]->getType() == KING)
            {
                if(grid[i][j]->getColor() == WHITE)
                {
                    king.setColor(Color(220, 213, 191));
                }
                else
                {
                  if(currentTheme == BROWN)
                  {
                    king.setColor(Color(87, 59, 38)); //brown color
                  }

                  if (currentTheme == GREEN)
                  {
                    king.setColor(Color(89, 139, 70)); //green color
                  }

                  if (currentTheme == BLUE)
                  {
                    king.setColor(Color(70, 100, 130)); //blue color
                  }
                }

                king.setPosition(j * cell_size + 24, i * cell_size - 2);
                king.setScale(1.3f, 1.1f);
                window.draw(king);
            }
        }
    }
}
