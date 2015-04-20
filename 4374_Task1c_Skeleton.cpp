//SKELETON PROGRAM
//---------------------------------//
//include libraries
//include standard libraries
#include <iostream >         //for output and input: cin >> and cout <<
#include <iomanip>           //for formatted output in 'cout'
#include <conio.h>           //for getch()
#include <string>            //for string
#include <vector>			//for vectors
#include <fstream>			//for files

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
const char INFO('I');		//get info about the game
//defining the cheats
const char FREEZE('F');		//freeze game
const char EXTERMINATE('X');	//exterminate remaining zombies
const char EAT('E');		//eat all remaining pills
//data structure to store data for a grid item
struct Item{
	const char symbol;	     //symbol on grid
	int x, y;			     //coordinates
	bool destroyed, exterminated; //boolean var's for cheat enabling
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------
//function declarations (prototypes)
void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies);
bool isArrowKey(int k);
void updateGame(char g[][SIZEX], Item& sp, int k, string& mess, vector<Item> &holes, vector<Item> &pills, int& lives, int& countPills, vector<Item> &zombies, bool zombiesFrozen, bool wantToExterminate, bool &exterminated);
void renderGame(const char g[][SIZEX], string mess, string playerName);
void quitProgram();
void noLivesLeft();
void noZombiesLeft(int countPills);
void gameEntry();
void infoScreen();
int  getKeyPress();
bool wantToQuit(int k);
bool wantToFreeze(int f);
bool wantToEat(int e);
bool wantToExterminateZombies(int x);
void enterGame(string playerName);
const string displayTime();
const string s = "You are playing SPOT!\n\nSpot is a game where you (spot) have to try outrun the zombies with as many remaining pills as possible. You have 5 lives, which are deducted if a zombie hits you or you hit a hole.\n\nTo beat highscores, you must try complete the game in the shortest time and in the least amount of moves!\n\nCheats enabled in this Game:\nPress 'F' to freeze zombies\nPress 'X' to exterminate all zombies\nPress 'E' to eat all pills\n\nPress enter to return to entry screen";

int main()
{
	gameEntry(); //function call to first console screen
	return 0;
} //end main


void gameEntry() //first console screen
{
	//set up functions for different key presses
	bool wantToPlay(int k); 
	bool wantInformation(int k);
	//setting up function to check validity of user input 
	bool checkForSpaces(string playerName);
	//info
	SelectBackColour(clBlack);
	SelectTextColour(clGreen);
	Gotoxy(60, 0);
	cout << displayTime() << endl;

	Gotoxy(30, 0);

	cout << " SPOT   GROUP 1RR\n\nFraser Burns -24017624, Ellie Fuller -24044160, Roddy Munro -24006031\n\n";

	SelectTextColour(clYellow);
	cout << "Press 'P' to play game \n" << "Press 'Q' to quit \n" << "Press 'I' for more information" ;
	int key(' '); //create key to store keyboard events
	string playerName; //define playerName as string
	while (wantToQuit(key) == false && wantToPlay(key)==false && wantInformation(key) == false) //runs loop until user presses a key (p, q or i)
	{
		key = getKeyPress(); //reads in key from keyboard
		if (wantInformation(key) == true) //if key pressed = I, returns true
		{
			Clrscr(); //clear console window
			infoScreen(); //function call which displays information about the game
		}
		if (wantToPlay(key) == true)
		{
			Clrscr(); // change this
			do {
				cout << "Enter player name (20 characters max): ";
				getline(cin, playerName); //user input
			} while ((playerName.length() > 20) || (checkForSpaces(playerName) == true)); //run loop until user enters a valid name (less than 20, no spaces)
			Clrscr(); //clear console window
			enterGame(playerName); //pass playername into function by value
		}

	}
	
}

void infoScreen() //function displaying game information
{
	void outputText(string s); //function prototype
	outputText(s); //calling function which returns a string 's'
	if (cin.get() == '\n') //when user presses enter key, clear screen and return to original console window
	{
		Clrscr();
		gameEntry();
	}
	
}

