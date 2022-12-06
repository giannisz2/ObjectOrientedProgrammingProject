#include <iostream>
#include <vector>
#include <typeinfo>

#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>


#include "classes.h"
#include "functions_def.h"
#include "types.h"


using namespace std;

int main() {

	unsigned int rows, columns; // Size of map

	cout << "Give size of map which will be N x N ( N >= 5 )." << endl;
	cin >> rows >> columns;
	while (rows != columns and rows < 5 and columns < 5) {
		cout << "Only N x N is allowed." << endl;
		cin >> rows >> columns;
	}

	char** Map = mapCreate(rows, columns); // create the map

	bool team = chooseTeam(); // function to choose team

	// those types are included in types.h file 
	warewolfVector vecW;
	vampireVector vecV;
	treeVector vecT;
	riverVector vecR;

	srand(time(NULL)); // give rand() seed

	int HP = rand() % (10 - 5 + 1) + 5; // get number bewteen 5 and 10

	for (unsigned int i = 0; i < rows; i++) { // get numbers between intervals using specific formula
		WareWolf wolf({ i,0 }, HP, (rand() % (2 - 0 + 1) + 0), (rand() % (3 - 1 + 1) + 1), (rand() % (2 - 1 + 1) + 1));
		vecW.push_back(wolf);

		Vampire vamp({ i,columns - 2 }, HP, (rand() % (2 - 0 + 1) + 0), (rand() % (3 - 1 + 1) + 1), (rand() % (2 - 1 + 1) + 1));
		vecV.push_back(vamp);

	}

	for (unsigned int i = 0; i < (rows*columns)/5; i++) {
		Tree tree({ rand() % (rows - 1) + 1, rand() % (columns - 1) + 1});
		// we restrict the placement to not collied with avatar
		vecT.push_back(tree);

		River river({ rand() % (rows - 1) + 1, rand() % (columns - 1) + 1});
		// we restrict the placement to not collied with avatar
		vecR.push_back(river);
	}

	placeHazards(Map, vecT, vecR, rows, columns); // place hazards (Trees and Rivers)

	Avatar avatar({ rows / 2, columns / 2 }, team, 1, true); // Create the avatar and place it
	Map[avatar.getCoord().x][avatar.getCoord().y] = avatar.getName();

	placeCharacters(Map, vecW, vecV, rows, columns);
	//pause(vecW, vecV);
	printMap(Map, rows, columns);

	//system("cls");
	pause(vecW, vecV);

	printMap(Map, rows, columns);

}
