#include <iostream>
#include <vector>

#include <conio.h>
#include <windows.h>

#include "classes.h"
#include "types.h"

using namespace std;



char** mapCreate(int rows, int columns) {
	char** Map = new char* [rows]; // Dynamically create a 2D array which will be the terrain
	for (int i = 0; i < rows; i++) {
		Map[i] = new char[columns];
	}

	for (int i = 0; i < rows; i++) { // initialize map
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
	while (team != 'W' and team != 'w' and team != 'V' and team != 'v') { // error 
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

			if ((counter_T - vecT.size()) != 0) { // if we haven't placed every object
				Tree tree = vecT.at(counter_T);
				if (tree.getCoord().x == i and tree.getCoord().y == j and
					// restrictions for tree placement, to not collide with avatar who has those coordinates
					(tree.getCoord().x != (rows / 2) or tree.getCoord().y == (columns / 2))) {

					Map[i][j] = tree.getName();


					counter_T++;
					continue; // if you place a tree, continue because we don't want a river on top of the tree
				}
			}


			if ((counter_R - vecR.size()) != 0) {
				River river = vecR.at(counter_R);
				if (river.getCoord().x == i and river.getCoord().y == j and
					// restrictions for river placement, to not collide with avatar who has those coordinates
					(river.getCoord().x != (rows / 2) or river.getCoord().y == (columns / 2))) {

					Map[i][j] = river.getName();

					counter_R++;

				}
			}
		}
	}
	vecR.clear(); // clear vector, we don't need those objects anymore
	vecT.clear(); // clear vector, we don't need those objects anymore
}



void placeCharacters(char** Map, warewolfVector& vecW, vampireVector& vecV, int rows, int columns) {

	int counter_W = 0;
	int counter_V = 0;

	for (int i = 0; i < rows; i++) {

		if (counter_W - vecW.size() != 0) { // if we haven't placed every object

			Map[i][0] = vecW.at(counter_W).getName();
			counter_W++;

		}

		// we have commented this because when vampires appear, for some reason their 'V' characters is 
		// not removed when they move and they stay put in the last column

		/*if (counter_V - vecV.size() != 0) {

			Map[i][columns - 1] = vecV.at(counter_V).getName();
			counter_V++;
		}*/
	}
}