void outputText(string s) //function which processes the string, implements word wrap as lot of writing for console
{
	int bufferWidth = 80; //set bufferwidth to standard size 80

	for (unsigned int i = 1; i <= s.length(); i++) //looping through each character in the string passed to the function
	{
		char c = s[i - 1]; //temp var for each character into 'c' (i-1 as loop starts from 1, array starts from 0)

		int spaceCount = 0; //initialise variable to keep note of how many characters looped back through to find a space

		if (c == '\n') //if '\n' is detected in string, new line
		{
			int charNumOnLine = ((i) % bufferWidth); //temp var which contains characters position on the line
			spaceCount = bufferWidth - charNumOnLine; //calculate amount of spaces needed to insert into string to move #
			s.insert((i - 1), (spaceCount), ' '); //insert blank space
			i += (spaceCount); //increment loop by amount of spaces we just inserted
			continue; 
		}

		if ((i % bufferWidth) == 0) //check i is a multiple of our buffer
		{
			if (c != ' ') //if the character isn't a blank space
			{
				for (int j = (i - 1); j > -1; j--) //loop until we find a space (end of a word)
				{
					if (s[j] == ' ')
					{
						s.insert(j, spaceCount, ' '); //found last word that fits on line, input spaces till it fits buffer width
						break;
					}
					else spaceCount++; //increment spaces to fill the line to the end
				}
			}
		}
	}
	cout << s << endl; //output the string
}

void doScoreStuff(string playerName, int lives) //calculate high scores, passing in current players name and lives left over
{
	string sScore; //initialise variable to read current highscore in file
	int highScore = 0; //initialise highscore as 0
	ifstream inFile(playerName + ".txt"); //declare file variable for input named with the playername variable, followed by .txt to save as score file


	if (inFile){ //if file exists
		getline(inFile, sScore); //read line from scr file and store in string score

		if (!sScore.empty()){//if string is not empty...
			highScore = stoi(sScore); //...convert string to integer, stored in highScore
		}
	}
	

	if (lives > highScore) { //if lives in current game is greater than a highscore stored
		ofstream file(playerName + ".txt"); //declare output file
		file << lives; //store integer 'lives' into the scr file
		file.clear(); //clear contents of file
		file.close();//close output file
	}

	inFile.close();//close input file
}


bool checkForSpaces(string playerName) //check for spaces when user inputs name
{
	bool spacesPresent = false; //initialise variable for checking spaces
	
	for (int count = 0; count < playerName.length(); count++) //loop through each character in string playerName
	{
		if (playerName[count] == ' '){  //checking if character is a space
			spacesPresent = true; //set variable to true if space existed
		}
	}
	return spacesPresent; //return true/false to determine validity of name
}

void enterGame(string playerName) //console screen where you play the game
{   //local variable declarations 
	char grid[SIZEY][SIZEX];                //grid for display
	Item spot = { SPOT };                   //Spot's symbol and position (0, 0) 
	int lives = 5, zombiesFreezeCount = 1, zombiesExterminateCount = 1, countPills = 8; //initialising variables
	string message("LET'S START...      "); //current message to player

	//create vectors
	vector<Item> holes; 
	vector<Item> pills;
	vector<Item> zombies;

	void doScoreStuff(string, int); //call function which calculates score for current game 
	
	bool zombiesRemain(vector<Item> zombies); //function prototype

	initialiseGame(grid, spot, holes, pills, zombies);           //initialise grid (incl. walls and spot)
	int key(' ');                         //create key to store keyboard events
	bool zombiesFrozen = FALSE, wantToExterminate = FALSE, exterminated = FALSE; //initialise local variables
	do {
		renderGame(grid, message, playerName);        //render game state on screen
		message = "                    "; //reset message
		key = getKeyPress();              //read in next keyboard event
		if (isArrowKey(key)) //checks if key is one of the arrow keys
			updateGame(grid, spot, key, message, holes, pills, lives, countPills, zombies, zombiesFrozen, wantToExterminate, exterminated); //calls funciton which will change spots position on the grid
		else if (wantToFreeze(key)) //checks if key is 'F' 
		{
			zombiesFreezeCount++; //count how many times zombies have been frozen
			if ((zombiesFreezeCount % 2) == 0) //check if user wants to freeze/unfreeze 
				zombiesFrozen = TRUE; //set to true when 'F' has been pressed once
			else
				zombiesFrozen = FALSE; //set to false then user want's to unfreeze
		}
		else if (wantToExterminateZombies(key)) //if key pressed is 'X'
		{
			zombiesExterminateCount++; //count how amny times zombies have been frozen
			if ((zombiesExterminateCount % 2) == 0) // check if user wants to exterminate or un-exterminate
				wantToExterminate = TRUE; //set to true when 'X' has been pressed once
			else
				wantToExterminate = FALSE; //set to false when user want's to un-exterminate the zombies
			updateGame(grid, spot, key, message, holes, pills, lives, countPills, zombies, zombiesFrozen, wantToExterminate, exterminated); //call function to remove/add zombies back to grid
		}
		else if (wantToEat(key)) //if key pressed is 'E'
		{
			for (int count = 0; count < 8; count++) //loop through each of the pils
				pills.at(count).destroyed = true; //destroy a pill from items
			countPills = 0; //set to 0 as all pills now destroyed
			updateGame(grid, spot, key, message, holes, pills, lives, countPills, zombies, zombiesFrozen, wantToExterminate, exterminated); //call function to remove pills from grid
		}
		else
			message = "INVALID KEY!        "; //set 'Invalid key' message
		
		
		cout << ("Lives: ") << lives << "   "; //output remaining lives to user
		cout << ("Pills: ") << countPills; //output remaining pills to user

	} while (!wantToQuit(key) && lives > 0 && zombiesRemain(zombies) == true); //while user has lives, doesn't want to quit or zombies still in the grid
	if (lives <= 0)	//if no more lives left...
	{
		doScoreStuff(playerName, lives); //check and save high score to file if needed
		noLivesLeft(); //quit the game and display relevant message
	}
	else if (wantToQuit(key)) //if user wants to quit...
	{
		doScoreStuff(playerName, lives); //check and save high score to file if needed
		quitProgram(); //quit the game and display relevant message
	}
	else if (zombiesRemain(zombies) == false) //if no more zombies in the grid...
	{
		updateGame(grid, spot, key, message, holes, pills, lives, countPills, zombies, zombiesFrozen, wantToExterminate, exterminated);
		doScoreStuff(playerName, lives); //check and save high score to file if needed
		noZombiesLeft(countPills); //quit the game and display relevant message
	}

	
}

