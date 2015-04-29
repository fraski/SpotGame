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
const char TELEPORT('O');   //teleport
//defining the command letters to move the blob on the maze
const int  UP(72);           //up arrow
const int  DOWN(80);         //down arrow
const int  RIGHT(77);        //right arrow
const int  LEFT(75);         //left arrow
//defining the other command letters
const char QUIT('Q');        //end the game
const char PLAY('P');		//play the game
const char INFO('I');		//get info about the game
const char REPLAY('R');     //replay recent moves
const char SAVE('S');
const char LOAD('L');
//defining the cheats
const char FREEZE('F');		//freeze game
const char EXTERMINATE('X');	//exterminate remaining zombies
const char EAT('E');		//eat all remaining pills
//data structure to store data for a grid item
struct Item{
	const char symbol;	     //symbol on grid
	int x, y;			     //coordinates
	bool destroyed, exterminated, protectionOn; //boolean var's for cheat enabling
	int protectionCount;
	vector<int> historyX;
	vector<int> historyY;
	vector<int> historyState;
};


//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------
//function declarations (prototypes)
void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies, int countPills, int noOfHoles, Item& teleport);
bool isArrowKey(int k);
void updateGame(char g[][SIZEX], Item& sp, int k, string& mess, vector<Item> &holes, vector<Item> &pills, int& lives, int& countPills, vector<Item> &zombies, bool zombiesFrozen, bool wantToExterminate, bool &exterminated, int noOfHoles, int noOfPills, Item teleport);
void saveGame( Item sp, vector<Item> holes, vector<Item> pills, vector<Item> zombies, Item teleport,string playerName,int noOfHoles,int noOfPills,int lives,int levelNo);
void loadGame(char g[][SIZEX],Item& sp, vector<Item>& holes, vector<Item>& pills, vector<Item>& zombies, Item& teleport, string playerName, int& noOfHoles, int& noOfPills, int& lives, int& levelNo);
void renderGame(const char g[][SIZEX], string mess, string playerName, Item Spot);
void quitProgram();
void noLivesLeft();
void noZombiesLeft(int countPills, int& levelNo, string playerName);
void gameEntry();
void infoScreen();
int  getKeyPress();
bool wantToQuit(int k);
bool wantToFreeze(int f);
bool wantToEat(int e);
bool wantToReplay(int r);
bool wantToExterminateZombies(int x);
bool wantToSave(int s);
bool wantToLoad(int l);
void enterGame(string playerName, int levelNo);
bool haveSaveFile(string playerName);
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
	cout << "Press 'P' to play game \n" << "Press 'Q' to quit \n" << "Press 'I' for more information";
	int key(' '); //create key to store keyboard events
	int levelNo;	//current level number
	string playerName; //define playerName as string
	while (wantToQuit(key) == false && wantToPlay(key) == false && wantInformation(key) == false) //runs loop until user presses a key (p, q or i)
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
			} while ((playerName.length() < 1) || (playerName.length() > 20) || (checkForSpaces(playerName) == true)); //run loop until user enters a valid name (less than 20, no spaces)
			Clrscr(); //clear console window
			do {
				cout << "Which level would you like to start on?\n\n";
				cout << "Press '1' for Easy! (12 holes, 8 lives, 8 power pills)\n";
				cout << "Press '2' for Medium! (5 holes, 5 lives, 5 power pills)\n";
				cout << "Press '3' for Hard! (2 holes, 3 lives, 2 power pills)\n";
				cin >> levelNo;
			} while ((levelNo != 1) && (levelNo != 2) && (levelNo != 3));
			Clrscr(); //clear console window
			enterGame(playerName, levelNo); //pass playername into function by value
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

