//SKELETON PROGRAM
//---------------------------------//
//include libraries
//include standard libraries
#include <iostream >         //for output and input: cin >> and cout <<
#include <iomanip>           //for formatted output in 'cout'
#include <conio.h>           //for getch()
#include <string>            //for string
using namespace std;

//include our own libraries
#include "RandomUtils.h"     //for Seed, Random
#include "ConsoleUtils.h"    //for Clrscr, Gotoxy, etc.

//---------------------------------
//define constants
//---------------------------------
//define global constants
//defining the size of the grid
const int SIZEY(12);         //vertical dimension
const int SIZEX(20);         //horizontal dimension
//defining symbols used for display of the grid and content
const char SPOT('@');        //spot
const char TUNNEL(' ');      //open space
const char WALL('#');        //border
const char HOLE('0');		//hole 
const char PILL('*');		//pill
const char ZOMBIE('Z');		//zombie
//defining the command letters to move the blob on the maze
const int  UP(72);           //up arrow
const int  DOWN(80);         //down arrow
const int  RIGHT(77);        //right arrow
const int  LEFT(75);         //left arrow
//defining the other command letters
const char QUIT('Q');        //end the game

//data structure to store data for a grid item
struct Item{
	const char symbol;	     //symbol on grid
	int x, y;			     //coordinates
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	void initialiseGame(char grid[][SIZEX], Item& spot, Item holes[], Item pills[], Item zombies[]);
	bool wantToQuit(int k);
	bool isArrowKey(int k);
	int  getKeyPress();
	void updateGame(char g[][SIZEX], Item& sp, int k, string& mess,Item holes[],Item pills[],int& lives, Item zombies[]);
	void renderGame(const char g[][SIZEX], string mess);
	void endProgram();

	//local variable declarations 
	char grid[SIZEY][SIZEX];                //grid for display
	Item spot = { SPOT };                   //Spot's symbol and position (0, 0) 
	int lives = 5;
	string message("LET'S START...      "); //current message to player
	//int holes[12][2]; //holds x and y for each hole
	//not sure how to do this without this weird work around..
	Item holes[] = { { HOLE }, { HOLE }, { HOLE }, { HOLE }, { HOLE }, { HOLE }, { HOLE }, { HOLE }, { HOLE }, { HOLE }, { HOLE }, { HOLE } };
	Item pills[] = { { PILL }, { PILL }, { PILL }, { PILL }, { PILL }, { PILL } };
	Item zombies[] = { { ZOMBIE }, { ZOMBIE }, { ZOMBIE }, { ZOMBIE } };
	//action...
	initialiseGame(grid, spot, holes, pills, zombies);           //initialise grid (incl. walls and spot)
	int key(' ');                         //create key to store keyboard events
	do {
		renderGame(grid, message);        //render game state on screen
		message = "                    "; //reset message
		key = getKeyPress();              //read in next keyboard event
		if (isArrowKey(key))
			updateGame(grid, spot, key, message, holes, pills, lives, zombies);
		else
			message = "INVALID KEY!        "; //set 'Invalid key' message
		cout << lives;
	} while (!wantToQuit(key) && lives > 0);               //while user does not want to quit
	endProgram();                             //display final message
	return 0;
} //end main

