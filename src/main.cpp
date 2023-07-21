#include "mylib.h"
#include "raylib.h"
#include <iostream>

int main()
{
	test1();
	test2();

    // Grid variables
	const int screenWidth = 460;
    const int screenHeight = 520;
    const int Grid_MarginTop = 80;
    const int Grid_MarginLeft = 20;
    const int Grid_MarginRight = 20;
    const int Grid_MarginBottom = 20;
    const int Grid_Gap = 140;
    const int Grid_FirstColumnX = Grid_MarginLeft + Grid_Gap;
    const int Grid_FirstColumnY = Grid_MarginTop;
    const int Grid_FirstRowX = Grid_MarginLeft;
    const int Grid_FirstRowY = Grid_MarginTop + Grid_Gap;
    const int Grid_Thickness = 10;
    const float PaddingLeft = 150.0F;
    const float PaddingTop = 140.0F;
    char CurrentPlayer = 'O', NextPlayer = 'O';
    bool GameOver = false;
    int ScorePlayer1 = 0;
    int ScorePlayer2 = 0;

    enum VictoryType
    {
        None = 0,
        CrossRightToLeft,
        CrossLeftToRight,
        CrossFirstVertical,
        CrossSecondVertical,
        CrossThirdVertical,
        CrossFirstHorizontal,
        CrossSecondHorizontal,
        CrossThirdHorizontal
    } WinningBy = VictoryType::None;

    BoundingBox MousePointer;

    struct Cell
    {
        BoundingBox Bounds;
        char Fill;
    };

    Cell board[3][3] = 
    {
        { // row 0
            { // col 0
                {{Grid_MarginLeft,Grid_MarginTop, 0},{Grid_MarginLeft + Grid_Gap, Grid_MarginTop + Grid_Gap, 0}}, ' '
            },
            { // col 1
                {{Grid_MarginLeft + Grid_Gap, Grid_MarginTop, 0},{Grid_MarginLeft + Grid_Gap * 2, Grid_MarginTop + Grid_Gap, 0}}, ' '
            },
            { // col 2
                {{Grid_MarginLeft + Grid_Gap * 2, Grid_MarginTop, 0},{Grid_MarginLeft + Grid_Gap * 3, Grid_MarginTop + Grid_Gap, 0}}, ' '
            }
        },
        { // row 1
            { // col 0
                {{Grid_MarginLeft,Grid_MarginTop + Grid_Gap, 0},{Grid_MarginLeft + Grid_Gap, Grid_MarginTop + Grid_Gap * 2, 0}}, ' '
            },
            { // col 1
                {{Grid_MarginLeft + Grid_Gap, Grid_MarginTop + Grid_Gap, 0},{Grid_MarginLeft + Grid_Gap * 2, Grid_MarginTop + Grid_Gap * 2, 0}}, ' '
            },
            { // col 2
                {{Grid_MarginLeft + Grid_Gap * 2, Grid_MarginTop + Grid_Gap, 0},{Grid_MarginLeft + Grid_Gap * 3, Grid_MarginTop + Grid_Gap * 2, 0}}, ' '
            }
        },
        { // row 3
            { // col 0
                {{Grid_MarginLeft,Grid_MarginTop + Grid_Gap * 2,0},{Grid_MarginLeft + Grid_Gap, Grid_MarginTop + Grid_Gap * 3, 0}}, ' '
            },
            { // col 1
                {{Grid_MarginLeft + Grid_Gap, Grid_MarginTop + Grid_Gap * 2,0},{Grid_MarginLeft + Grid_Gap * 2, Grid_MarginTop + Grid_Gap * 3, 0}}, ' '
            },
            { // col 2
                {{Grid_MarginLeft + Grid_Gap * 2, Grid_MarginTop + Grid_Gap * 2,0},{Grid_MarginLeft + Grid_Gap * 3, Grid_MarginTop + Grid_Gap * 3, 0}}, ' '
            }
        }
    };

    InitWindow(screenWidth, screenHeight, "TIC TAC TOE");

    SetTargetFPS(60);

    while (!WindowShouldClose())   
    {
        if (!GameOver)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                // Capture mouse position and define an invible bounding box for collision test
                MousePointer.min.x = static_cast<float>(GetMouseX()-1);
                MousePointer.min.y = static_cast<float>(GetMouseY()-1);
                MousePointer.min.z = 0;
                MousePointer.max.x = static_cast<float>(GetMouseX()+1);
                MousePointer.max.y = static_cast<float>(GetMouseY()+1);
                MousePointer.max.z = 0;

                // Identify which cell was clicked 
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (board[i][j].Fill == ' ' && CheckCollisionBoxes(board[i][j].Bounds, MousePointer))
                        {
                            CurrentPlayer = NextPlayer;

                            // Save current player's move
                            board[i][j].Fill = CurrentPlayer;

                            // Set next player turn
                            (CurrentPlayer == 'O') ? NextPlayer = 'X' : NextPlayer = 'O';
                            break;
                        }
                    }
                }

                // Check if there is a winner
                if (board[0][0].Fill != ' ')
                {
                    if (board[0][0].Fill == board[0][1].Fill && board[0][0].Fill == board[0][2].Fill) 
                    {
                        WinningBy = CrossFirstHorizontal;
                    }
                    if (board[0][0].Fill == board[1][0].Fill && board[0][0].Fill == board[2][0].Fill)
                    {
                        WinningBy = CrossFirstVertical;
                    }
                }
                if (board[1][1].Fill != ' ' )
                {
                    if (board[1][1].Fill == board[1][0].Fill && board[1][0].Fill == board[1][2].Fill)
                    {
                        WinningBy = CrossSecondHorizontal;
                    }
                    if (board[1][1].Fill == board[0][1].Fill && board[0][1].Fill == board[2][1].Fill)
                    {
                        WinningBy = CrossSecondVertical;
                    }
                    if (board[1][1].Fill == board[0][0].Fill && board[1][1].Fill == board[2][2].Fill)
                    {
                        WinningBy = CrossLeftToRight;
                    }
                    if (board[1][1].Fill == board[0][2].Fill && board[1][1].Fill == board[2][0].Fill)
                    {
                        WinningBy = CrossRightToLeft;
                    }
                }
                if (board[2][2].Fill != ' ')
                {   
                    if(board[2][2].Fill == board[2][1].Fill && board[2][2].Fill == board[2][0].Fill)
                    {
                        WinningBy = CrossThirdHorizontal;
                    }
                    if(board[2][2].Fill == board[1][2].Fill && board[2][2].Fill == board[0][2].Fill)
                    {
                        WinningBy = CrossThirdVertical;
                    }
                }
                GameOver = (WinningBy != None);
                                
                // Check if the match is a draw
                if (!GameOver)
                {
                    bool MovesLeft = false;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            if (board[i][j].Fill == ' ')
                            {
                                MovesLeft = true;
                                break;
                            }
                        }
                    }
                    GameOver = (!MovesLeft);
                }
            }
        }
        else
        {
            // Start another match
            if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (WinningBy != VictoryType::None)
                {
                    // Update players score
                    (CurrentPlayer == 'O') ? ScorePlayer1++ : ScorePlayer2++;

                }

                // Clean up the board and reset the match
                GameOver = false;
                CurrentPlayer = NextPlayer = 'O';
                WinningBy = VictoryType::None;
                for (int i = 0; i < 3 ; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        board[i][j].Fill = ' ';
                    }
                }
            }
        }

        // Draw board and moves
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            
            // Show players score
            DrawText(TextFormat("PLAYER 1: %02i vs PLAYER 2: %02i", ScorePlayer1, ScorePlayer2), 100, 20, 10, DARKGRAY);  

            // Draw grid
            DrawLineEx({Grid_FirstColumnX, Grid_FirstColumnY}, {Grid_MarginLeft + Grid_Gap, screenHeight - Grid_MarginBottom}, Grid_Thickness, BLACK);
            DrawLineEx({Grid_FirstColumnX + Grid_Gap, Grid_FirstColumnY}, {Grid_MarginLeft + Grid_Gap * 2, screenHeight - Grid_MarginBottom}, Grid_Thickness, BLACK);
            DrawLineEx({Grid_FirstRowX, Grid_FirstRowY}, {screenWidth - Grid_MarginRight, Grid_MarginTop + Grid_Gap}, Grid_Thickness, BLACK);
            DrawLineEx({Grid_FirstRowX, Grid_FirstRowY+ Grid_Gap}, {screenWidth - Grid_MarginRight, Grid_MarginTop + Grid_Gap * 2}, Grid_Thickness, BLACK);

            // Show mouse pointer position
            DrawText(TextFormat("X: %03i Y: %03i", GetMouseX(), GetMouseY()), 10, 20, 10, DARKGRAY);  

            // Draw player's moves
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (board[i][j].Fill == 'O')
                    {
                        DrawCircle(80 + PaddingLeft * j, 150 + PaddingTop * i, 50.0, RED);
                    }
                    if (board[i][j].Fill == 'X')
                    {
                        DrawLineEx({30 + PaddingLeft * j, 120 + PaddingTop * i},{130 + PaddingLeft * j, 190 + PaddingTop * i}, 25.0, BLUE);
                        DrawLineEx({30 + PaddingLeft * j, 190 + PaddingTop * i},{130 + PaddingLeft * j, 120 + PaddingTop * i}, 25.0, BLUE);
                    }
                }
            }

            // Draw cross line in case a winner is found
            switch (WinningBy)
            {
                case VictoryType::None:
                    break;
                case CrossFirstHorizontal:
                    DrawLineEx({20, 150},{440, 150}, 25.0, DARKGRAY);
                    break;
                case CrossFirstVertical:
                    DrawLineEx({80, 80},{80, 490}, 25.0, DARKGRAY);
                    break;
                case CrossSecondHorizontal:
                    DrawLineEx({20, 300},{440, 300}, 25.0, DARKGRAY);
                    break;
                case CrossSecondVertical:
                    DrawLineEx({230, 85},{230, 500}, 25.0, DARKGRAY);
                    break;
                case CrossLeftToRight:
                    DrawLineEx({33, 90},{432, 471}, 25.0, DARKGRAY);
                    break;
                case CrossRightToLeft:
                    DrawLineEx({30, 480},{430, 90}, 25.0, DARKGRAY);
                    break;
                case CrossThirdHorizontal:
                    DrawLineEx({20, 440},{440, 440}, 25.0, DARKGRAY);
                    break;  
                case CrossThirdVertical:
                    DrawLineEx({377, 85},{377, 500}, 25.0, DARKGRAY);
                    break;
            }

            // Draw end match message
            if (GameOver)
            {
                if  (WinningBy != VictoryType::None)
                {
                    DrawText(TextFormat("%s won! Press ENTER to play another match.", ((CurrentPlayer == 'O')? "Player 1" : "Player 2")), 10, 40, 10, DARKGREEN);   
                }
                else{
                    DrawText(TextFormat("DRAW! Press ENTER to play another match.", ((CurrentPlayer == 'O')? "Player 1" : "Player 2")), 10, 40, 10, DARKGREEN);   
                }
            }
        }
        EndDrawing();

    }
    CloseWindow(); 

	return 0;
}