bool zombiesRemain(vector<Item> zombies) //checks that there are still zombies in the grid
{
	if (zombies.at(0).destroyed == false || zombies.at(1).destroyed == false || zombies.at(2).destroyed == false || zombies.at(3).destroyed == false)
		return true; //if at least one zombie hasnt been destroyed, then zombiesRemain is true
	else
		return false; //if all zombies are destroyed, then zombiesRemain is false
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

void updateGame(char grid[][SIZEX], Item& spot, int key, string& message, vector<Item> &holes, vector<Item> &pills, int& lives, int& countPills, vector<Item> &zombies, bool zombiesFrozen, bool wantToExterminate, bool &exterminated)
{ //updateGame state
	void updateSpotCoordinates(const char g[][SIZEX], Item& spot, int key, string& mess, int& lives, int& countPills, vector<Item>& pills);
	void updateGrid(char g[][SIZEX], Item spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies, int key, bool zombiesFrozen,int &lives, bool wantToExterminate, bool &exterminated);

	updateSpotCoordinates(grid, spot, key, message, lives, countPills, pills);    //update spot coordinates
	//according to key
	updateGrid(grid, spot, holes, pills, zombies, key, zombiesFrozen, lives, wantToExterminate, exterminated);                             //update grid information
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
		if ((zombie.destroyed == false) && (zombie.exterminated == false))	//checks that the zombies haven't been exterminated or destroyed permanently
		{ 
			int x, y;
			y = zombie.y;
			x = zombie.x;
			grid[y][x] = ZOMBIE; //places a zombie symbol in the coordinates that have been assigned to the zombie
		}
	}
}

void generateZombies(vector<Item> &zombies){

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
			y = (SIZEY - 2);
			break;
		case 2:
			x = (SIZEX - 2);
			y = 1;
			break;
		case 3:
			x = (SIZEX - 2);
			y = (SIZEY - 2);
			break;
		}
		Item zombie = { ZOMBIE, x, y};
		zombies.push_back(zombie);
	}
}

