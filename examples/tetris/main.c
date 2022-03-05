//Used for the graphics module
#include "graphTe.h"

//Used for file input
#include <stdio.h>

//Used for the rand module
#include <stdlib.h>

//Used for gravity timing
#include <time.h>

//Used for updating the score and level texts
#include <string.h>

//Macro that returns the max value between two numbers
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

//Defines the size of a square on the grid. Can be changed to match the user preference
const uint16 squareSize = 32;

//Time in milliseconds between a game moves. A game move counts as gravity + input
float moveTime = 500;

//Time in milliseconds that in required before a new user input
const float moveCooldown = 50;

//a simple array that will be intialized later with the rgb values for the pieces
color colors[9];

//the main grid component. stores a number for each color (0 = empty square)
uint16 board[10][20];

//all the tetriminoes are loaded into this matrix with the identifiers:
// 1) pieceID
// 2) rotationID
// 3) x coord
// 4) y coord
uint16 tetriminos[7][4][4][4];

//the current selected piece(the one that drops down) is stored into this struct for convenience
struct
{
	uint16 id, rotation; 
	short int x, y;
}
selectedTetrimino;

//deltaTime stores the time in process clocks between each individual frame
clock_t deltaTime = 0;

//elapsed time stores the time in milliseconds that has passed since the last gravity drop
float elapsedTime = 0;

//Additional variables that influence the game factors along the playtime
unsigned int level = 1;
unsigned long long int score = 0;
float additionalMoveTime = 50; 
uint16 passedRows = 0;

//A char* is required as a passing argument for the text() function from graphTe.h
char scoreText[100];
char levelText[100];

//the keepAlive variable. if set to false the game will throw a gameOver screen and exit
BOOL gameLoop = TRUE;

//color intialization function
void initColors()
{
	colors[0] = rgb(0, 0, 0);
	colors[1] = rgb(0, 255, 255);
	colors[2] = rgb(255, 255, 0);
	colors[3] = rgb(128, 0, 128);
	colors[4] = rgb(0, 255, 0);
	colors[5] = rgb(255, 0, 0);
	colors[6] = rgb(0, 0, 255);
	colors[7] = rgb(255, 0, 0);
	colors[8] = rgb(30, 30, 30); 
}

//this function loads all the pieces from a text file into the respective matrix
void loadTetriminos()
{
	//pieces can be reconfigured in tetriminos.txt
	FILE* file = fopen("assets/tetriminos.txt","r");

	for(unsigned short int tetrimino = 0; tetrimino < 7; tetrimino++)
	{
		for(unsigned short int rotation = 0; rotation < 4; rotation++)
		{
			for(unsigned short int x = 0; x < 4; x++)
			{
				for(unsigned short int y = 0; y < 4; y++)
				{
					//the syntax of the text file must match the default one for correct execution
					fscanf(file, "%d", &tetriminos[tetrimino][rotation][x][y]);
				}
			}
		}
	}

	fclose(file);
}

//this function generates a new random piece and puts it at the top of the screen
void newTetrimino()
{
	selectedTetrimino.x = 3;
	selectedTetrimino.y = 0;

	selectedTetrimino.rotation = 0;

	srand(time(0));
	selectedTetrimino.id = rand() % 7;
}

//this function is responsible for drawing everything to the screen
//a frame call starts with this function effectively setting the framerate to the bufferTime of display()
void render()
{
	//background drawing:
	fill(colors[0]);
	image(0, 0, 600, 700, "assets/controls.bmp");

	//board pieces drawing:
	for(int x = 0; x < 10; x++)
		for(int y = 0; y < 20; y++)
			if(board[x][y])
				rect(x * squareSize, y * squareSize, squareSize, squareSize, colors[board[x][y]]);

	//selected tetrimono drawing:
	for(int x = 0; x < 4; x++)
		for(int y = 0; y < 4; y++)
			if(tetriminos[selectedTetrimino.id][selectedTetrimino.rotation][x][y])
				rect((x + selectedTetrimino.x) * squareSize, (y + selectedTetrimino.y) * squareSize, squareSize, squareSize, colors[tetriminos[selectedTetrimino.id][selectedTetrimino.rotation][x][y]]);

	//grid lines drawing:
	for(int x = 0; x <= 10; x++)
		line(x * squareSize, 0, x * squareSize, squareSize * 20, squareSize / 16, colors[8]);
	for(int y = 0; y <= 20; y++)
		line(0, y * squareSize, 10 * squareSize, y * squareSize, squareSize / 16, colors[8]);

	//score & level drawing:
	text(400, 64, scoreText, rgb(255, 255, 255));
	text(400, 128, levelText, rgb(255, 255, 255));

	//copy the memory bitmap into the GPU buffer by creating a new render job:
	display();
}