void getActionAvatar(char** Map, Avatar& avatar, warewolfVector& vecW, vampireVector& vecV, int rows, int columns, int HP) {
	long current_tick, two_second_delay = (GetTickCount64() + 400);
	char keydown = 'k';

	do {
		if (_kbhit()) {
			keydown = _getch();
		}

		unsigned int avatars_x = avatar.getCoord().x;
		unsigned int avatars_y = avatar.getCoord().y;

		if (keydown == key_W) {
			// if next coordinate is out of bounds or it's not '.' or 'P' then don't you can't move there
			if (avatars_x - 1 <= 0 or (Map[avatars_x - 1][avatars_y] != '.' and Map[avatars_x - 1][avatars_y] != 'P')) { // if out of board, return
				return;
			}

			if (Map[avatars_x - 1][avatars_y] == 'P') // if next coordinate is potion then get it
				avatar.addPotion();

			Map[avatars_x][avatars_y] = '.'; // in avatar's coordinate put '.'
			avatar.move({ (unsigned int)(avatars_x - 1), (unsigned int)(avatars_y) }); // move the avatar
			avatars_x = avatar.getCoord().x; 
			avatars_y = avatar.getCoord().y;
			Map[avatars_x][avatars_y] = avatar.getName(); // place 'A' in current avatar's coordinates

		}
		else if (keydown == key_S) {
			if (avatars_x + 1 > (rows - 1) or (Map[avatars_x + 1][avatars_y] != '.' and Map[avatars_x + 1][avatars_y] != 'P')) {
				return;
			}

			if (Map[avatars_x + 1][avatars_y] == 'P')
				avatar.addPotion();

			Map[avatars_x][avatars_y] = '.';
			avatar.move({ (unsigned int)(avatars_x + 1), (unsigned int)(avatars_y) });
			avatars_x = avatar.getCoord().x;
			avatars_y = avatar.getCoord().y;
			Map[avatars_x][avatars_y] = avatar.getName();


		}
		else if (keydown == key_D) {
			if (avatars_y + 1 > (columns - 1) or (Map[avatars_x][avatars_y + 1] != '.' and Map[avatars_x][avatars_y + 1] != 'P')) {
				return;
			}

			if (Map[avatars_x][avatars_y + 1] == 'P') 
				avatar.addPotion();

			Map[avatars_x][avatars_y] = '.';
			avatar.move({ (unsigned int)(avatars_x), (unsigned int)(avatars_y + 1) });
			avatars_x = avatar.getCoord().x;
			avatars_y = avatar.getCoord().y;
			Map[avatars_x][avatars_y] = avatar.getName();


		}
		else if (keydown == key_A) {
			if (avatars_y - 1 <= 0 or (Map[avatars_x][avatars_y - 1] != '.' and Map[avatars_x][avatars_y - 1] != 'P')) {
				return;
			}

			if (Map[avatars_x][avatars_y - 1] == 'P')
				avatar.addPotion();

			Map[avatars_x][avatars_y] = '.';
			avatar.move({ (unsigned int)(avatars_x), (unsigned int)(avatars_y - 1) });
			avatars_x = avatar.getCoord().x;
			avatars_y = avatar.getCoord().y;
			Map[avatars_x][avatars_y] = avatar.getName();

		}
		else if (keydown == key_E) { // if player presses E, then avatar heals
			bool team = avatar.getTeam();
			if (team == false and avatar.getDayState() == true and avatar.getPotions() > 0) { // for vampires
				for (vampireVector::iterator vamp = vecV.begin(); vamp != vecV.end(); vamp++) {
					if (vamp->getState() == true) { // if vampire is alive
						vamp->fullHP(HP); // get it to full hp
					}
				}
				avatar.pickUpPotion(); // make this function true so the potion will be placed again
				avatar.removePotion(); // remove one potion from inventory
			}
			// same for warewolves
			else if (team == true and avatar.getDayState() == false and avatar.getPotions() > 0) {
				for (warewolfVector::iterator wolf = vecW.begin(); wolf != vecW.end(); wolf++) {
					if (wolf->getState() == true) {
						wolf->fullHP(HP);
					}
				}
				avatar.pickUpPotion();
				avatar.removePotion();
			}
		}
		current_tick = GetTickCount64();
	} while (current_tick < two_second_delay && keydown == 'k');
	system("cls");
	if (current_tick >= two_second_delay) {
		return;
	}
}