void updateGame(char grid[][SIZEX], Item& spot, int key, string& message, Item holes[], Item pills[] ,int& lives, Item zombies[])
{ //updateGame state
	void updateSpotCoordinates(const char g[][SIZEX], Item& spot, int key, string& mess, int& lives);
	void updateGrid(char g[][SIZEX], Item spot, Item holes[], Item pills[], Item zombies[], int key);

	updateSpotCoordinates(grid, spot, key, message,lives);    //update spot coordinates
                                                        //according to key
	updateGrid(grid, spot, holes, pills, zombies, key);                             //update grid information
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], Item& spot, Item holes[], Item pills[], Item zombies[])
{ //initialise grid and place spot in middle
	void setGrid(char[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot);

	void generateZombies(Item zombies[]);
	void placeZombies(char gr[][SIZEX], Item zombies[]);

	void placeSpot(char gr[][SIZEX], Item spot);

	void generateHoles(Item holes[], Item spot, Item zombies[]);
	void placeHoles(char gr[][SIZEX], Item holes[]);

	void generatePills(Item pills[], Item spot, Item holes[], Item zombies[]);
	void placePills(char gr[][SIZEX], Item pills[]);
	

	Seed();                            //seed random number generator
	setSpotInitialCoordinates(spot);   //initialise spot position
	setGrid(grid);                     //reset empty grid
	placeSpot(grid, spot);             //set spot in grid

	generateZombies(zombies);
	placeZombies(grid, zombies);
	//generate and then place zombies

	generateHoles(holes, spot, zombies);
	placeHoles(grid, holes);
	//generate and then place holes

	generatePills(pills, spot, holes, zombies);
	placePills(grid, pills);
	//generate and then place pills

} //end of initialiseGame

void placeZombies(char grid[][SIZEX], Item zombies[]){
	for (int count = 0; count < 4; count++){
		int x, y;
		y = zombies[count].y;
		x = zombies[count].x;
		//grid[row][col]
		grid[y][x] = ZOMBIE;
	}

}

void generateZombies(Item zombies[]){
	for (int count = 0; count < 4; count++){
		int x, y;
		switch (count)
		{
		case 0 :
			zombies[count].x = 1;
			zombies[count].y = 1;
			break;
		case 1 :
			zombies[count].x = 1;
			zombies[count].y = 10;
			break;
		case 2:
			zombies[count].x = 18;
			zombies[count].y = 1;
			break;
		case 3:
			zombies[count].x = 18;
			zombies[count].y = 10;
			break;
		}
	}
}

void moveZombies(char grid[][SIZEX], Item zombies[], Item spot, int key)
{
	//this procedure is VERY FLAWED - only 1 zombie moves, and this is in relation to what arrow is pressed. zombie also can go through walls and holes, etc
	for (int count = 0; count < 4; count++)
	{
		switch (count){
		case 0 :
			switch (key){
			case UP:
				zombies[count].y = zombies[count].y + 1;
				break;
			case DOWN:
				zombies[count].y = zombies[count].y - 1;
				break;
			case RIGHT:
				zombies[count].x = zombies[count].x - 1;
				break;
			case LEFT:
				zombies[count].x = zombies[count].x + 1;
				break;
			}
			switch (/*Zombies new position*/count)
			{		//...depending on what's on the target position in grid...
			case HOLE:
				//ERASE ZOMBIE
				break;
			case WALL:        
				//GO OTHER DIRECTION??
				break;
			}
			break;

		}
	}
}

void placeHoles(char grid[][SIZEX], Item holes[]){
	for (int count = 0; count < 12; count++){
		int x, y;
		y = holes[count].y;
		x = holes[count].x;
		//grid[row][col]
		grid[y][x] = HOLE;
	}

}

void generateHoles(Item holes[], Item spot, Item zombies[]){
	bool checkHoleCoords(int, int, Item[], Item[]);
	Seed();
	for (int count = 0;count < 12;count++){
		int x, y;
		do{
			while ((y = Random(SIZEY - 2)) == spot.y){
			//will repeat while loop if the Random number generated
			//is either the spots location, or other hole locations
			}
			while ((x = Random(SIZEX - 2)) == spot.x){
			//will repeat while loop if the Random number generated
			//is either the spots location, or other hole locations
			}
		} while (!checkHoleCoords(x, y, holes, zombies));
		holes[count].y = y;
		holes[count].x = x;
	}
}

bool checkHoleCoords(int x,int y, Item holes[], Item zombies[]){
	bool isValid = true;
	for (int count = 0; count < 12; count++){
		if (holes[count].x == x && holes[count].y == y){
			isValid = false;
		}
	}
	for (int count = 0; count < 4; count++){
		if (zombies[count].x == x && zombies[count].y == y){
			isValid = false;
		}
	}
	return isValid;
}

void placePills(char grid[][SIZEX], Item pills[]){
	for (int count = 0; count < 6; count++){
		int x, y;
		y = pills[count].y;
		x = pills[count].x;
		//grid[row][col]
		grid[y][x] = PILL;
	}

}

void generatePills(Item pills[], Item spot, Item holes[], Item zombies[]){
	bool checkPillCoords(int, int, Item[], Item[], Item[]);
	Seed();
	for (int count = 0; count < 6; count++){
		int x, y;
		do{
			while ((y = Random(SIZEY - 2)) == spot.y){
				//will repeat while loop if the Random number generated
				//is either the spots location, or other hole locations
			}
			while ((x = Random(SIZEX - 2)) == spot.x){
				//will repeat while loop if the Random number generated
				//is either the spots location, or other hole locations
			}
		} while (!checkPillCoords(x, y, pills, holes, zombies));
		pills[count].y = y;
		pills[count].x = x;
	}
}
bool checkPillCoords(int x, int y, Item pills[], Item holes[], Item zombies[]){
	bool isValid = true;
	for (int count = 0; count < 12; count++){
		if (holes[count].x == x && holes[count].y == y){
			isValid = false;
		}
	}
	for (int count = 0; count < 6; count++){
		if (pills[count].x == x && pills[count].y == y){
			isValid = false;
		}
	}
	for (int count = 0; count < 4; count++){
		if (zombies[count].x == x && zombies[count].y == y){
			isValid = false;
		}
	}

	return isValid;
}

void setSpotInitialCoordinates(Item& spot)
{ //set spot coordinates inside the grid at random at beginning of game
	spot.y = SIZEY/ 2;      //vertical coordinate in range [1..(SIZEY - 2)]
	spot.x = SIZEX/ 2;    //horizontal coordinate in range [1..(SIZEX - 2)]
} //end of setSpotInitialoordinates

void setGrid(char grid[][SIZEX])
{ //reset the empty grid configuration
	for (int row(0); row < SIZEY; ++row) //for each column
	{
		for (int col(0); col < SIZEX; ++col) //for each col
		{
			if ((row == 0) || (row == SIZEY - 1))     //top and bottom walls
				grid[row][col] = WALL;                //draw a wall symbol
			else
				if ((col == 0) || (col == SIZEX - 1)) //left and right walls
					grid[row][col] = WALL;            //draw a wall symbol
				else
					grid[row][col] = TUNNEL;          //draw a space
		} //end of row-loop
	} //end of col-loop
} //end of setGrid

void placeSpot(char gr[][SIZEX], Item spot)
{ //place spot at its new position in grid
	gr[spot.y][spot.x] = spot.symbol;
} //end of placeSpot

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], Item spot, Item holes[], Item pills[], Item zombies[], int key)
{ //update grid configuration after each move
	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeHoles(char[][SIZEX], Item holes[]);
	void placePills(char[][SIZEX], Item pills[]);
	void placeZombies(char[][SIZEX], Item zombies[]);
	void moveZombies(char[][SIZEX], Item zombies[], Item spot, int key);
	setGrid(grid);	         //reset empty grid
	placeHoles(grid, holes); //set holes in grid
	placeSpot(grid, spot);	 //set spot in grid
	placePills(grid, pills); //set pills in grid
	moveZombies(grid, zombies, spot, key);
	placeZombies(grid, zombies);  //set zombies in grid
	//must put spot in after holes!
	
} //end of updateGrid