void enterGame(string playerName, int levelNo) //console screen where you play the game
{   //local variable declarations 
	
	char grid[SIZEY][SIZEX];                //grid for display
	Item spot = { SPOT };                   //Spot's symbol and position (0, 0) 
	Item teleport = { TELEPORT };
	spot.protectionOn = false;
	int lives, zombiesFreezeCount = 1, zombiesExterminateCount = 1, countPills, noOfHoles; //initialising variables
	string message("LET'S START...      "); //current message to player
	bool replay = false;
	time_t timer;
	time(&timer);
	double duration = 0;
	switch (levelNo) //switch statement holding 3 possible levels user can use
	{
	case 1: //if user presses 1
		noOfHoles = 12;
		countPills = 8;
		lives = 8;
		break;
	case 2: //if user presses 2
		noOfHoles = 5;
		countPills = 5;
		lives = 5;
		break;
	case 3: //if user presses 3
		noOfHoles = 2;
		countPills = 2;
		lives = 3;
		break; 
	}

	int noOfPills = countPills; //set pill count after level has been chosen

	//create vectors
	vector<Item> holes;
	vector<Item> pills;
	vector<Item> zombies;

	void doScoreStuff(string, int); //call function which calculates score for current game 

	void replayGame(Item spot, vector<Item> zombies, vector<Item> pills, vector<Item> holes, int noOfPills, int noOfHoles, Item teleport); //call function which will replay the game upon user key press

	bool zombiesRemain(vector<Item> zombies); //function prototype
	
	
	int key(' ');      //create key to store keyboard events
	if (haveSaveFile(playerName)){
		cout << "Do you wish to load your previous game? Y/N";
		do{
			key = getKeyPress();
			} while (key  != 'Y' && key  != 'N');
		if (key == 'Y'){
			loadGame(grid,spot, holes, pills, zombies, teleport, playerName, noOfHoles, noOfPills, lives, levelNo);
		}
		else{
			initialiseGame(grid, spot, holes, pills, zombies, noOfHoles, countPills, teleport);
		}
	}
	else{
		initialiseGame(grid, spot, holes, pills, zombies, noOfHoles, countPills, teleport);           //initialise grid (incl. walls and spot)
	}

	key = ' ';
	bool zombiesFrozen = FALSE, wantToExterminate = FALSE, exterminated = FALSE; //initialise local variables
	do {
		renderGame(grid, message, playerName, spot);        //render game state on screen
		message = "                    "; //reset message
		key = getKeyPress();
		if (isArrowKey(key)){ //checks if key is one of the arrow keys
			updateGame(grid, spot, key, message, holes, pills, lives, countPills, zombies, zombiesFrozen, wantToExterminate, exterminated, noOfHoles, noOfPills, teleport); //calls funciton which will change spots position on the grid
		}
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
			updateGame(grid, spot, key, message, holes, pills, lives, countPills, zombies, zombiesFrozen, wantToExterminate, exterminated, noOfHoles, noOfPills, teleport); //call function to remove/add zombies back to grid
		}
		else if (wantToEat(key)) //if key pressed is 'E'
		{
			for (int count = 0; count < 8; count++) //loop through each of the pils
				pills.at(count).destroyed = true; //destroy a pill from items
			countPills = 0; //set to 0 as all pills now destroyed
			updateGame(grid, spot, key, message, holes, pills, lives, countPills, zombies, zombiesFrozen, wantToExterminate, exterminated, noOfHoles, noOfPills, teleport); //call function to remove pills from grid
			
		}
		else if (wantToReplay(key)){ //if key pressed is 'R'
			replayGame(spot, zombies, pills, holes, noOfPills, noOfHoles, teleport); //call function to replay game
		}
		else if (wantToSave(key)){
			saveGame(spot,holes,pills,zombies,teleport,playerName,noOfHoles,noOfPills,lives,levelNo);
		}
		else if (key == -1){

		}
		else
			message = "INVALID KEY!        "; //set 'Invalid key' message


		time_t current;
		time(&current);
		cout << ("Lives: ") << lives << "   "; //output remaining lives to user
		cout << ("Pills: ") << countPills; //output remaining pills to user
		Gotoxy(40, 15);
		cout << ("Time: ") << (int)difftime(timer,current)*-1;
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
		updateGame(grid, spot, key, message, holes, pills, lives, countPills, zombies, zombiesFrozen, wantToExterminate, exterminated, noOfHoles, noOfPills, teleport);
		doScoreStuff(playerName, lives); //check and save high score to file if needed
		noZombiesLeft(countPills, levelNo, playerName); //quit the game and display relevant message
	}


}
bool haveSaveFile(string playerName){
	bool saveFile = false;
	ifstream file(playerName + "savefile.txt");
	if (file){
		saveFile = true;
	}
	return saveFile;
}
void loadGame(char grid[][SIZEX],Item& sp, vector<Item>& holes, vector<Item>& pills, vector<Item>& zombies, Item& teleport, string playerName, int& noOfHoles, int& noOfPills, int& lives, int& levelNo){
	void setGrid(char[][SIZEX]);
	void placeZombies(char gr[][SIZEX], vector<Item> zombies);
	void placeSpot(char gr[][SIZEX], Item spot);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);
	void placePills(char gr[][SIZEX], vector<Item> pills);
	void placeTeleport(char gr[][SIZEX], Item teleport);
	ifstream file(playerName + "savefile.txt");
	string line;
	getline(file, line);
	int x = 0;
	int z = 0;
	string item = "";
	//sort spot
	while (line[x] != NULL){
		while (line[x] != '#' && line[x] != NULL){
			item += line[x];
			x++;
		}
			switch (z){
			case 0:
				sp.x = stoi(item);
				item = "";
				z++;
				break;
			case 1:
				sp.y = stoi(item);
				item = "";
				z++;
				break;
			case 2:
				sp.protectionOn = stoi(item);
				item = "";
				z++;
				break;
			case 3:
				sp.protectionCount = stoi(item);
				item = "";
				z++;
				break;
			case 4:
				lives = stoi(item);
				item = "";
				z++;
				break;
			case 5:
				levelNo = stoi(item);
				item = "";
				z++;
				break;
			
		}
		x++;
	}
	x = 0;
	getline(file, line);
	//sort holes
	while (line[x] != NULL){
		Item hole{ '0' };
		for (int z = 0; z < 3; z++){
			while (line[x] != '#' && line[x] != NULL){
				item += line[x];
				x++;
			}
			switch (z){
			case 0:
				hole.x = stoi(item);
				item = "";
				x++;
				break;
			case 1:
				hole.y = stoi(item);
				item = "";
				x++;
				break;
			case 2:
				hole.destroyed = stoi(item);
				item = "";
				x++;
				break;
			}

		}
		holes.push_back(hole);
	}
	x = 0;
	getline(file, line);
	//sort pills
	while (line[x] != NULL){
		Item pill{ '*' };
		for (int z = 0; z < 3; z++){
			while (line[x] != '#' && line[x] != NULL){
				item += line[x];
				x++;
			}
			switch (z){
			case 0:
				pill.x = stoi(item);
				item = "";
				x++;
				break;
			case 1:
				pill.y = stoi(item);
				item = "";
				x++;
				break;
			case 2:
				pill.destroyed = stoi(item);
				item = "";
				x++;
				break;
			}

		}
		pills.push_back(pill);
	}
	x = 0;
	getline(file, line);
	//sort zombies
	while (line[x] != NULL){
		Item zombie{ 'Z' };
		for (int z = 0; z < 3; z++){
			while (line[x] != '#' && line[x] != NULL){
				item += line[x];
				x++;
			}
			switch (z){
			case 0:
				zombie.x = stoi(item);
				item = "";
				x++;
				break;
			case 1:
				zombie.y = stoi(item);
				item = "";
				x++;
				break;
			case 2:
				zombie.destroyed = stoi(item);
				item = "";
				x++;
				break;
			}

		}
		zombies.push_back(zombie);
		
	}
	x = 0;
	z = 0;
	getline(file, line);
	while (line[x] != NULL){
		while (line[x] != '#' && line[x] != NULL){
			item += line[x];
			x++;
		}
		switch (z){
		case 0:
			teleport.x = stoi(item);
			item = "";
			z++;
			x++;
			break;
		case 1:
			teleport.y = stoi(item);
			item = "";
			z++;
			x++;
			break;
		}
		cout << "stuck";
	}
	setGrid(grid);
    //set spot in grid
	placeSpot(grid, sp);         
	placeTeleport(grid, teleport);
	placeZombies(grid, zombies);
	//generate and then place zombies
	placeHoles(grid, holes);
	//generate and then place holes
	placePills(grid, pills);
	//generate and then place pills


}
void saveGame(Item sp, vector<Item> holes, vector<Item> pills, vector<Item> zombies, Item teleport, string playerName, int noOfHoles, int noOfPills, int lives,int levelNo){
	ofstream file(playerName + "savefile.txt");
	if (file){
		//first line should be spot, etc etc
		file << sp.x << "#";
		file << sp.y << "#";
		file << sp.protectionOn << "#";
		file << sp.protectionCount << "#";
		file << lives<< "#" <<levelNo <<"#"<< endl;
		for (int x = 0; x < noOfHoles; x++){
			file << holes.at(x).x << "#" << holes.at(x).y << "#" << holes.at(x).destroyed << "#";
		}
		file << endl;
		for (int x = 0; x < noOfPills; x++){
			file << pills.at(x).x << "#" << pills.at(x).y << "#" << pills.at(x).destroyed << "#";
		}
		file << endl;
		for (int x = 0; x < 4; x++){
			file << zombies.at(x).x << "#" << zombies.at(x).y << "#" << zombies.at(x).destroyed << "#";
		}
		file << endl;
		file << teleport.x << "#";
		file << teleport.y << "#" << endl;
	}
}
void replayGame(Item spot, vector<Item> zombies, vector<Item> pills, vector<Item> holes, int noOfPills, int noOfHoles, Item teleport){
	int turn = 0;
	Clrscr();
	char grid[SIZEY][SIZEX];
	void setGrid(char[][SIZEX]);
	void paintGrid(const char g[][SIZEX], bool magicProtection);
	bool magicProtection = false;
	int magicCounter = 0;
	cout << spot.historyX.size();
	while (turn < spot.historyX.size()){
	
		setGrid(grid);

		for (int x = 0; x < noOfHoles; x++){
			grid[holes.at(x).historyY.at(0)][holes.at(x).historyX.at(0)] = HOLE;
		}
		for (int x = 0; x < noOfPills; x++){
			if (pills.at(x).historyState.at(turn) == 0){
				grid[pills.at(x).historyY.at(0)][pills.at(x).historyX.at(0)] = PILL;
			}
			else{
				if (turn > 0){
					if (pills.at(x).historyState.at(turn-1) == 0){
						magicProtection = true;
						magicCounter = 10;
					}
				}
				grid[pills.at(x).historyY.at(0)][pills.at(x).historyX.at(0)] = TUNNEL;
			}
		}
		for (int x = 0; x < 4; x++){
			if (zombies.at(x).historyState.at(turn) == 0){
				grid[zombies.at(x).historyY.at(turn)][zombies.at(x).historyX.at(turn)] = ZOMBIE;
			}
		}
		if (spot.historyState.at(turn) == 0){
			grid[spot.historyY.at(turn)][spot.historyX.at(turn)] = SPOT;
		}

		grid[teleport.historyY.at(0)][teleport.historyX.at(0)] = TELEPORT;

		if (magicCounter > 0){
			magicCounter--;
		}

		paintGrid(grid, magicProtection);
		Sleep(500);
		cout << magicCounter << endl;
		turn++;
		
		if (magicCounter == 0){
			magicProtection = false;
		}
	}
}

