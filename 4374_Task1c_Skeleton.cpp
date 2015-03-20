//SKELETON PROGRAM
//---------------------------------//
//include libraries
//include standard libraries
#include <iostream >         //for output and input: cin >> and cout <<
#include <iomanip>           //for formatted output in 'cout'
#include <conio.h>           //for getch()
#include <string>            //for string
#include <vector>

using namespace std;

//include our own libraries
#include "RandomUtils.h"     //for Seed, Random
#include "ConsoleUtils.h"    //for Clrscr, Gotoxy, etc.
#include <time.h>			//for outputting time
#include <stdio.h>			//for outputting time

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
const char PLAY('P');		//play the game
//data structure to store data for a grid item
struct Item{
	const char symbol;	     //symbol on grid
	int x, y;			     //coordinates
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------
//function declarations (prototypes)

void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies);
bool isArrowKey(int k);
void updateGame(char g[][SIZEX], Item& sp, int k, string& mess, vector<Item> &holes, vector<Item> &pills, int& lives, vector<Item> &zombies);
void renderGame(const char g[][SIZEX], string mess);
void endProgram();
void gameEntry();
int  getKeyPress();
bool wantToQuit(int k);
void enterGame();
const string displayTime();


int main()
{
	//action...
	gameEntry();
	return 0;
} //end main


void gameEntry() //first console screen
{
	bool wantToPlay(int k);
	cout << " SPOT   GROUP 1RR - Fraser Burns, Ellie Fuller, Roddy Munro \n";
	cout << "date/time: " << displayTime() << endl;
	cout << "Press 'P' to play game \n" << "Press 'Q' to quit";
	int key(' ');

	while (wantToQuit(key) == false)
	{
		key = getKeyPress();
		if (wantToPlay(key) == true)
		{
			enterGame();
		}
	}
	endProgram();
}

void enterGame() //console screen where you play the game
{   //local variable declarations 
	char grid[SIZEY][SIZEX];                //grid for display
	Item spot = { SPOT };                   //Spot's symbol and position (0, 0) 
	int lives = 5;
	string message("LET'S START...      "); //current message to player
	//int holes[12][2]; //holds x and y for each hole
	//not sure how to do this without this weird work around..
	vector<Item> holes;
	vector<Item> pills;
	vector<Item> zombies;

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
	endProgram(); //display final message
}

const string displayTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%d-%m-%Y.%X", &tstruct);
	return buf;
}