//---------------------------------------------------------------------------
//----- move the spot
//---------------------------------------------------------------------------
void updateSpotCoordinates(const char g[][SIZEX], Item& sp, int key, string& mess,int& lives)
{ //move spot in required direction
	void setKeyDirection(int k, int& dx, int& dy);

	//calculate direction of movement required by key - if any
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy); 	//find direction indicated by key
	//check new target position in grid 
	//and update spot coordinates if move is possible
	const int targetY(sp.y + dy);
	const int targetX(sp.x + dx);
	switch (g[targetY][targetX])
	{		//...depending on what's on the target position in grid...
	case TUNNEL:      //can move
		sp.y += dy;   //go in that Y direction
		sp.x += dx;   //go in that X direction
		break;
	case HOLE:
		sp.y += dy;
		sp.x += dx; 
		//a life would then be removed.
		lives--;
		break;
	case WALL:        //hit a wall and stay there
		cout << '\a'; //beep the alarm
		mess = "CANNOT GO THERE!    ";
		break;
	}
} //end of updateSpotCoordinates

//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(int key, int& dx, int& dy)
{ //
	switch (key)    //...depending on the selected key...
	{
	case LEFT:      //when LEFT arrow pressed...
		dx = -1;    //decrease the X coordinate
		dy = 0;
		break;
	case RIGHT:     //when RIGHT arrow pressed...
		dx = +1;    //increase the X coordinate
		dy = 0;
		break;
	case UP:	//when UP arrow is pressed...
		dx = 0;
		dy = -1;	//increase the Y coordinate
		break;
	case DOWN:	//when DOWN arrow is pressed...
		dx = 0;
		dy = +1;	//decrease the Y coordinate
		break;
	}
} //end of setKeyDirection