bool zombiesRemain(vector<Item> zombies) //checks that there are still zombies in the grid
{
	if (zombies.at(0).destroyed == false || zombies.at(1).destroyed == false || zombies.at(2).destroyed == false || zombies.at(3).destroyed == false)
		return true; //if at least one zombie hasnt been destroyed, then zombiesRemain is true
	else
		return false; //if all zombies are destroyed, then zombiesRemain is false
}

const string displayTime() //function called when time needs to be displayed
{
	time_t     now = time(0); //time type
	struct tm  tstruct; //time structure initialised
	char       buf[80];  //initialising buffer
	tstruct = *localtime(&now); //get local time and put into tstruct
	strftime(buf, sizeof(buf), "%d-%m-%Y.%X", &tstruct); //format date as string
	return buf; //return buffer
}

void updateGame(char grid[][SIZEX], Item& spot, int key, string& message, vector<Item> &holes, vector<Item> &pills, int& lives, int& countPills, vector<Item> &zombies, bool zombiesFrozen, bool wantToExterminate, bool &exterminated, int noOfHoles, int noOfPills, Item teleport)
{ //updateGame state
	void updateSpotCoordinates(char g[][SIZEX], Item& spot, int key, string& mess, int& lives, int& countPills, vector<Item>& pills, int noOfPills);
	void updateGrid(char g[][SIZEX], Item spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies, int key, bool zombiesFrozen, int &lives, bool wantToExterminate, bool &exterminated, int noOfHoles, int countPills, Item teleport);

	updateSpotCoordinates(grid, spot, key, message, lives, countPills, pills, noOfPills);    //update spot coordinates
	//according to key
	updateGrid(grid, spot, holes, pills, zombies, key, zombiesFrozen, lives, wantToExterminate, exterminated, noOfHoles, countPills, teleport);                             //update grid information
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies, int noOfHoles, int countPills, Item& teleport)
{ //initialise grid and place spot in middle
	void setGrid(char[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot, char grid[][SIZEX]);

	void generateZombies(vector<Item> &zombie);
	void placeZombies(char gr[][SIZEX], vector<Item> zombies);

	void placeSpot(char gr[][SIZEX], Item spot);

	void generateHoles(vector<Item> &holes, Item spot, vector<Item> zombies, char gr[][SIZEX], int noOfHoles, Item teleport);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);

	void generatePills(vector<Item> &pills, Item spot, vector<Item> holes, vector<Item> zombie, char gr[][SIZEX], int countPills, Item teleport);
	void placePills(char gr[][SIZEX], vector<Item> pills);

	void generateTeleport(Item spot, char gr[][SIZEX], Item& teleport);
	void placeTeleport(char gr[][SIZEX], Item teleport);


	Seed();                            //seed random number generator
	setSpotInitialCoordinates(spot, grid);   //initialise spot position
	setGrid(grid);                     //reset empty grid
	placeSpot(grid, spot);             //set spot in grid
	generateTeleport(spot, grid, teleport);
	placeTeleport(grid, teleport);
	generateZombies(zombies);
	placeZombies(grid, zombies);
	//generate and then place zombies

	generateHoles(holes, spot, zombies, grid, noOfHoles, teleport);
	placeHoles(grid, holes);
	//generate and then place holes

	generatePills(pills, spot, holes, zombies, grid, countPills, teleport);
	placePills(grid, pills);
	//generate and then place pills

} //end of initialiseGame