void moveZombies(char grid[][SIZEX], vector<Item> &zombies, Item spot, int key,int &lives)
{
	int count = 0;
	for (Item zombie : zombies)
	{
		if (zombie.destroyed == false){
			int random = Random(3);
			int move;
			int origX = zombie.x;
			int origY = zombie.y;
			if (zombie.x >= spot.x && zombie.y >= spot.y){
				switch (random){
					case 1:
						zombie.x--;
						break;
					case 2:
						zombie.x--;
						zombie.y--;
						break;
					case 3:
						zombie.y--;
						break;		
				}
			}
			else if (zombie.x <= spot.x && zombie.y >= spot.y){
				switch (random){
				case 1:
					zombie.x++;
					break;
				case 2:
					zombie.x++;
					zombie.y--;
					break;
				case 3:
					zombie.y--;
					break;
				}

			}
			else if (zombie.x >= spot.x && zombie.y <= spot.y){
				switch (random){
				case 1:
					zombie.x--;
					break;
				case 2:
					zombie.x--;
					zombie.y++;
					break;
				case 3:
					zombie.y++;
					break;
				}
			}
			else if (zombie.x <= spot.x && zombie.y <= spot.y){
				switch (random){
				case 1:
					zombie.x++;
					break;
				case 2:
					zombie.x++;
					zombie.y++;
					break;
				case 3:
					zombie.y++;
					break;
				}
			}
			
			switch (grid[zombie.y][zombie.x])
			{		//...depending on what's on the target position in grid...
			case HOLE:
				zombies.at(count).destroyed = true;
				break;
			case WALL:
				zombie.x = origX;
				zombie.y = origY;
				break;
			case SPOT:
				lives--;

				switch (count)
				{
				case 0:
					zombie.x = 1;
					zombie.y = 1;

					break;
				case 1:
					zombie.x = 1;
					zombie.y = (SIZEY - 2);
					break;
				case 2:
					zombie.x = (SIZEX - 2);
					zombie.y = 1;
					break;
				case 3:
					zombie.x = (SIZEX - 2);
					zombie.y = (SIZEY - 2);
					break;
				}
				break;
			}
			zombies.at(count).x = zombie.x;
			zombies.at(count).y = zombie.y;
		}
		count++;
	}
	for (int x = 0; x < 4; x++){
		for (int zom = 0; zom < 4; zom++){
			if (zombies.at(x).x == zombies.at(zom).x && zombies.at(x).y == zombies.at(zom).y && x != zom && zombies.at(x).destroyed == false && zombies.at(zom).destroyed == false){
				cout << "\a";
	
				switch (zom)
				{
				case 0:
					zombies.at(zom).x = 1;
					zombies.at(zom).y = 1;

					break;
				case 1:
					zombies.at(zom).x = 1;
					zombies.at(zom).y = (SIZEY - 2);
					break;
				case 2:
					zombies.at(zom).x = (SIZEX- 2);
					zombies.at(zom).y = 1;
					break;
				case 3:
					zombies.at(zom).x = (SIZEX - 2);
					zombies.at(zom).y = (SIZEY - 2);
					break;
				}
		
				switch (x)
				{
				case 0:
					zombies.at(x).x = 1;
					zombies.at(x).y = 1;

					break;
				case 1:
					zombies.at(x).x = 1;
					zombies.at(x).y = (SIZEY - 2);
					break;
				case 2:
					zombies.at(x).x = (SIZEX - 2);
					zombies.at(x).y = 1;
					break;
				case 3:
					zombies.at(x).x = (SIZEX - 2);
					zombies.at(x).y = (SIZEY - 2);
					break;
				}
			}
		}
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
		if (zombies.at(count).x == x && zombies.at(count).y == y){
			isValid = false;
		}
	}
	return isValid;
}

void placePills(char grid[][SIZEX], vector<Item> pills){
	for (Item pill : pills){
		if (pill.destroyed == false){
			int x, y;
			y = pill.y;
			x = pill.x;
			//grid[row][col]

			grid[y][x] = PILL;
		}
	}

}