void updateGame(char grid[][SIZEX], Item& spot, int key, string& message, vector<Item> &holes, vector<Item> &pills, int& lives, vector<Item> &zombies)
{ //updateGame state
	void updateSpotCoordinates(const char g[][SIZEX], Item& spot, int key, string& mess, int& lives);
	void updateGrid(char g[][SIZEX], Item spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies, int key);

	updateSpotCoordinates(grid, spot, key, message, lives);    //update spot coordinates
	//according to key
	updateGrid(grid, spot, holes, pills, zombies, key);                             //update grid information
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies)
{ //initialise grid and place spot in middle
	void setGrid(char[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot);

	void generateZombies(vector<Item> &zombie);
	void placeZombies(char gr[][SIZEX], vector<Item> zombies);

	void placeSpot(char gr[][SIZEX], Item spot);

	void generateHoles(vector<Item> &holes, Item spot, vector<Item> zombies);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);

	void generatePills(vector<Item> &pills, Item spot, vector<Item> holes, vector<Item> zombie);
	void placePills(char gr[][SIZEX], vector<Item> pills);


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

void placeZombies(char grid[][SIZEX], vector<Item> zombies){
	for (Item zombie : zombies){
		int x, y;
		//(zombies.at(count)).y;
		y = zombie.y;
		x = zombie.x;
		grid[y][x] = ZOMBIE;
	}

}

void generateZombies(vector<Item> &zombies){
	//do we need a for loop here? 
	//seems pointless, we could literally just have 
	//zombies[0].x = 1
	//zombies[1].x = .. etc

	for (int count = 0; count < 4; count++){
		int x, y;
		switch (count)
		{
		case 0:
			x = 1;
			y = 1;

			break;
		case 1:
			x = 1;
			y = 10;
			break;
		case 2:
			x = 18;
			y = 1;
			break;
		case 3:
			x = 18;
			y = 10;
			break;
		}
		Item zombie = { ZOMBIE, x, y };
		zombies.push_back(zombie);
	}
}

void moveZombies(char grid[][SIZEX], vector<Item> &zombies, Item spot, int key)
{
	//this procedure is VERY FLAWED - only 1 zombie moves, and this is in relation to what arrow is pressed. zombie also can go through walls and holes, etc
	//attempted to change it, got all 4 moving, but they are WAY too smart. I think we should add some kind of random element in it
	int count = 0;
	for (Item zombie : zombies)
	{
		if (zombie.x > spot.x){
			zombie.x = zombie.x - 1;
			cout << "1";
		}
		else if (zombie.x < spot.x){
			zombie.x++;
			cout << "2";
		}
		else if (zombie.y < spot.y){
			zombie.y++;
			cout << "3";
		}
		else if (zombie.y > spot.y){
			cout << "4";
			zombie.y--;
		}
		zombies.at(count).x = zombie.x;
		zombies.at(count).y = zombie.y;
		/*
		switch (/*Zombies new position)
		{		//...depending on what's on the target position in grid...
		case HOLE:
		//ERASE ZOMBIE
		break;
		case WALL:
		//GO OTHER DIRECTION??
		break;
		}*/
		count++;
	}
}

void placeHoles(char grid[][SIZEX], vector<Item> holes){
	for (Item hole : holes){
		int x, y;
		y = hole.y;
		x = hole.x;
		//grid[row][col]
		grid[y][x] = HOLE;
	}

}

void generateHoles(vector<Item> &holes, Item spot, vector<Item> zombies){
	bool checkHoleCoords(int, int, vector<Item>, vector<Item>);
	Seed();
	for (int count = 0; count < 12; count++){
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
		Item hole = { HOLE, x, y };
		holes.push_back(hole);
	}
	cout << "stop";
}

bool checkHoleCoords(int x, int y, vector<Item> holes, vector<Item> zombies){
	bool isValid = true;
	for (int count = 0; count < holes.size(); count++){
		if (holes.at(count).x == x && holes.at(count).y == y){
			isValid = false;
		}
	}
	for (int count = 0; count < zombies.size(); count++){
		if (zombies.at(count).x == x && zombies.at(count).x == y){
			isValid = false;
		}
	}
	return isValid;
}

void placePills(char grid[][SIZEX], vector<Item> pills){
	for (Item pill : pills){
		int x, y;
		y = pill.y;
		x = pill.x;
		//grid[row][col]
		grid[y][x] = PILL;
	}

}

void generatePills(vector<Item> &pills, Item spot, vector<Item> holes, vector<Item> zombies){
	bool checkPillCoords(int, int, vector<Item>, vector<Item>, vector<Item>);
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
		Item pill = { PILL, x, y };
		pills.push_back(pill);
	}
}
bool checkPillCoords(int x, int y, vector<Item> pills, vector<Item> holes, vector<Item> zombies){
	bool isValid = true;
	for (int count = 0; count < holes.size(); count++){
		if (holes.at(count).x == x && holes.at(count).y == y){
			isValid = false;
		}
	}
	for (int count = 0; count < zombies.size(); count++){
		if (zombies.at(count).x == x && zombies.at(count).x == y){
			isValid = false;
		}
	}
	for (int count = 0; count < pills.size(); count++){
		if (pills.at(count).x == x && pills.at(count).y == y){
			isValid = false;
		}
	}


	return isValid;
}

void setSpotInitialCoordinates(Item& spot)
{ //set spot coordinates inside the grid at random at beginning of game
	spot.y = SIZEY / 2;      //vertical coordinate in range [1..(SIZEY - 2)]
	spot.x = SIZEX / 2;    //horizontal coordinate in range [1..(SIZEX - 2)]
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

void updateGrid(char grid[][SIZEX], Item spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies, int key)
{ //update grid configuration after each move
	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeHoles(char[][SIZEX], vector<Item> holes);
	void placePills(char[][SIZEX], vector<Item> pills);
	void placeZombies(char[][SIZEX], vector<Item> zombies);
	void moveZombies(char[][SIZEX], vector<Item> &zombies, Item spot, int key);
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
void updateSpotCoordinates(const char g[][SIZEX], Item& sp, int key, string& mess, int& lives)
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

bool wantToPlay(int key)
{ //check if key 'P' is pressed
	return (key == PLAY);
}


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
