#include "graphTe.h"

#include "string.h"

#include <stdio.h>
#include <stdlib.h>

const unsigned short int pieceSize = 100;

char pieces[8][8];	

unsigned int mouseState = 0, selectedX, selectedY, newX, newY;
POINT mPos;

char FEN[100];
char pos[2];
char activeColor[2] = "b";
char castlingAvailability[4] = "-";
char enPassantTargetSquare[3] = "-";
char halfmoveClock[100] = "0";
char fullmoveNumber[100] = "1";

void generateFEN()
{
	FEN[0] = 0;
	
	int empty = 0;

	for(int y = 0; y < 8; y++)
	{
		for(int x = 0; x < 8; x++)
		{
			if(pieces[x][y])
			{
				if(empty)
				{
					pos[0] = empty + '0';
					pos[1] = 0;
					strcat(FEN, pos);

					empty = 0;
				}
				
				pos[0] = pieces[x][y];
				pos[1] = 0;
				strcat(FEN, pos);
			}
			else
			{
				empty++;
				if(x == 7)
				{
					pos[0] = empty + '0';
					pos[1] = 0;
					strcat(FEN, pos);

					empty = 0;
				}
			}
		}
		if(y != 7)
		{
			pos[0] = '/';
			pos[1] = 0;
			strcat(FEN, pos);
		}
	}

	strcat(FEN, " ");
	strcat(FEN, activeColor);
	strcat(FEN, " ");
	strcat(FEN, castlingAvailability);
	strcat(FEN, " ");
	strcat(FEN, enPassantTargetSquare);
	strcat(FEN, " ");
	strcat(FEN, halfmoveClock);
	strcat(FEN, " ");
	strcat(FEN, fullmoveNumber);
}

void executeComputerMove(char* move)
{
	printf("%s", move);
	Sleep(100000);
}

//start stuff:

void generateComputerMove()
{
	fork();
}	

void resetBoard()
{
	char newPieces[8][8] = {{'r', 'p',  0 ,  0 ,  0 ,  0 , 'P', 'R'},
					 		{'n', 'p',  0 ,  0 ,  0 ,  0 , 'P', 'N'},
					 		{'b', 'p',  0 ,  0 ,  0 ,  0 , 'P', 'B'},
					 		{'q', 'p',  0 ,  0 ,  0 ,  0 , 'P', 'Q'},
					 		{'k', 'p',  0 ,  0 ,  0 ,  0 , 'P', 'K'},
					 		{'b', 'p',  0 ,  0 ,  0 ,  0 , 'P', 'B'},
					 		{'n', 'p',  0 ,  0 ,  0 ,  0 , 'P', 'N'},
					 		{'r', 'p',  0 ,  0 ,  0 ,  0 , 'P', 'R'}};	

	memcpy(pieces, newPieces, 64 * sizeof(char));
}

void drawNotation()
{
	text(1 * pieceSize - 15, 8 * pieceSize - 15, "A", rgb(0, 0, 0));
	text(2 * pieceSize - 15, 8 * pieceSize - 15, "B", rgb(0, 0, 0));
	text(3 * pieceSize - 15, 8 * pieceSize - 15, "C", rgb(0, 0, 0));
	text(4 * pieceSize - 15, 8 * pieceSize - 15, "D", rgb(0, 0, 0));
	text(5 * pieceSize - 15, 8 * pieceSize - 15, "E", rgb(0, 0, 0));
	text(6 * pieceSize - 15, 8 * pieceSize - 15, "F", rgb(0, 0, 0));
	text(7 * pieceSize - 15, 8 * pieceSize - 15, "G", rgb(0, 0, 0));
	text(8 * pieceSize - 15, 8 * pieceSize - 15, "H", rgb(0, 0, 0));

	text(0, 7 * pieceSize, "1", rgb(0, 0, 0));
	text(0, 6 * pieceSize, "2", rgb(0, 0, 0));
	text(0, 5 * pieceSize, "3", rgb(0, 0, 0));
	text(0, 4 * pieceSize, "4", rgb(0, 0, 0));
	text(0, 3 * pieceSize, "5", rgb(0, 0, 0));
	text(0, 2 * pieceSize, "6", rgb(0, 0, 0));
	text(0, 1 * pieceSize, "7", rgb(0, 0, 0));
	text(0, 0 * pieceSize, "8", rgb(0, 0, 0));
}

