#include "chess.h"

int screenX = 1024;
int screenY = 1024;

int main() 
{
  RenderWindow window(VideoMode(screenX, screenY), "Chess", Style::Default);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  const int cell_size = 128;
  const int height = 8;
  const int width = 8;
  int cellPositionX;
  int cellPositionY;
  bool selectedPiece = false;
  int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  bool playerTurn = true; // true intially for the white's turn first
  int currentScreen = LOADING;
  int timer = 180; // for 3 sec delay of the loading screen
  int mouseX;
  int mouseY;
  bool buttonPlay = false;
  bool isCheckWhite = false;
  bool isCheckBlack = false;
  int selectedTheme;
  bool resultSaved = false; // ensures we save only once per game

  // ---- Player names ----
  string player1 = "WHITE";
  string player2 = "BLACK";

  // Show previously saved results on startup
  showResults();


  //################################## SHAPES MAKING ##########################################################
  RectangleShape block(Vector2f(cell_size, cell_size));
  RectangleShape selectPiece(Vector2f(cell_size, cell_size));
  RectangleShape piecePositions(Vector2f(cell_size, cell_size));
  CircleShape check(4.f);
  check.setFillColor(Color::Red);

  selectPiece.setFillColor(Color(255, 255, 255, 128));

  //################################## SPRITE AND TEXTURE VARIABLES ###########################################
  Texture PawnTex, KnightTex, BishopTex, RookTex, QueenTex, KingTex;
  Sprite Pawn, Knight, Bishop, Rook, Queen, King;

  Texture turnTex;
  Sprite turn;

  Texture Loading;
  Sprite load_screen;

  Texture Main;
  Sprite main_screen;

  Texture ThemeTex;
  Sprite Theme;

  Texture GameOverTexWhite;
  Sprite GameOverWhite;

  Texture GameOverTexBlack;
  Sprite GameOverBlack;
  
  // Texture borderTex;
  // Sprite border;

  SoundBuffer buffer;
  Sound sound;

  SoundBuffer button;
  Sound Button;

  //################################## SPRITE AND TEXTURE LOADING ###########################################
  PawnTex.loadFromFile("assets/pawn.png");
  Pawn.setTexture(PawnTex);

  KnightTex.loadFromFile("assets/knight.png");
  Knight.setTexture(KnightTex);

  BishopTex.loadFromFile("assets/bishop.png");
  Bishop.setTexture(BishopTex);

  RookTex.loadFromFile("assets/rook.png");
  Rook.setTexture(RookTex);

  QueenTex.loadFromFile("assets/queen.png");
  Queen.setTexture(QueenTex);

  KingTex.loadFromFile("assets/king.png");
  King.setTexture(KingTex);

  turnTex.loadFromFile("assets/turn.png");
  turn.setTexture(turnTex);

  Loading.loadFromFile("assets/loading.png");
  load_screen.setTexture(Loading);

  Main.loadFromFile("assets/main.jpeg");
  main_screen.setTexture(Main);

  ThemeTex.loadFromFile("assets/theme.jpeg");
  Theme.setTexture(ThemeTex);

  GameOverTexWhite.loadFromFile("assets/checkMateWhite.jpeg");
  GameOverWhite.setTexture(GameOverTexWhite);
  
  GameOverTexBlack.loadFromFile("assets/checkMateBlack.jpeg");
  GameOverBlack.setTexture(GameOverTexBlack);
  
  // borderTex.loadFromFile("assets/border.png");
  // border.setTexture(borderTex);
  // border.setScale(2.1f, 2.1f);
  // border.setPosition(-15.f, 0);

  buffer.loadFromFile("sounds/music.wav");
  sound.setBuffer(buffer);
  sound.setVolume(100.f);

  button.loadFromFile("sounds/button.wav");
  Button.setBuffer(button);
  Button.setVolume(100.f);

  Board board(height, width, cell_size, block, Pawn, Rook, Knight, Bishop, Queen, King);

  sound.setLoop(true);
  sound.play();

  while (window.isOpen()) {
    sf::Event evnt;
    while (window.pollEvent(evnt)) {
      if (evnt.type == sf::Event::Closed) {
        window.close();
      }

      if (evnt.type == sf::Event::MouseButtonPressed) {
        if (evnt.mouseButton.button == Mouse::Left) {

          mouseX = evnt.mouseButton.x;
          mouseY = evnt.mouseButton.y;

          if (currentScreen == MAIN) {
            if (mouseX >= 584 && mouseX <= 897 && mouseY >= 109 && mouseY <= 172) {
              Button.play();
              currentScreen = GAME;
            }

            if (mouseX >= 584 && mouseX <= 897 && mouseY >= 218 && mouseY <= 279) {
              Button.play();
              window.close();
            }

            if (mouseX >= 119 && mouseX <= 228 && mouseY >= 112 && mouseY <= 222) {
              Button.play();
              currentScreen = THEME;
            }
          }

          if (currentScreen == THEME) {
            if(mouseX >= 699 && mouseX <= 904 && mouseY >= 112 && mouseY <= 173)
            {
                Button.play();
                currentScreen = MAIN;
            }

            if(mouseX >= 113 && mouseX <= 333 && mouseY >= 727 && mouseY <= 775)
            {
                Button.play();
                //brown
                cout << "Brown\n";
                selectedTheme = BROWN;
            }

            if(mouseX >= 402 && mouseX <= 623 && mouseY >= 727 && mouseY <= 775)
            {
                Button.play();
                //green
                cout << "Green\n";
                selectedTheme = GREEN;
            }

            if(mouseX >= 693 && mouseX <= 915 && mouseY >= 727 && mouseY <= 775)
            {
                Button.play();
                //blue
                cout << "Blue\n";
                selectedTheme = BLUE;
            }

          }

          if (currentScreen == CHECKMATEWHITE || currentScreen == CHECKMATEBLACK)
          {
            if(mouseX >= 376 && mouseX <= 652 && mouseY >= 713 && mouseY <= 775)
            {
              Button.play();

              board.reset();
              playerTurn = true;
              selectedPiece = false;
              isCheckWhite = false;
              isCheckBlack = false;

              currentScreen = GAME;
            }
          }

          cout << "X: " << evnt.mouseButton.x << endl; // for debugging only
          cout << "Y: " << evnt.mouseButton.y << endl;

          if (currentScreen == GAME) {

            cellPositionX = mouseX / cell_size;
            cellPositionY = mouseY / cell_size;


            if (selectedPiece == false) {
              if (playerTurn == true) {
                if (board.getPiece(cellPositionY, cellPositionX) != nullptr && board.getPiece(cellPositionY, cellPositionX)->getColor() == WHITE) {
                  selectedPiece = true;
                  x1 = cellPositionY;
                  y1 = cellPositionX;
                }
              }

              else if (playerTurn == false) {
                if (board.getPiece(cellPositionY, cellPositionX) != nullptr && board.getPiece(cellPositionY, cellPositionX)->getColor() == BLACK) {
                  selectedPiece = true;
                  x1 = cellPositionY;
                  y1 = cellPositionX;
                }
              }
            }

            else if (selectedPiece == true) {
              x2 = cellPositionY;
              y2 = cellPositionX;
              if (board.isValidMove(x1, y1, x2, y2)) {

                isCheckWhite = false;
                isCheckBlack = false;
                board.move(x1, y1, x2, y2);

                // if(playerTurn == true)
                // {
                //   board.isCheck(BLACK);
                //   board.isCheckMate(BLACK);
                // }
                // else
                // {
                //   board.isCheck(WHITE);
                //   board.isCheckMate(WHITE);
                // }

                if(playerTurn == true)
                {
                    if(board.isCheck(BLACK))
                    {
                      isCheckBlack = true;
                      cout << "Black King is in Check!" << endl;
                    }
                      
                     if(board.isCheckMate(BLACK))
                     {
                        currentScreen = CHECKMATEBLACK;
                        cout << "Checkmate! White Wins!" << endl;

                        // ---- FILE HANDLING: announce and save result ----
                        if(!resultSaved)
                        {
                          announceWinner(player1 + " (White)");
                          saveResult(player1, player2, player1 + " (White)", board.getMoveCount());
                          resultSaved = true;
                        }

                      }
                }
                else
                {
                    if(board.isCheck(WHITE))
                    {
                      isCheckWhite = true;
                      cout << "White King is in Check!" << endl;
                    }
                    
                    if(board.isCheckMate(WHITE))
                    {
                       currentScreen = CHECKMATEWHITE;
                       cout << "Checkmate! Black Wins!" << endl;

                      // ---- FILE HANDLING: announce and save result ----
                      if(!resultSaved)
                      {
                        announceWinner(player2 + " (Black)");
                        saveResult(player1, player2, player2 + " (Black)", board.getMoveCount());
                        resultSaved = true;
                      }
                    }
                }

                playerTurn = !playerTurn;
              }
              selectedPiece = false;
            }
          }
        }
      }
    }

    //###################################################### SCREEN MANAGING ##################################################
    window.clear();

    if (currentScreen == THEME) {
        window.draw(Theme);
    }

    if (currentScreen == LOADING) {
      if (timer > 0) {
        window.draw(load_screen);
        timer--;
      }

      if (timer <= 0) {
        currentScreen = MAIN;
      }
    }

    else if (currentScreen == MAIN) {
      window.draw(main_screen);
    } 
    else if (currentScreen == GAME) {
      board.displayBoard(window, selectedTheme);
      if (playerTurn) {
        turn.setColor(Color::White);
        turn.setScale(0.1f, 0.1f);
        turn.setPosition(-10.f, 990.f);
        window.draw(turn);

      } 
      else {
        turn.setColor(Color::Black);
        turn.setScale(0.1f, 0.1f);
        turn.setPosition(-10.f, 20.f);
        window.draw(turn);
      }

      if(isCheckBlack)
      {
        check.setPosition(10.f, 10.f);
        window.draw(check);
      }

      if(isCheckWhite)
      {
        check.setPosition(10.f, screenY - 20.f);
        window.draw(check);
      }

      
      // window.draw(border);
    }

    else if(currentScreen == CHECKMATEBLACK)
    {
      window.draw(GameOverWhite);
    }

    else if(currentScreen == CHECKMATEWHITE)
    {
      window.draw(GameOverBlack);
    }
    window.display();
  }

  return 0;
}