int getKeyPress()
{ //get key or command selected by user
	int keyPressed;
	keyPressed = getch();      //read in the selected arrow key or command letter
	while (keyPressed == 224)     //ignore symbol following cursor key
		keyPressed = getch();
	return(keyPressed);   
} //end of getKeyPress

bool isArrowKey(int key)
{ //check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return ((key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN));
} //end of isArrowKey

bool wantToQuit(int key)
{ //check if the key pressed is 'Q'
	return (key == QUIT);
} //end of wantToQuit


//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------
void clearMessage()
{ //clear message area on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(40, 8);
	string str(20, ' ');
	cout << str;  //display blank message

} //end of setMessage

void renderGame(const char gd[][SIZEX], string mess)
{ //display game title, messages, maze, spot and apples on screen
	void paintGrid(const char g[][SIZEX]);
	void showTitle();
	void showOptions();
	void showMessage(string);

	Gotoxy(0, 0);
	//display grid contents
	paintGrid(gd);
	//display game title
	showTitle();
	//display menu options available
	showOptions();
	//display message if any
	showMessage(mess);
} //end of paintGame

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)      //for each row (vertically)
	{
		for (int col(0); col < SIZEX; ++col)  //for each column (horizontally)
		{
			cout << g[row][col];              //output cell content
		} //end of col-loop
		cout << endl;
	} //end of row-loop
} //end of paintGrid

void showTitle()
{ //display game title
	SelectTextColour(clYellow);
	Gotoxy(0, 0);
	cout << "___ZOMBIES GAME SKELETON___\n" << endl;
	SelectBackColour(clWhite);
	SelectTextColour(clRed);
	Gotoxy(40, 0);
	cout << "Pascale Vacher: March 15";
} //end of showTitle


void showOptions()
{ //show game options
	SelectBackColour(clRed);
	SelectTextColour(clYellow);
	Gotoxy(40, 5);
	cout << "TO MOVE USE KEYBOARD ARROWS  ";
	Gotoxy(40, 6);
	cout << "TO QUIT ENTER 'Q'   ";
} //end of showOptions

void showMessage(string m)
{ //print auxiliary messages if any
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(40, 8);
	cout << m;	//display current message
} //end of showMessage

void endProgram()
{ //end program with appropriate message
	SelectBackColour(clBlack);
	SelectTextColour(clYellow);
	Gotoxy(40, 8);
	cout << "PLAYER QUITS!          ";
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 9);
	system("pause");
} //end of endProgram