void generatePills(vector<Item> &pills, Item spot, vector<Item> holes, vector<Item> zombies){
	bool checkPillCoords(int, int, vector<Item>, vector<Item>, vector<Item>);
	Seed();
	for (int count = 0; count < 8; count++){
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
		if (zombies.at(count).x == x && zombies.at(count).y == y){
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
	spot.y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 3)]
	spot.x = Random(SIZEX - 2);    //horizontal coordinate in range [1..(SIZEX - 3)]
	if (spot.x == 0)	//checks that spot is not placed in a wall
		spot.x = spot.x + 1;	

	if (spot.y == 0)	//checks that spot is not placed in a wall
		spot.y = spot.y + 1;

	if (spot.x == 1) //checks that spot is not in a zombie position
		spot.x = spot.x + 1;

	if (spot.y == 1) //checks that spot is not in a zombie position
		spot.y = spot.y + 1;
	
		
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

void updateGrid(char grid[][SIZEX], Item spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies, int key, bool zombiesFrozen,int& lives, bool wantToExterminate, bool &exterminated)
{ //update grid configuration after each move
	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeHoles(char[][SIZEX], vector<Item> holes);
	void placePills(char[][SIZEX], vector<Item> pills);
	void placeZombies(char[][SIZEX], vector<Item> zombies);
	void moveZombies(char[][SIZEX], vector<Item> &zombies, Item spot, int key,int& lives);
	
	setGrid(grid);	         //reset empty grid
	placeHoles(grid, holes); //set holes in grid
	placeSpot(grid, spot);	 //set spot in grid
	placePills(grid, pills); //set pills in grid
	if (zombiesFrozen == FALSE){
		moveZombies(grid, zombies, spot, key,lives);
	}
	if (wantToExterminate == TRUE && exterminated == false){
		zombies.at(0).exterminated = true;
		zombies.at(1).exterminated = true;
		zombies.at(2).exterminated = true;
		zombies.at(3).exterminated = true;
		exterminated = TRUE;
		
	}
	else if(wantToExterminate == false && exterminated == true){
		zombies.at(0).exterminated = false;
		zombies.at(1).exterminated = false;
		zombies.at(2).exterminated = false;
		zombies.at(3).exterminated = false;
		exterminated = false;
		for (int count = 0; count < 4; count++){
			switch (count)
			{
			case 0:
				zombies.at(count).x = 1;
				zombies.at(count).y = 1;

				break;
			case 1:
				zombies.at(count).x = 1;
				zombies.at(count).y = (SIZEY - 2);
				break;
			case 2:
				zombies.at(count).x = (SIZEX - 2);
				zombies.at(count).y = 1;
				break;
			case 3:
				zombies.at(count).x = (SIZEX - 2);
				zombies.at(count).y = (SIZEY - 2);
				break;
			}
		}
		
	}

	placeZombies(grid, zombies);  //set zombies in grid

} //end of updateGrid

//---------------------------------------------------------------------------
//----- move the spot
//---------------------------------------------------------------------------
void updateSpotCoordinates(const char g[][SIZEX], Item& sp, int key, string& mess, int& lives, int& countPills, vector<Item> &pills)
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
	case PILL:
		sp.y += dy;
		sp.x += dx;
		for (int counter = 0; counter < 8; counter++){
			if (pills.at(counter).x == sp.x && pills.at(counter).y == sp.y){
				pills.at(counter).destroyed = true;
				lives++;
				countPills--;
			}
			
		}
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
	keyPressed = toupper(getch());      //read in the selected arrow key or command letter
	while (keyPressed == 224)     //ignore symbol following cursor key
		keyPressed = toupper(getch());
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

bool wantInformation(int key)
{
	//check if key 'I' is pressed
	return (key == INFO);
}
bool wantToFreeze(int key)
{
	//check if key 'F' is pressed
	return (key == FREEZE);
}

bool wantToExterminateZombies(int key)
{
	//check if key 'X' is pressed
	return (key == EXTERMINATE);
}

bool wantToEat(int key)
{
	//check if key 'E' is pressed
	return (key == EAT);
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

void renderGame(const char gd[][SIZEX], string mess, string playerName)
{ //display game title, messages, maze, spot and apples on screen
	void paintGrid(const char g[][SIZEX]);
	void showTitle(string);
	void showOptions();
	void showMessage(string);

	Gotoxy(0, 0);
	//display grid contents
	paintGrid(gd);
	//display game title
	showTitle(playerName);
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

void showTitle(string playerName)
{ //display game title
	int getHighScore(string playerName);
	SelectTextColour(clYellow);
	Gotoxy(0, 0);
	cout << "___ZOMBIES GAME SKELETON___\n" << endl;
	SelectBackColour(clWhite);
	SelectTextColour(clRed);
	Gotoxy(40, 0);
	cout << playerName << "'s high score is: " << getHighScore(playerName);
	Gotoxy(40, 1);
	cout << displayTime();

} //end of showTitle

int getHighScore(string playerName)
{
	ifstream file(playerName + ".txt");
	if (!file)	//if file doesnt exist, player hasnt played before, so no previous high score
	{
		return -1;
	}
	else  //if file does exist, read contents of file
	{
		string sScore;
		getline(file, sScore);
		return stoi(sScore);
	}
}

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

void quitProgram()
{ //end program with appropriate message
	SelectBackColour(clBlack);
	SelectTextColour(clYellow);
	Gotoxy(40, 10);
	cout << "YOU QUIT!             ";
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 11);
	//cin.clear();
	system("pause");
} //end of quitProgram

void noLivesLeft()
{ //end program with appropriate message
	SelectBackColour(clBlack);
	SelectTextColour(clYellow);
	Gotoxy(40, 10);
	cout << "YOU LOST!             ";
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 11);
	//cin.clear();
	system("pause");
} //end of noLivesLeft

void noZombiesLeft(int countPills)
{ //end program with appropriate message
	SelectBackColour(clBlack);
	SelectTextColour(clYellow);
	Gotoxy(40, 10);
	cout << "YOU WON WITH " << countPills << " pills remaining!";
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 11);
	//cin.clear();
	system("pause");
} //end of noZombiesLeft