void generateTeleport(Item spot, char grid[][SIZEX], Item& teleport)
{
	Seed();
	do
	{
		teleport.y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
		teleport.x = Random(SIZEX - 2);    //horizontal coordinate in range [1..(SIZEX - 2)]
	} while (grid[teleport.y][teleport.x] == SPOT || grid[teleport.y][teleport.x] == WALL);
	teleport.historyX.push_back(teleport.x);
	teleport.historyY.push_back(teleport.y);
	teleport.historyState.push_back(0);
}

void placeTeleport(char grid[][SIZEX], Item teleport)
{
	int x, y;
	y = teleport.y;
	x = teleport.x;
	//grid[row][col]

	grid[y][x] = TELEPORT;
}


void placeWalls(char grid[][SIZEX]) //find a more efficient way to do this??
{

	//place inner walls to coordinates on grid
	grid[3][4] = WALL;
	grid[3][5] = WALL;
	grid[4][3] = WALL;
	grid[3][6] = WALL;
	grid[3][3] = WALL;
	grid[5][3] = WALL;

	grid[3][15] = WALL;
	grid[3][14] = WALL;
	grid[4][16] = WALL;
	grid[3][13] = WALL;
	grid[3][16] = WALL;
	grid[5][16] = WALL;

	grid[9][4] = WALL;
	grid[9][5] = WALL;
	grid[8][3] = WALL;
	grid[9][6] = WALL;
	grid[9][3] = WALL;
	grid[7][3] = WALL;

	grid[9][15] = WALL;
	grid[9][14] = WALL;
	grid[8][16] = WALL;
	grid[9][16] = WALL;
	grid[9][13] = WALL;
	grid[7][16] = WALL;

}