void moveWareWolves(char** Map, warewolfVector& vecW, vampireVector& vecV, int rows, int columns, int HP) {
	
	srand(time(NULL));
	
	for (warewolfVector::iterator wolf = vecW.begin(); wolf != vecW.end(); wolf++) {

		if (wolf->getState() == false) // if defeated, return
			continue;

		Coordinates coord = { 0,0 }; // initialize a point
		unsigned int wolf_x = wolf->getCoord().x; // get coords of warewolf
		unsigned int wolf_y = wolf->getCoord().y;


		int r = rand() % 4 + 1;

		switch (r) { // choose random coordinates for next move
		case 1:
			coord = { wolf_x - 1,wolf_y };
			break;
		case 2:
			coord = { wolf_x + 1,wolf_y };
			break;
		case 3:
			coord = { wolf_x,wolf_y - 1 };
			break;
		case 4:
			coord = { wolf_x,wolf_y + 1 };
		}

		if (coord.x < 0 or coord.x >= rows or coord.y < 0 or coord.y >= columns) { // if next random
			// cordinate is out of bounds, continue to next warewolf
			continue;
		}


		if (Map[coord.x][coord.y] == '.') { // if next coord is '.'(terrain) then move it there
			Map[wolf->getCoord().x][wolf->getCoord().y] = '.';
			wolf->move({ coord.x,coord.y });
			Map[coord.x][coord.y] = wolf->getName();

		}
		// all possible coordinates to check if an enemy is nearby
		if ((coord.x == 0 and coord.y == 0 and (Map[coord.x + 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V'))
			or (coord.x == 0 and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x + 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or ((coord.x > 0 and coord.x < rows - 1) and coord.y == 0 and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V' or Map[coord.x + 1][coord.y] == 'V'))
			or (coord.x == rows - 1 and coord.y == 0 and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V'))
			or (coord.x == rows - 1 and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or (coord.x == rows - 1 and coord.y == columns - 1 and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or ((coord.x > 0 and coord.x < rows - 1) and coord.y == columns - 1 and (Map[coord.x + 1][coord.y] == 'V' or Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or (coord.x == 0 and coord.y == columns - 1 and (Map[coord.x + 1][coord.y] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or ((coord.x > 0 and coord.x < rows - 1) and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x + 1][coord.y] == 'V'
				or Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V' or Map[coord.x][coord.y - 1] == 'V'))) {


			for (vampireVector::iterator vamp = vecV.begin(); vamp != vecV.end(); vamp++) {

				if (vamp->getState() == false) // if vampire is defetead get to next vampire
					continue;

				unsigned int vamp_x = vamp->getCoord().x; // get coords of vamp
				unsigned int vamp_y = vamp->getCoord().y;

				if (coord.x != vamp_x or coord.y != vamp_y) // if the current vampire is not the one we encountered
					continue;

				else {

					if ((*wolf)[2] >= (*vamp)[2]) { // if wolf strengh is higher or equal to vamp 
						int diff = (*wolf)[2] - (*vamp)[3]; // difference of their HP
						vamp->loseHP(diff);
						if (diff < 0) break; // if defense is higher than strengh, go to next warewolf
						else {
							if ((*vamp)[0] <= 0) { // if HP is 0 or lower
								Map[vamp_x][vamp_y] = '.'; // remove 'V' from map and change it's state to defeated
								vamp->changeState();
							}
							break;
						}
					}
					else { // if strengh is lower move away. We check boundaries in this portion
						if (wolf->getCoord().x + 1 < rows - 1 and Map[wolf->getCoord().x + 1][wolf->getCoord().y] == '.') {
							Map[wolf->getCoord().x][wolf->getCoord().y] = '.';
							wolf->move({ wolf->getCoord().x + 1 , wolf->getCoord().y });
							Map[wolf->getCoord().x + 1][wolf->getCoord().y] = wolf->getName();
							break;
						}
						else if (wolf->getCoord().x - 1 > 0 and Map[wolf->getCoord().x - 1][wolf->getCoord().y] == '.') {
							Map[wolf->getCoord().x][wolf->getCoord().y] = '.';
							wolf->move({ wolf->getCoord().x - 1 , wolf->getCoord().y });
							Map[wolf->getCoord().x - 1][wolf->getCoord().y] = wolf->getName();
							break;
						}
						else if (wolf->getCoord().y + 1 < columns - 1 and Map[wolf->getCoord().x][wolf->getCoord().y + 1] == '.') {
							Map[wolf->getCoord().x][wolf->getCoord().y] = '.';
							wolf->move({ wolf->getCoord().x , wolf->getCoord().y + 1 });
							Map[wolf->getCoord().x][wolf->getCoord().y + 1] = wolf->getName();
							break;
						}
						else if (wolf->getCoord().y - 1 > 0 and Map[wolf->getCoord().x][wolf->getCoord().y - 1] == '.') {
							Map[wolf->getCoord().x][wolf->getCoord().y] = '.';
							wolf->move({ wolf->getCoord().x , wolf->getCoord().y - 1 });
							Map[wolf->getCoord().x][wolf->getCoord().y - 1] = wolf->getName();
							break;
						}
					}
				}
			}
		}
		// all possible coordinates to check if an ally is nearby
		else if ((coord.x == 0 and coord.y == 0 and (Map[coord.x + 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W'))
			or (coord.x == 0 and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x + 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or ((coord.x > 0 and coord.x < rows - 1) and coord.y == 0 and (Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W' or Map[coord.x + 1][coord.y] == 'W'))
			or (coord.x == rows - 1 and coord.y == 0 and (Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W'))
			or (coord.x == rows - 1 and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or (coord.x == rows - 1 and coord.y == columns - 1 and (Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or ((coord.x > 0 and coord.x < rows - 1) and coord.y == columns - 1 and (Map[coord.x + 1][coord.y] == 'W' or Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or (coord.x == 0 and coord.y == columns - 1 and (Map[coord.x + 1][coord.y] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or ((coord.x > 0 and coord.x < rows - 1) and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x + 1][coord.y] == 'W'
				or Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W' or Map[coord.x][coord.y - 1] == 'W'))) {

			for (warewolfVector::iterator wolf2 = vecW.begin(); wolf2 != vecW.end(); wolf2++) {
				if (coord.x != wolf2->getCoord().x or coord.y != wolf2->getCoord().y or (*wolf) == (*wolf2))
					// == operator is overloaded
					continue;
				if ((*wolf2)[0] <= HP and (*wolf)[1] > 0 and rand() % 2 == 0) { // if 2nd warewolf's HP is not full, and 1st wolf has a med, and also choose it randomly
					wolf2->heal();
					wolf->consumedMed();
				}
			}
		}
	} 
}

void moveVampires(char** Map, warewolfVector& vecW, vampireVector& vecV, int rows, int columns, int HP) {
	
	srand(time(NULL));
	
	// pretty much same as moveWareWolves(), so same comments
	for (vampireVector::iterator vamp = vecV.begin(); vamp!= vecV.end(); vamp++) {

		if (vamp->getState() == false)
			continue;

		Coordinates coord = { 0,0 };
		unsigned int vamp_x = vamp->getCoord().x;
		unsigned int vamp_y = vamp->getCoord().y;

		int r = rand() % 8 + 1;

		switch (r) { // add diagonal coordinates in random movement
		case 1:
			coord = { vamp_x - 1,vamp_y };
			break;
		case 2:
			coord = { vamp_x + 1,vamp_y };
			break;
		case 3:
			coord = { vamp_x,vamp_y - 1 };
			break;
		case 4:
			coord = { vamp_x,vamp_y + 1 };
		case 5:
			coord = { vamp_x - 1, vamp_y - 1 };
		case 6:
			coord = { vamp_x - 1, vamp_y + 1 };
		case 7:
			coord = { vamp_x + 1, vamp_y - 1 };
		case 8:
			coord = { vamp_x + 1, vamp_y + 1 };
		}

		if (coord.x < 0 or coord.x >= rows or coord.y < 0 or coord.y >= columns) {
			continue;
		}


		if (Map[coord.x][coord.y] == '.') {
			Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
			vamp->move(coord);
			Map[coord.x][coord.y] = vamp->getName();
		}

		if ((coord.x == 0 and coord.y == 0 and (Map[coord.x + 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W'))
			or (coord.x == 0 and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x + 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or ((coord.x > 0 and coord.x < rows - 1) and coord.y == 0 and (Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W' or Map[coord.x + 1][coord.y] == 'W'))
			or (coord.x == rows - 1 and coord.y == 0 and (Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W'))
			or (coord.x == rows - 1 and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or (coord.x == rows - 1 and coord.y == columns - 1 and (Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or ((coord.x > 0 and coord.x < rows - 1) and coord.y == columns - 1 and (Map[coord.x + 1][coord.y] == 'W' or Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or (coord.x == 0 and coord.y == columns - 1 and (Map[coord.x + 1][coord.y] == 'W' or Map[coord.x][coord.y - 1] == 'W'))
			or ((coord.x > 0 and coord.x < rows - 1) and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x + 1][coord.y] == 'W'
				or Map[coord.x - 1][coord.y] == 'W' or Map[coord.x][coord.y + 1] == 'W' or Map[coord.x][coord.y - 1] == 'W'
				or Map[coord.x - 1][coord.y - 1] == 'W' or Map[coord.x + 1][coord.y + 1] == 'W' or Map[coord.x + 1][coord.y - 1] == 'W' or Map[coord.x - 1][coord.y + 1] == 'W'))) {

			for (warewolfVector::iterator wolf = vecW.begin(); wolf != vecW.end(); wolf++) {

				if (wolf->getState() == false)
					continue;

				unsigned int wolf_x = wolf->getCoord().x;
				unsigned int wolf_y = wolf->getCoord().y;

				if (coord.x != wolf_x or coord.y != wolf_y) {
					continue;
				}

				else {

					if ((*vamp)[2] >= (*wolf)[2]) {
						int diff = (*vamp)[2] - (*wolf)[3];
						wolf->loseHP(diff);
						if (diff < 0) break; 
						else {
							if ((*wolf)[0] <= 0) {
								Map[wolf_x][wolf_y] = '.';
								wolf->changeState();
							}
							break;
						}
					}
				else {
					if (vamp->getCoord().x + 1 < rows - 1 and Map[vamp->getCoord().x + 1][vamp->getCoord().y] == '.') {
						Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
						vamp->move({ vamp->getCoord().x + 1 , vamp->getCoord().y });
						Map[vamp->getCoord().x + 1][vamp->getCoord().y] = vamp->getName();
						break;
					}
					else if (vamp->getCoord().x - 1 > 0 and Map[vamp->getCoord().x - 1][vamp->getCoord().y] == '.') {
						Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
						vamp->move({ vamp->getCoord().x - 1 , vamp->getCoord().y });
						Map[vamp->getCoord().x - 1][vamp->getCoord().y] = vamp->getName();
						break;
					}
					else if (vamp->getCoord().y + 1 < columns - 1 and Map[vamp->getCoord().x][vamp->getCoord().y + 1] == '.') {
						Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
						vamp->move({ vamp->getCoord().x , vamp->getCoord().y + 1 });
						Map[vamp->getCoord().x][vamp->getCoord().y + 1] = vamp->getName();
						break;
					}
					else if (vamp->getCoord().y - 1 > 0 and Map[vamp->getCoord().x][vamp->getCoord().y - 1] == '.') {
						Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
						vamp->move({ vamp->getCoord().x , vamp->getCoord().y - 1 });
						Map[vamp->getCoord().x][vamp->getCoord().y - 1] = vamp->getName();
						break;
					}
					else if (vamp->getCoord().x - 1 > 0 and vamp->getCoord().y - 1 > 0 and
						Map[vamp->getCoord().x - 1][vamp->getCoord().y] == '.' and
						Map[vamp->getCoord().x][vamp->getCoord().y - 1] == '.') {

						Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
						vamp->move({ vamp->getCoord().x - 1 , vamp->getCoord().y - 1 });
						Map[vamp->getCoord().x - 1][vamp->getCoord().y - 1] = vamp->getName();
						break;
					}
					else if (vamp->getCoord().x + 1 < rows - 1 and vamp->getCoord().y - 1 > 0 and
						Map[vamp->getCoord().x + 1][vamp->getCoord().y] == '.' and
						Map[vamp->getCoord().x][vamp->getCoord().y - 1] == '.') {

						Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
						vamp->move({ vamp->getCoord().x + 1 , vamp->getCoord().y - 1 });
						Map[vamp->getCoord().x + 1][vamp->getCoord().y - 1] = vamp->getName();
						break;
					}
					else if (vamp->getCoord().x - 1 > 0 and vamp->getCoord().y + 1 < columns - 1 and
						Map[vamp->getCoord().x - 1][vamp->getCoord().y] == '.' and
						Map[vamp->getCoord().x][vamp->getCoord().y + 1] == '.') {

						Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
						vamp->move({ vamp->getCoord().x - 1 , vamp->getCoord().y + 1 });
						Map[vamp->getCoord().x - 1][vamp->getCoord().y + 1] = vamp->getName();
						break;
					}
					else if (vamp->getCoord().x + 1 < rows - 1 and vamp->getCoord().y + 1 < columns - 1 and
						Map[vamp->getCoord().x + 1][vamp->getCoord().y] == '.' and
						Map[vamp->getCoord().x][vamp->getCoord().y + 1] == '.') {

						Map[vamp->getCoord().x][vamp->getCoord().y] = '.';
						vamp->move({ vamp->getCoord().x + 1 , vamp->getCoord().y + 1 });
						Map[vamp->getCoord().x + 1][vamp->getCoord().y + 1] = vamp->getName();
						break;
						}
					}
				}
			}
		}
		else if ((coord.x == 0 and coord.y == 0 and (Map[coord.x + 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V'))
			or (coord.x == 0 and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x + 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or ((coord.x > 0 and coord.x < rows - 1) and coord.y == 0 and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V' or Map[coord.x + 1][coord.y] == 'V'))
			or (coord.x == rows - 1 and coord.y == 0 and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V'))
			or (coord.x == rows - 1 and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or (coord.x == rows - 1 and coord.y == columns - 1 and (Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or ((coord.x > 0 and coord.x < rows - 1) and coord.y == columns - 1 and (Map[coord.x + 1][coord.y] == 'V' or Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or (coord.x == 0 and coord.y == columns - 1 and (Map[coord.x + 1][coord.y] == 'V' or Map[coord.x][coord.y - 1] == 'V'))
			or ((coord.x > 0 and coord.x < rows - 1) and (coord.y > 0 and coord.y < columns - 1) and (Map[coord.x + 1][coord.y] == 'V'
			or Map[coord.x - 1][coord.y] == 'V' or Map[coord.x][coord.y + 1] == 'V' or Map[coord.x][coord.y - 1] == 'V'
			or Map[coord.x - 1][coord.y - 1] == 'V' or Map[coord.x + 1][coord.y + 1] == 'V' or Map[coord.x + 1][coord.y - 1] == 'V' or Map[coord.x - 1][coord.y + 1] == 'V'))) {
		
			for (vampireVector::iterator vamp2 = vecV.begin(); vamp2 != vecV.end(); vamp2++) {
				if (coord.x != vamp2->getCoord().x or coord.y != vamp2->getCoord().y or (*vamp) == (*vamp2))
					continue;
				if ((*vamp2)[0] < HP and (*vamp)[1] > 0 and rand() % 2 == 0) {
					vamp2->heal();
					vamp->consumedMed();
				}
			}
		}
	}
}

void placePotion(char** Map, Avatar& avatar, int rows, int columns) {
	srand(time(NULL));
	while (avatar.getPotions() < 2) {
		unsigned int x = rand() % (rows - 1) + 1;
		unsigned int y = rand() % (columns - 1) + 1;
		if (Map[x][y] == '.') {
			Potion* potion = new Potion({ x,y });
			Map[x][y] = potion->getName();
			avatar.pickUpPotion();
			delete potion;
			break;

		}
	}
}

void pause(Avatar avatar, warewolfVector& vecW, vampireVector& vecV, bool team) {

	system("cls");
	int defeatedWarewolves = 0;
	int defeatedVampires = 0;

	if (team) // if true, then by default, true means Warewolves
		cout << "You support: " << "Warewolves." << endl << endl;
	else
		cout << "You support: " << "Vampires." << endl << endl;

	cout << "You have: " << avatar.getPotions() << " potions." << endl << endl;

	for (int i = 0; i < vecW.size(); i++) {
		WareWolf wolf = vecW.at(i);

		if (wolf.getState() == true) { // if alive
			cout << "|Warewolf stats ->" "HP: " << wolf[0] << " Meds: " << wolf[1] << " STR: "
				<< wolf[2] << " DEF: " << wolf[3] << ".|" << endl;
		}
		else { // if defeated
			defeatedWarewolves++;
			cout << "*This warewolf is defeated.*" << endl;
		}

	}

	cout << endl << endl;

	for (int i = 0; i < vecV.size(); i++) {
		Vampire vamp = vecV.at(i);

		if (vamp.getState() == true) { // if alive
			cout << "|Vampire stats ->" "HP: " << vamp[0] << " Meds: " << vamp[1] << " STR: "
				<< vamp[2] << " DEF: " << vamp[3] << ".|" << endl;
		}
		else { // if defeated
			defeatedVampires++;
			cout << "*This vampire is defeated.*" << endl;
		}

	}

	if (defeatedWarewolves > defeatedVampires) {
		cout << "The vampires are winning." << endl;
	}
	else if (defeatedVampires > defeatedWarewolves) {
		cout << "The warewolves are winning." << endl;
	}

	cout << endl;
	if (avatar.getDayState() == true)
		cout << "It's day" << endl;
	else
		cout << "It's night" << endl;

	system("pause");
	return;
}