void drawPiece(int x, int y, int posX, int posY)
{
	switch(pieces[x][y])
	{
		case 'r':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/blackRook.bmp",rgb(0, 255, 0));
			break;
		case 'n':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/blackKnight.bmp",rgb(0, 255, 0));
			break;
		case 'b':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/blackBishop.bmp",rgb(0, 255, 0));
			break;
		case 'q':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/blackQueen.bmp",rgb(0, 255, 0));
			break;
		case 'k':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/blackKing.bmp",rgb(0, 255, 0));
			break;
		case 'p':
	transparentImage(posX, posY, pieceSize, pieceSize, "pieces/blackPawn.bmp",rgb(0, 255, 0));
			break;

		case 'R':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/whiteRook.bmp",rgb(0, 255, 0));
			break;
		case 'N':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/whiteKnight.bmp",rgb(0, 255, 0));
			break;
		case 'B':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/whiteBishop.bmp",rgb(0, 255, 0));
			break;
		case 'Q':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/whiteQueen.bmp",rgb(0, 255, 0));
			break;
		case 'K':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/whiteKing.bmp",rgb(0, 255, 0));
			break;
		case 'P':
			transparentImage(posX, posY, pieceSize, pieceSize, "pieces/whitePawn.bmp",rgb(0, 255, 0));
			break;
	}
}

void changeMouseState()
{
	if(mouseState)
		mouseState = 0;
	else
		mouseState = 1;
}

void render()
{
	//clearScreen
	fill(rgb(0, 0, 0));

	//board:
	for(int x = 0; x < 8; x++)
	{
		for(int y = 0; y < 8; y++)
		{
			if((x + y) % 2)
				rect(x * pieceSize, y * pieceSize, pieceSize, pieceSize, rgb(184, 136, 97));
			else
				rect(x * pieceSize, y * pieceSize, pieceSize, pieceSize, rgb(239, 220, 180));

			drawPiece(x, y, x * pieceSize, y * pieceSize);
		}
	}
	
	//MousePiece:
	if(mouseState == 1)
	{
		if((selectedX + selectedY) % 2)
			rect(selectedX * pieceSize, selectedY * pieceSize, pieceSize, pieceSize, rgb(184, 136, 97));
		else
			rect(selectedX * pieceSize, selectedY * pieceSize, pieceSize, pieceSize, rgb(239, 220, 180));

		drawNotation();
		drawPiece(selectedX, selectedY, mPos.x - pieceSize / 2, mPos.y - pieceSize / 2);
	}
	else
	{
		drawNotation();
	}
	

	display();
}

void playerInput()
{
	mPos = getMousePosition();

	if(checkKeyLiveInput(VK_LBUTTON))
	{
		if(!mouseState)
		{
			selectedX = mPos.x / pieceSize;
			selectedY = mPos.y / pieceSize;

			if(pieces[selectedX][selectedY] && selectedX >= 0 && selectedX < 8 && selectedY >= 0 && selectedY < 8)
			{
				changeMouseState();
			}
		}
	}
	else
	{
		if(mouseState)
		{
			newX = mPos.x / pieceSize;
			newY = mPos.y / pieceSize;

			if(newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
			{
				char temp = pieces[selectedX][selectedY];
				pieces[selectedX][selectedY] = 0;
				pieces[mPos.x / pieceSize][mPos.y / pieceSize] = temp;

				changeMouseState();
				render();
				generateFEN();
				generateComputerMove();
			}
			else
			{
				changeMouseState();
			}
		}
	}

	if(checkKeyLiveInput(VK_END) && !mouseState)
	{
		resetBoard();
	}

	if(checkKeyLiveInput(VK_ESCAPE) && !mouseState)
	{
		generateFEN();
	}
}

int main()
{
	initHost();
	setWindowTitle("Chess - Ene Alexandru_Florin");
	setWindowSize(2000, 1000);
	update();
	
	resetBoard();
	while(1)
	{		
		playerInput();
		render();
	}

	releaseHost();
}