void placeZombies(char grid[][SIZEX], vector<Item> zombies){
	for (Item zombie : zombies){ //
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
		Item zombie = { ZOMBIE, x, y };
		zombie.historyX.push_back(x);
		zombie.historyY.push_back(y);
		zombie.historyState.push_back(0);
		zombies.push_back(zombie);
	}
}

void moveZombies(char grid[][SIZEX], vector<Item> &zombies, Item spot, int key, int& lives, int noOfHoles, vector<Item> pills, int countPills, Item teleport, vector<Item> holes)
{
	int count = 0; //initialise to 0
	for (Item zombie : zombies) //for every zombie existing in 'zombies', put their values into zombies
	{
		if (zombie.destroyed == true){ //if zombie has been hit by spot
			zombies.at(count).historyState.push_back(1); //return zombie to original starting point
		
		}else{ //if zombie has not been hit by spot
			int random = Random(3); //initialise 3 random moves for zombie to take
			int move;
			int origX = zombie.x; //put zombie's current x value into variable
			int origY = zombie.y;//put zombie's current y value into variable
			int dx = 0, dy = 0;
			if (spot.y == zombie.y && spot.x == zombie.x && spot.protectionOn == true){ //({		//if zombies coordinates match spots (collision) and magic protection is on
				zombies.at(count).destroyed = true; //destroy that current zombie which collided
				Beep(500, 300); //make noise

			}

			//make zombies run away from spot when magic protection is on
			if (zombie.x >= spot.x && zombie.y >= spot.y){		//condense this down??   
				switch (random){
				case 1:
					dx--;
					break;
				case 2:
					dx--;
					dy--;
					break;
				case 3:
					dy--;
					break;
				}
			}
			else if (zombie.x <= spot.x && zombie.y >= spot.y){
				switch (random){
				case 1:
					dx++;
					break;
				case 2:
					dx++;
					dy--;
					break;
				case 3:
					dy--;
					break;
				}

			}
			else if (zombie.x >= spot.x && zombie.y <= spot.y){
				switch (random){
				case 1:
					dx--;
					break;
				case 2:
					dx--;
					dy++;
					break;
				case 3:
					dy++;
					break;
				}
			}
			else if (zombie.x <= spot.x && zombie.y <= spot.y){
				switch (random){
				case 1:
					dx++;
					break;
				case 2:
					dx++;
					dy++;
					break;
				case 3:
					dy++;
					break;
				}
			}

			//not quite sure what this is doing...
			if (spot.protectionOn == false)
			{
				zombie.x += dx;
				zombie.y += dy;
			}
			else
			{
				zombie.x -= dx;
				zombie.y -= dy;
			}


			switch (grid[zombie.y][zombie.x])
			{		//...depending on what's on the target position in grid...
			case TELEPORT:
				bool checkPillCoords(int, int, vector<Item>, vector<Item>, vector<Item>, char gr[][SIZEX], Item);
				Seed();
					int x, y;
					do{
						do {
							x = (Random(SIZEX - 2));
							y = (Random(SIZEY - 2));
						} while (spot.x == x && spot.y == y);
						zombie.x = x;
						zombie.y = y;
					} while (!checkPillCoords(x, y, pills, holes, zombies, grid, teleport));
				break;
			case HOLE:
				zombies.at(count).destroyed = true; //destroy zombies when collide with a hole
				break;
			case WALL:
				//zombie doesn't move - stays at current position if collision with wall
				zombie.x = origX;
				zombie.y = origY;
				break;
			case SPOT:
				if (spot.protectionOn == false){ //when spot doesn't have magic protection
					lives--; //decrease life amount

					switch (count) //current zombie, place back at original spot
					{
					case 0: //top left corner
						zombie.x = 1;
						zombie.y = 1;

						break;
					case 1: //bottom left corner
						zombie.x = 1;
						zombie.y = (SIZEY - 2);
						break;
					case 2: //top right corner
						zombie.x = (SIZEX - 2);
						zombie.y = 1;
						break;
					case 3: //bottom right corner
						zombie.x = (SIZEX - 2);
						zombie.y = (SIZEY - 2);
						break;
					}
				}
				
				break;
			}

			//not quite sure what this is doing either...
			zombies.at(count).x = zombie.x;
			zombies.at(count).y = zombie.y;
			zombies.at(count).historyX.push_back(zombie.x);
			zombies.at(count).historyY.push_back(zombie.y);
			zombies.at(count).historyState.push_back(0);
			
		}
		count++; //increment to go to next zombie
	
	}
	
	for (int x = 0; x < 4; x++){ //looping through 4 times 
		for (int zom = 0; zom < 4; zom++){ //looping through 4 times 
			if (zombies.at(x).x == zombies.at(zom).x && zombies.at(x).y == zombies.at(zom).y && x != zom && zombies.at(x).destroyed == false && zombies.at(zom).destroyed == false){ //checking if zombies collide
				cout << "\a"; //output error noise

				switch (zom) //reposition zombie which collided to original position
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
					zombies.at(zom).x = (SIZEX - 2);
					zombies.at(zom).y = 1;
					break;
				case 3:
					zombies.at(zom).x = (SIZEX - 2);
					zombies.at(zom).y = (SIZEY - 2);
					break;
				}

				switch (x) //reposition other zombie which collided to original position
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

void placeHoles(char grid[][SIZEX], vector<Item> holes){  //function to place holes
	for (Item hole : holes){ //for every existing hole in holes, put their values into 'holes'
		int x, y;
		y = hole.y;
		x = hole.x;
		grid[y][x] = HOLE; //randomly assign holes to coordinates on grid
	}

}

void generateHoles(vector<Item> &holes, Item spot, vector<Item> zombies, char grid[][SIZEX], int noOfHoles, Item teleport){ //function to create holes
	bool checkHoleCoords(int, int, vector<Item>, vector<Item>, char grid[][SIZEX], Item); //receive true/false value to make sure holes aren't placed on any existing items in grid
	Seed();  //seed random number generator
	for (int count = 0; count < noOfHoles; count++){ //loop through each hole
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
		} while (!checkHoleCoords(x, y, holes, zombies, grid, teleport)); //loop through whilst s
		Item hole = { HOLE, x, y };
		hole.historyX.push_back(x);
		hole.historyY.push_back(y);
		hole.historyState.push_back(0);
		holes.push_back(hole);
	}
	cout << "stop";
}

bool checkHoleCoords(int x, int y, vector<Item> holes, vector<Item> zombies, char gr[][SIZEX], Item teleport){
	bool isValid = true;

	if (teleport.x == x && teleport.y == y)
	{
		isValid = false;
	}
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

	if (gr[y][x] == WALL){
		isValid = false;
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

void generatePills(vector<Item> &pills, Item spot, vector<Item> holes, vector<Item> zombies, char grid[][SIZEX], int countPills, Item teleport){
	bool checkPillCoords(int, int, vector<Item>, vector<Item>, vector<Item>, char gr[][SIZEX], Item);
	Seed();
	for (int count = 0; count < countPills; count++){
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
		} while (!checkPillCoords(x, y, pills, holes, zombies, grid, teleport));
		Item pill = { PILL, x, y };
		pill.historyX.push_back(x);
		pill.historyY.push_back(y);
		pill.historyState.push_back(0);
		pills.push_back(pill);
	}
}
bool checkPillCoords(int x, int y, vector<Item> pills, vector<Item> holes, vector<Item> zombies, char gr[][SIZEX], Item teleport){
	bool isValid = true;

	if (teleport.x == x && teleport.y == y)
	{
		isValid = false;
	}
	
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
	if (gr[y][x] == WALL){
		isValid = false;
	}

	return isValid;
}

void setSpotInitialCoordinates(Item& spot, char grid[][SIZEX]) //STILL NEED TO FIX INNER WALLS!!!!!!!!!!!!!
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
	spot.historyX.push_back(spot.x);
	spot.historyY.push_back(spot.y);
	spot.historyState.push_back(0);

} //end of setSpotInitialoordinates

void setGrid(char grid[][SIZEX])
{ //reset the empty grid configuration
	void placeWalls(char gr[][SIZEX]);
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
	placeWalls(grid);
} //end of setGrid

void placeSpot(char gr[][SIZEX], Item spot)
{ //place spot at its new position in grid
	gr[spot.y][spot.x] = spot.symbol;
} //end of placeSpot

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], Item spot, vector<Item> &holes, vector<Item> &pills, vector<Item> &zombies, int key, bool zombiesFrozen, int& lives, bool wantToExterminate, bool &exterminated, int noOfHoles, int countPills, Item teleport)
{ //update grid configuration after each move
	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeHoles(char[][SIZEX], vector<Item> holes);
	void placePills(char[][SIZEX], vector<Item> pills);
	void placeZombies(char[][SIZEX], vector<Item> zombies);
	void moveZombies(char[][SIZEX], vector<Item> &zombies, Item spot, int key, int& lives, int noOfHoles, vector<Item> pills, int countPills, Item teleport, vector<Item> holes);
	void placeTeleport(char[][SIZEX], Item teleport);

	setGrid(grid);	         //reset empty grid
	placeHoles(grid, holes); //set holes in grid
	placeSpot(grid, spot);	 //set spot in grid	
	placePills(grid, pills); //set pills in grid
	placeTeleport(grid, teleport);
	if (zombiesFrozen == FALSE){
		moveZombies(grid, zombies, spot, key, lives, noOfHoles, pills, countPills, teleport, holes);
	}
	if (wantToExterminate == TRUE && exterminated == false){
		zombies.at(0).exterminated = true;
		zombies.at(1).exterminated = true;
		zombies.at(2).exterminated = true;
		zombies.at(3).exterminated = true;
		exterminated = TRUE;

	}
	else if (wantToExterminate == false && exterminated == true){
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
void updateSpotCoordinates(char g[][SIZEX], Item& sp, int key, string& mess, int& lives, int& countPills, vector<Item> &pills, int noOfPills)
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
	case TELEPORT:
		sp.y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 3)]
		sp.x = Random(SIZEX - 2);    //horizontal coordinate in range [1..(SIZEX - 3)]
		if (sp.x == 0)	//checks that spot is not placed in a wall
			sp.x = sp.x + 1;

		if (sp.y == 0)	//checks that spot is not placed in a wall
			sp.y = sp.y + 1;

		if (sp.x == 1) //checks that spot is not in a zombie position
			sp.x = sp.x + 1;

		if (sp.y == 1) //checks that spot is not in a zombie position
			sp.y = sp.y + 1;
		break;
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
		for (int counter = 0; counter < noOfPills; counter++){
			if (pills.at(counter).x == sp.x && pills.at(counter).y == sp.y){
				pills.at(counter).destroyed = true;
				lives++;
				countPills--;
				pills.at(counter).historyState.push_back(1);
			}
			else{
				pills.at(counter).historyState.push_back(0);

			}

		}
		sp.protectionOn = true;
		if (noOfPills == 8){
			sp.protectionCount = 10;
			
		}
		else if (noOfPills == 5){
			sp.protectionCount = 8;
		}
		else if (noOfPills == 2){
			sp.protectionCount = 5;
		}
	
		break;
	case ZOMBIE:
		if (sp.protectionOn == true){
			sp.y += dy;   //go in that Y direction
			sp.x += dx;   //go in that X direction
		}
		break;
	}
	if (sp.protectionOn == true){
		Beep(700, 100);
		sp.protectionCount--;
		if (sp.protectionCount == 0){
			sp.protectionOn = false;
		}
	}

	for (int counter = 0; counter < noOfPills; counter++){
		if ((pills.at(counter).destroyed == true)){
			pills.at(counter).historyState.push_back(1);
		}
		else{
			pills.at(counter).historyState.push_back(0);
		}
	}

	sp.historyX.push_back(sp.x);
	sp.historyY.push_back(sp.y);
	sp.historyState.push_back(0);
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
		dy = -1;	//increase the Y coordinatee
		break;
	case DOWN:	//when DOWN arrow is pressed...
		dx = 0;
		dy = +1;	//decrease the Y coordinate
		break;
	}
} //end of setKeyDirection