//this function makes the selected piece drop down when the moveTime has passed
void gravity()
{
	//calculation of the elapsed time variable:
	deltaTime = clock() - deltaTime;
	elapsedTime += (float)deltaTime / CLOCKS_PER_SEC;
	deltaTime = clock();

	//only if moveTime has passed the piece gets dropped down
	if(elapsedTime * 1000 >= MAX(moveTime - level * additionalMoveTime, 50))
	{
		elapsedTime = 0;
		selectedTetrimino.y++;
	}
}

//this function returns TRUE(winapi) if two pieces colide
BOOL collisionLeftRight()
{
	//rotation collision check:
	//this is used to check if rotating a piece will collide with anything on the board
	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			if(tetriminos[selectedTetrimino.id][selectedTetrimino.rotation][x][y] && board[x + selectedTetrimino.x][y + selectedTetrimino.y])
				return TRUE;
		}
	}

	//this checks if a piece has crossed the outline boundaries for each piece type and respective rotation
	if(selectedTetrimino.id == 0)
	{
		if(selectedTetrimino.rotation == 0 || selectedTetrimino.rotation == 2)
		{
			if(selectedTetrimino.x < -2 || selectedTetrimino.x > 7)
				return TRUE;
		}
		else
		{
			if(selectedTetrimino.x < 0 || selectedTetrimino.x > 6)
				return TRUE;
		}
	}
	else if(selectedTetrimino.id == 1)
	{
		if(selectedTetrimino.x < -1 || selectedTetrimino.x > 7)
			return TRUE;
	}
	else if(selectedTetrimino.id == 2)
	{
		if(selectedTetrimino.rotation == 0)
		{
			if(selectedTetrimino.x < -1 || selectedTetrimino.x > 7)
				return TRUE;
		}
		else if(selectedTetrimino.rotation == 1 || selectedTetrimino.rotation == 3)
		{
			if(selectedTetrimino.x < 0 || selectedTetrimino.x > 7)
				return TRUE;
		}
		else
		{
			if(selectedTetrimino.x < 0 || selectedTetrimino.x > 8)
				return TRUE;
		}
	}
	else if(selectedTetrimino.id == 3 || selectedTetrimino.id == 4)
	{
		if(selectedTetrimino.rotation == 0 || selectedTetrimino.rotation == 2)
		{
			if(selectedTetrimino.x < -1 || selectedTetrimino.x > 7)
				return TRUE;
		}
		else
		{
			if(selectedTetrimino.x < 0 || selectedTetrimino.x > 7)
				return TRUE;
		}
	}
	else if(selectedTetrimino.id == 5 || selectedTetrimino.id == 6)
	{
		if(selectedTetrimino.rotation == 0)
		{
			if(selectedTetrimino.x < -1 || selectedTetrimino.x > 7)
				return TRUE;
		}
		else if(selectedTetrimino.rotation == 1)
		{
			if(selectedTetrimino.x < 0 || selectedTetrimino.x > 7)
				return TRUE;
		}
		else if(selectedTetrimino.rotation == 2)
		{
			if(selectedTetrimino.x < 0 || selectedTetrimino.x > 8)
				return TRUE;
		}
		else
		{
			if(selectedTetrimino.x < 0 || selectedTetrimino.x > 7)
				return TRUE;
		}
	}

	//returns FALSE in case the piece dosent collide with anything or an error is thrown
	return FALSE;
}


//this function prints the selected tetrimono to the board memory, effectively making it permanent
void placeTetrimono()
{
	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			if(tetriminos[selectedTetrimino.id][selectedTetrimino.rotation][x][y])
			{
				board[x + selectedTetrimino.x][y + selectedTetrimino.y] = tetriminos[selectedTetrimino.id][selectedTetrimino.rotation][x][y];
			}
		}
	}
	
	newTetrimino();
}

//this function checks if the selected piece has collided with the last row or with the bottom piece on the board
void collision()
{
	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			if(tetriminos[selectedTetrimino.id][selectedTetrimino.rotation][x][y] && (board[x + selectedTetrimino.x][y + selectedTetrimino.y + 1] || y + selectedTetrimino.y + 1 >= 20))
			{
				placeTetrimono();
			}
		}
	}
}

//this function sets a new value for the level and is called only when the level check has been trigered
void updateLevel()
{
	for(int i = 0; i < 100; i++)
	{
		 levelText[i] = 0;
	}	
	sprintf(levelText, "Level:\n%d", level);
}

