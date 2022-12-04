#include <iostream>
#include <vector>

#include "classes.h"
#include "types.h"



char** mapCreate(int rows, int columns) {
	char** Map = new char* [rows]; // Create a 2D array which will be the Terrain
	for (int i = 0; i < rows; i++) {
		Map[i] = new char[columns];
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Map[i][j] = '.';
		}
	}

	return Map;
}




void printMap(char** Map, int rows, int columns) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << Map[i][j] << "  ";
		}
		cout << endl;
	}
}




bool chooseTeam() {
	char team;
	cout << "Choose your team. 'W' for Warewolfs and 'V' for Vampires." << endl;
	cin >> team;
	while (team != 'W' and team != 'w' and team != 'V' and team != 'v') {
		cout << "Wrong input. Try again." << endl;
		cin >> team;
	}

	bool t = (team == 'W' or team == 'w') ? true : false;

	if (t == true) {
		cout << "You chose the Warewolves." << endl;
	}
	else {
		cout << "You chose the Vampires." << endl;
	}

	return t;
}




void placeHazards(char** Map, treeVector& vecT, riverVector& vecR, int rows, int columns) {
	// putting Trees and Rivers on map
	int counter_T = 0;
	int counter_R = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if ((counter_T - vecT.size()) != 0) {
				Tree tree = vecT.at(counter_T);
				if (tree.getCoord().x == i and tree.getCoord().y == j and
					// restrictions for tree placement, to not collide with avatar who has those coordinates
					(tree.getCoord().x != (rows / 2) or tree.getCoord().y == (columns / 2))) {

					Map[i][j] = vecT.at(counter_T).getName();

					counter_T++;
					continue; // if you place a tree, continue because we don't want a river on top of the tree
				}
			}

			if ((counter_R - vecR.size()) != 0) {
				River river = vecR.at(counter_R);
				if (river.getCoord().x == i and river.getCoord().y == j and
					// restrictions for river placement, to not collide with avatar who has those coordinates
					(river.getCoord().x != (rows / 2) or river.getCoord().y == (columns / 2))) {

					Map[i][j] = vecR.at(counter_R).getName();

					counter_R++;

				}
			}
		}
	}
	vecR.clear(); // deallocate memory, we don't need those objects anymore
	vecT.clear(); // deallocate memory, we don't need those objects anymore
}



void placeCharacters(char** Map, warewolfVector& vecW, vampireVector& vecV, int rows, int columns) {

	int counter_W = 0;
	int counter_V = 0;

	for (int i = 0; i < rows; i++) {

		if (counter_W - vecW.size() != 0) {
			WareWolf wolf = vecW.at(counter_W);		
			Map[i][0] = vecW.at(counter_W).getName();
			counter_W++;
			
		}

		if (counter_V - vecV.size() != 0) {
			Vampire vamp = vecV.at(counter_V);		
			Map[i][columns-1] = vecV.at(counter_V).getName();
			counter_V++;
		}
	}
}

void getActionAvatar() {

}

void moveCharacters() {

}

void placePotions() {

}

void pause() {

}

void gameOver() {

}