int getKeyPress()
{ //get key or command selected by user
	int keyPressed = -1;
	time_t timer;
	time(&timer);
	double duration = 0;
	if (kbhit()){
		keyPressed = toupper(getch());      //read in the selected arrow key or command letter
	}
	while ((keyPressed == 224 || keyPressed == -1) && duration < 1){     //ignore symbol following cursor key
		if (kbhit()){
			keyPressed = toupper(getch());

		}
		else{
			keyPressed = -1;
		}
		time_t now;
		time(&now);
		duration = difftime(timer, now)*-1;
	}
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
bool wantToSave(int key){
	return(key == SAVE);
}
bool wantToLoad(int key){
	return(key == LOAD);
}
bool wantToPlay(int key)
{ //check if key 'P' is pressed
	return (key == PLAY);
}
bool wantToReplay(int key){
	return (key == REPLAY);
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

void renderGame(const char gd[][SIZEX], string mess, string playerName, Item spot)
{ //display game title, messages, maze, spot and apples on screen
	void paintGrid(const char g[][SIZEX], bool magicProtection);
	void showTitle(string);
	void showOptions();
	void showMessage(string);

	bool magicProtection = spot.protectionOn;

	Gotoxy(0, 0);
	//display grid contents
	paintGrid(gd, magicProtection);
	//display game title
	showTitle(playerName);
	//display menu options available
	showOptions();
	//display message if any
	showMessage(mess);
} //end of paintGame

void paintGrid(const char g[][SIZEX], bool magicProtection)
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)      //for each row (vertically)
	{
		for (int col(0); col < SIZEX; ++col)  //for each column (horizontally)
		{
			switch (g[row][col])
			{
			case ZOMBIE:
				SelectTextColour(clGreen);
				break;
			case PILL:
				SelectTextColour(clYellow);
				break;
			case SPOT:
				if (magicProtection == false)
					SelectTextColour(clBlue);
				else{
					SelectTextColour(clMagenta);
				}
				break;
			case HOLE:
				SelectTextColour(clRed);
				break;
			case TELEPORT:
				SelectTextColour(clCyan);
				break;
			default:
				SelectTextColour(clWhite);
				break;
			}
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

void noZombiesLeft(int countPills, int& levelNo, string playerName)
{ //end program with appropriate message
	levelNo++;
	SelectBackColour(clBlack);
	SelectTextColour(clYellow);
	Gotoxy(40, 10);
	cout << "YOU WON WITH " << countPills << " pills remaining!";
	if (levelNo <= 3)
	{
		Clrscr();
		enterGame(playerName, levelNo);
	}
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 12);
	//cin.clear();
	system("pause");
} //end of noZombiesLeft