//this function removes a line from the board matrix and is called when a new full line is made
void deleteLine(int deleteRowY)
{
	for(int x = 0; x < 10; x++)
	{
		for(int y = deleteRowY; y > 0; y--)
		{
			board[x][y] = board[x][y - 1];
		}
	}

	//updates the destroyed rows for the level mechanic
	passedRows++;
	if(passedRows % 10 == 0)
	{
		level++;
		updateLevel();
	}
}

//this function sets a new value for the score and is called only when the score check has been trigered
void updateScore()
{
	for(int i = 0; i < 100; i++)
	{
		 scoreText[i] = 0;
	}	
	sprintf(scoreText, "SCORE:\n%d", score);
}

//this function checks the entire board matrix for newly created full lines
void checkTetris()
{
	uint16 linesInARow = 0;
	BOOL fullLine;

	//goes though the board from the bottom up
	for(int y = 19; y >= 0; y--)
	{
		//we make the assumption that the next line will be full
		fullLine = TRUE;
		for(int x = 0; x < 10; x++)
		{
			//is the case that we find a single exception we know that the line is not full anymore
			if(board[x][y] == 0)
			{
				fullLine = FALSE;
				break;
			}
		}
		
		//if the line is full we delete it and check for more fulllines in a row
		if(fullLine)
		{
			linesInARow++;
			deleteLine(y);
		}
		//if there is no fullLine we add score depending on how many fulllines in a row have been found
		else
		{
			//this score implementation uses the original NES values and therefore keeps authenticity
			switch(linesInARow)
			{
				case 1:
					score += 40 * (level + 1);
					updateScore();
					break;
				case 2:
					score += 100 * (level + 1);
					updateScore();
					break;
				case 3:
					score += 300 * (level + 1);
					updateScore();
					break;
				case 4:
					score += 1200 * (level + 1);
					updateScore();
					break;
			}

			//after getting a score update the stacked lines variables must be set to 0 again
			linesInARow = 0;
		}
	}
}

//this function checks for new user input and runs the coresponding code
void move()
{
	//move selected piece left
	if(checkKeyLiveInput(VK_LEFT))
	{
		selectedTetrimino.x--;
		if(collisionLeftRight())
			selectedTetrimino.x++;

		Sleep(moveCooldown);
	}
	//move selected piece right
	else if(checkKeyLiveInput(VK_RIGHT))
	{
		selectedTetrimino.x++;
		if(collisionLeftRight())
			selectedTetrimino.x--;

		Sleep(moveCooldown);
	}
	//rotates selected piece clockwise
	else if(checkKeyLiveInput(VK_UP))
	{
		selectedTetrimino.rotation++;
		if(selectedTetrimino.rotation > 3)
			selectedTetrimino.rotation = 0;

		if(collisionLeftRight())
		{
			if(selectedTetrimino.rotation == 0)
			{
				selectedTetrimino.rotation = 3;
			}
			else
			{
				selectedTetrimino.rotation--;
			}
		}

		render();
		Sleep(moveCooldown * 4);
	}
	//speeds up the game time (this game mechanic is called soft-drop)
	else if(checkKeyLiveInput(VK_DOWN))
	{
		moveTime = 50;
	}
	else
	{
		moveTime = 500;
	}
}

//this function disables the game loop if the top line is reached
void checkEndgame()
{
	for(int x = 0; x < 10; x++)
	{
		if(board[x][0] != 0)
			gameLoop = FALSE;
	}
}

int main()
{
	//this is required to be at the start of each program that uses graphTe as it enables the graphical mode on the console
	//it is recomended to not use any graphTe program logic before running initHost()
	initHost();

	//setting the window tile and boundaries(size)
	setWindowTitle("Tetris");
	setWindowSize(600, 700);

	//this function updates the canvas in memory to the new window resolution
	update();

	//start screen
	image(0, 0, 600, 700, "assets/start.bmp");
	display();
	forceInput();

	//setup
	score = 0;
	level = 0;
	passedRows = 0;

	initColors();
	loadTetriminos();
	newTetrimino();
	updateScore();
	updateLevel();

	//frameloop
	while(gameLoop)
	{
		render();
		move();
		gravity();
		collision();
		checkTetris();
		checkEndgame();
	}

	//endscreen
	image(0, 0, 600, 700, "assets/endscreen.bmp");
	display();

	//countdown before program exit
	Sleep(5000);

	//this function is required when using graphTe.h in order to free up the memory of the GPU buffer and re enable textmode on the terminal
	releaseHost();
}