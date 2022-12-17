#include <iostream>
#include <vector>
#include <typeinfo>
#include <cstdlib>

#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>


#include "classes.h"
#include "functions_def.h"
#include "types.h"


using namespace std;

int main() {

	cout << "UP W   DOWN S   RIGHT D   LEFT A " << endl
		<< "PAUSE P    HEAL E   EXIT M" << endl
		<< "ONLY CAPITAL LETTERS WORK!!!" << endl;
	system("pause");
	system("cls");

	unsigned int rows, columns; // Size of map

	cout << "Give size of map which will be N x N ( N >= 5 )." << endl;
	cin >> rows >> columns;
	while (rows != columns and rows < 5 and columns < 5) { // error 
		cout << "Only N x N is allowed." << endl;
		cin >> rows >> columns;
	}

	char** Map = mapCreate(rows, columns); // create the map

	bool team = chooseTeam(); // function to choose team

	//                 ** INITIALIZE MAP STATE**                       //

	// those types are included in types.h file 
	warewolfVector vecW;
	vampireVector vecV;
	treeVector vecT;
	riverVector vecR;

	srand((unsigned)time(NULL)); // give rand() seed

	int HP = rand() % (10 - 5 + 1) + 5; // get number bewteen 5 and 10

	for (unsigned int i = 0; i < rows; i++) { // get numbers between intervals using specific formula
		WareWolf wolf({ i,0 }, HP, (rand() % (2 - 0 + 1) + 0), (rand() % (3 - 1 + 1) + 1), (rand() % (2 - 1 + 1) + 1));
		vecW.push_back(wolf);

		Vampire vamp({ i,columns - 1 }, HP, (rand() % (2 - 0 + 1) + 0), (rand() % (3 - 1 + 1) + 1), (rand() % (2 - 1 + 1) + 1));
		vecV.push_back(vamp);

	}

	for (unsigned int i = 0; i < (rows * columns) / 5; i++) {
		Tree tree({ rand() % (rows - 1) + 1, rand() % (columns - 2) + 2 });
		// we restrict the placement to not collide with avatar
		vecT.push_back(tree);

		River river({ rand() % (rows - 1) + 1, rand() % (columns - 2) + 2 });
		// we restrict the placement to not collide with avatar
		vecR.push_back(river);
	}

	placeHazards(Map, vecT, vecR, rows, columns); // place hazards (Trees and Rivers)

	Avatar avatar({ rows / 2, columns / 2 }, team, 1, true); // Create the avatar and place it
	Map[avatar.getCoord().x][avatar.getCoord().y] = avatar.getName();

	placeCharacters(Map, vecW, vecV, rows, columns);

	//**  MAIN LOOP **//

	bool warewolvesWon = false;
	bool vampiresWon = false;

	while (1) {

		int countDefWw = 0;
		for (warewolfVector::iterator wolf = vecW.begin(); wolf != vecW.end(); wolf++) {
			if (wolf->getState() == false)// count the warewols that are defeated
				countDefWw += 1;
		}

		int countDefVp = 0;
		for (vampireVector::iterator vamp = vecV.begin(); vamp != vecV.end(); vamp++) {
			if (vamp->getState() == false) // count those vampires that are defeated
				countDefVp += 1;

		}

		// flag so we can know who won
		if (countDefWw == vecW.size() - 1) {
			vampiresWon = true;
			break;
		}else if(countDefVp == vecV.size() - 1) {
			warewolvesWon = true;
			break;
		}

		// if player presses M the game is over, and if he presses P, the game is
		// paused. This code is a trick to set time
		// limit to function _getch() so when the player doesn't give input,
		// the game doesn't have to stop
		long current_tick, two_second_delay = (GetTickCount64() + 200);
		char keydown = 'k';

		do {
			system("cls");
			printMap(Map, rows, columns);
			if (_kbhit()) {
				keydown = _getch();
			}

			if (keydown == key_M) {
				cout << "You exit the game." << endl;
				return 0;
			}

			if (keydown == key_P) {
				pause(avatar, vecW, vecV, team);
			}

			current_tick = GetTickCount64();
		} while (current_tick < two_second_delay && keydown == 'k');


		system("cls");

		printMap(Map, rows, columns);
		getActionAvatar(Map, avatar, vecW, vecV, rows, columns, HP);
		moveWareWolves(Map, vecW, vecV, rows, columns, HP);
		moveVampires(Map, vecW, vecV, rows, columns, HP);

		if(avatar.getPotions() < 2 and avatar.getPickUpState() == false)
			placePotion(Map, avatar, rows, columns);

		if (rand() % 2 == 0) { // randomly change day
			avatar.changeDayState();
		}

		
	}

	if ((team == true and warewolvesWon == true) or (team == false and vampiresWon == true)) {
		cout << "Your team won!" << endl;
	}
	else if ((team == false and warewolvesWon == true) or (team == true and vampiresWon == true)) {
		cout << "Your team lost..." << endl;
	}

	vecV.clear();
	vecW.clear();

	return 0;
}
