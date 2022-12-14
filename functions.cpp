#include <iostream>
#include <vector>

#include <conio.h>
#include <windows.h>

#include "classes.h"
#include "types.h"

using namespace std;



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

		if (counter_W - vecW.size() != 0) {
		
			Map[i][0] = vecW.at(counter_W).getName();
			counter_W++;
			
		}

		if (counter_V - vecV.size() != 0) {
				
			Map[i][columns-1] = vecV.at(counter_V).getName();
			counter_V++;
		}
	}
}

void getActionAvatar(char** Map, Avatar& avatar, warewolfVector& vecW, vampireVector& vecV, int rows, int columns, int HP) {
	long current_tick, two_second_delay = (GetTickCount64() + 300);
	char keydown = 'k';

	do {
		if (_kbhit()) {
			keydown = _getch();
		}
		unsigned int avatars_x = avatar.getCoord().x;
		unsigned int avatars_y = avatar.getCoord().y;
		if (keydown == key_W) {
			if (avatars_x - 1 <= 0 or (Map[avatars_x - 1][avatars_y] != '.' and Map[avatars_x - 1][avatars_y] != 'P')) { // if out of board, return
				return;
			}
			
			Map[avatars_x][avatars_y] = '.';
			avatar.move({ (unsigned int)(avatars_x - 1), (unsigned int)(avatars_y) });
			avatars_x = avatar.getCoord().x;
			avatars_y = avatar.getCoord().y;
			Map[avatars_x][avatars_y] = avatar.getName();

			if (Map[avatar.getCoord().x][avatar.getCoord().y] == 'P') { // get potion
				Map[avatar.getCoord().x][avatar.getCoord().y] = avatar.getName();
				avatar.addPotion();
			}
			
		}
		else if (keydown == key_S) {
			if (avatars_x + 1 > (rows - 1) or (Map[avatars_x - 1][avatars_y] != '.' and Map[avatars_x - 1][avatars_y] != 'P')) {
				return;
			}
			
			Map[avatars_x][avatars_y] = '.';
			avatar.move({ (unsigned int)(avatars_x + 1), (unsigned int)(avatars_y) });
			avatars_x = avatar.getCoord().x;
			avatars_y = avatar.getCoord().y;
			Map[avatars_x][avatars_y] = avatar.getName();

			if (Map[avatar.getCoord().x][avatar.getCoord().y] == 'P') { // get potion
				Map[avatar.getCoord().x][avatar.getCoord().y] = avatar.getName();
				avatar.addPotion();
			}
			
		}
		else if (keydown == key_D) {
			if (avatars_y + 1 > (columns - 1) or (Map[avatars_x - 1][avatars_y] != '.' and Map[avatars_x - 1][avatars_y] != 'P')) {
				return;
			}
			
			Map[avatars_x][avatars_y] = '.';
			avatar.move({ (unsigned int)(avatars_x), (unsigned int)(avatars_y + 1) });
			avatars_x = avatar.getCoord().x;
			avatars_y = avatar.getCoord().y;
			Map[avatars_x][avatars_y] = avatar.getName();

			if (Map[avatar.getCoord().x][avatar.getCoord().y] == 'P') { // get potion
				Map[avatar.getCoord().x][avatar.getCoord().y] = avatar.getName();
				avatar.addPotion();
			}
			
		}
		else if (keydown == key_A) {
			if (avatars_y - 1 <= 0 or (Map[avatars_x - 1][avatars_y] != '.' and Map[avatars_x - 1][avatars_y] != 'P')) {
				return;
			}
			
			Map[avatars_x][avatars_y] = '.';
			avatar.move({ (unsigned int)(avatars_x), (unsigned int)(avatars_y - 1) });
			avatars_x = avatar.getCoord().x;
			avatars_y = avatar.getCoord().y;
			Map[avatars_x][avatars_y] = avatar.getName();

			if (Map[avatar.getCoord().x][avatar.getCoord().y] == 'P') { // get potion
				Map[avatar.getCoord().x][avatar.getCoord().y] = avatar.getName();
				avatar.addPotion();
			}
			
		}
		else if (keydown == key_E) {
			bool team = avatar.getTeam();
			if (avatar.getDayState() == true and team == false and avatar.getPotions() > 0) {
				for (int i = 0; i < vecV.size(); i++) {
					if (vecV.at(i)[4] == true) {
						vecV.at(i).fullHP(HP);
					}
				}
				avatar.removePotion();
			}
			else if (team == true and avatar.getDayState() == false and avatar.getPotions() > 0) {
				for (int i = 0; i < vecW.size(); i++) {
					if (vecW.at(i)[4] == true) {
						vecW.at(i).fullHP(HP);
					}
				}
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
	int i = 0;
	
	vector<WareWolf>::iterator itr;
	for (itr = vecW.begin(); itr != vecW.end(); itr++) {

		if (vecW.at(i)[4] == false) // if defeated, return
			return;

		Coordinates coord = { 0,0 };
		unsigned int wolf_x = vecW.at(i).getCoord().x;
		unsigned int wolf_y = vecW.at(i).getCoord().y;

		int r = rand() % 4 + 1;
		
		switch (r) { // choose next random movement
		case 1:
			coord = { wolf_x - 1,wolf_y };
			break;
		case 2:
			coord = { wolf_x + 1,wolf_y };
			break;
		case 3:
			coord = { wolf_x,wolf_y - 1};
			break;
		case 4:
			coord = { wolf_x,wolf_y + 1 };
		}
		
		if (coord.x < 0 or coord.x >= rows or coord.y < 0 or coord.y >= columns) {
			return;
		}

		if (Map[coord.x][coord.y] == '.') {
			Map[vecW.at(i).getCoord().x][vecW.at(i).getCoord().y] = '.';
			vecW.at(i).move(coord);
			Map[coord.x][coord.y] = vecW.at(i).getName();
			
		}

		else if (Map[coord.x][coord.y] == 'V') {
			int j = 0;
			vector<Vampire>::iterator itr2;
			for (itr2 = vecV.begin(); itr2 != vecV.end(); itr2++) {

				if (vecV.at(j)[4] == false) // if vampire is defetead get to next vampire
					continue;

				unsigned int vamp_x = vecV.at(j).getCoord().x;
				unsigned int vamp_y = vecV.at(j).getCoord().y;

				if (wolf_x != vamp_x or wolf_y != vamp_y) {
					continue;
				}

				else {

					if (vecW.at(i)[2] >= vecV.at(j)[2]) { // if wolf strengh is higher or equal to vamp 
						cout << "Yes" << endl;
						int diff = vecW.at(i)[2] - vecV.at(j)[3]; // difference of their HP
						vecV.at(j).loseHP(diff);
						if (diff < 0) break; // if defense is higher than strengh
						else {
							if (vecV.at(j)[0] <= 0) {
								Map[vamp_x][vamp_y] = '.';
								vecV.at(j).changeState();	
							}
							break;
						}
					}
					else { // if strengh is lower move away. We check boundaries in this portion
						if (vecW.at(i).getCoord().x + 1 == '.' and vecW.at(i).getCoord().x + 1 < rows) {
							Map[vecW.at(i).getCoord().x][vecW.at(i).getCoord().y] = '.';
							vecW.at(i).move({ vecW.at(i).getCoord().x + 1 , vecW.at(i).getCoord().y });
							Map[vecW.at(i).getCoord().x + 1][vecW.at(i).getCoord().y] = vecW.at(i).getName();
							break;
						}
						else if (vecW.at(i).getCoord().x - 1 == '.' and vecW.at(i).getCoord().x - 1 >= 0) {
							Map[vecW.at(i).getCoord().x][vecW.at(i).getCoord().y] = '.';
							vecW.at(i).move({ vecW.at(i).getCoord().x - 1 , vecW.at(i).getCoord().y });
							Map[vecW.at(i).getCoord().x - 1][vecW.at(i).getCoord().y] = vecW.at(i).getName();
							break;
						}
						else if (vecW.at(i).getCoord().y + 1 == '.' and vecW.at(i).getCoord().y + 1 < columns) {
							Map[vecW.at(i).getCoord().x][vecW.at(i).getCoord().y] = '.';
							vecW.at(i).move({ vecW.at(i).getCoord().x , vecW.at(i).getCoord().y + 1 });
							Map[vecW.at(i).getCoord().x][vecW.at(i).getCoord().y + 1] = vecW.at(i).getName();
							break;
						}
						else if (vecW.at(i).getCoord().y - 1 == '.' and vecW.at(i).getCoord().y - 1 >= 0) {
							Map[vecW.at(i).getCoord().x][vecW.at(i).getCoord().y] = '.';
							vecW.at(i).move({ vecW.at(i).getCoord().x , vecW.at(i).getCoord().y - 1 });
							Map[vecW.at(i).getCoord().x][vecW.at(i).getCoord().y - 1] = vecW.at(i).getName();
							break;
						}
					}
				}
			}
			j++;
		}
		else if (Map[coord.x][coord.y] == 'W') {
			vector<WareWolf>::iterator itr3;
			int k = 0;
			for (itr3 = vecW.begin(); itr3 != vecW.end(); itr3++) {
				if (coord.x != vecW.at(k).getCoord().x or coord.y != vecW.at(k).getCoord().y or vecW.at(i) == vecW.at(k)) 
					// == operator is overloaded,we basically check their coordinates and 
					// we don't want to heal the object itself, so we get the same in the loop then continue
					continue;
				if (vecW.at(k)[0] < HP and vecW.at(i)[1] > 0) {
					vecW.at(k).heal();
					vecW.at(i).consumedMed();
				}
				k++;
			}
		}
		i++;
	}
}

void moveVampires(char** Map, warewolfVector& vecW, vampireVector& vecV, int rows, int columns, int HP) {
	srand(time(NULL));
	int i = 0;

	vector<Vampire>::iterator itr;
	for (itr = vecV.begin(); itr != vecV.end(); itr++) {

		if (vecV.at(i)[4] == false)
			return;

		Coordinates coord = { 0,0 };
		unsigned int vamp_x = vecV.at(i).getCoord().x;
		unsigned int vamp_y = vecV.at(i).getCoord().y;

		int r = rand() % 8 + 1;

		switch (r) {
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
			return;
		}

		if (Map[coord.x][coord.y] == '.') {
			Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
			vecV.at(i).move(coord);
			Map[coord.x][coord.y] = vecV.at(i).getName();

		}

		else if (Map[coord.x][coord.y] == 'W') {
			int j = 0;
			vector<WareWolf>::iterator itr2;
			for (itr2 = vecW.begin(); itr2 != vecW.end(); itr2++) {

				if (vecW.at(j)[4] == false) // if vampire is defetaed get to next vampire
					continue;

				unsigned int wolf_x = vecW.at(j).getCoord().x;
				unsigned int wolf_y = vecW.at(j).getCoord().y;

				if (vamp_x != wolf_x or vamp_y != wolf_y) {
					continue;
				}

				else {

					if (vecV.at(i)[2] >= vecW.at(j)[2]) { // if wolf strenght is higher or equal to vamp 
						int diff = vecV.at(i)[2] - vecW.at(j)[3]; // difference of their HP
						vecW.at(j).loseHP(diff);
						if (diff < 0) break; // if defense is higher than strengh
						else {
							if (vecW.at(j)[0] <= 0) {
								Map[wolf_x][wolf_y] = '.';
								vecW.at(j).changeState();
							}
							break;
						}
					}
					else {
						if (vecV.at(i).getCoord().x + 1 == '.' and vecV.at(i).getCoord().x + 1 < rows) {
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
							vecV.at(i).move({ vecV.at(i).getCoord().x + 1 , vecV.at(i).getCoord().y });
							Map[vecV.at(i).getCoord().x + 1][vecV.at(i).getCoord().y] = vecV.at(i).getName();
							break;
						}
						else if (vecV.at(i).getCoord().x - 1 == '.' and vecV.at(i).getCoord().x - 1 >= 0) {
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
							vecV.at(i).move({ vecV.at(i).getCoord().x - 1 , vecV.at(i).getCoord().y });
							Map[vecV.at(i).getCoord().x - 1][vecV.at(i).getCoord().y] = vecV.at(i).getName();
							break;
						}
						else if (vecV.at(i).getCoord().y + 1 == '.' and vecV.at(i).getCoord().y + 1 < columns) {
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
							vecV.at(i).move({ vecV.at(i).getCoord().x , vecV.at(i).getCoord().y + 1 });
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y + 1] = vecV.at(i).getName();
							break;
						}
						else if (vecV.at(i).getCoord().y - 1 == '.' and vecV.at(i).getCoord().y - 1 >= 0) {
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
							vecV.at(i).move({ vecV.at(i).getCoord().x , vecV.at(i).getCoord().y - 1 });
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y - 1] = vecV.at(i).getName();
							break;
						}
						else if (vecV.at(i).getCoord().x - 1 == '.' and vecV.at(i).getCoord().x - 1 >= 0 and
							vecV.at(i).getCoord().y - 1 == '.' and vecV.at(i).getCoord().y >= 0) {
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
							vecV.at(i).move({ vecV.at(i).getCoord().x - 1 , vecV.at(i).getCoord().y - 1 });
							Map[vecV.at(i).getCoord().x - 1][vecV.at(i).getCoord().y - 1] = vecV.at(i).getName();
							break;
						}
						else if (vecV.at(i).getCoord().x - 1 == '.' and vecV.at(i).getCoord().x - 1 >= 0 and
							vecV.at(i).getCoord().y + 1 == '.' and vecV.at(i).getCoord().y + 1 < columns) {
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
							vecV.at(i).move({ vecV.at(i).getCoord().x - 1 , vecV.at(i).getCoord().y + 1 });
							Map[vecV.at(i).getCoord().x - 1][vecV.at(i).getCoord().y + 1] = vecV.at(i).getName();
							break;
						}
						else if (vecV.at(i).getCoord().x + 1 == '.' and vecV.at(i).getCoord().x + 1 < rows and
							vecV.at(i).getCoord().y - 1 == '.' and vecV.at(i).getCoord().y - 1 >= 0) {
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
							vecV.at(i).move({ vecV.at(i).getCoord().x + 1 , vecV.at(i).getCoord().y - 1 });
							Map[vecV.at(i).getCoord().x + 1][vecV.at(i).getCoord().y - 1] = vecV.at(i).getName();
							break;
						}
						else if (vecV.at(i).getCoord().x + 1 == '.' and vecV.at(i).getCoord().x + 1 < rows and
							vecV.at(i).getCoord().y + 1 == '.' and vecV.at(i).getCoord().y + 1 >= 0) {
							Map[vecV.at(i).getCoord().x][vecV.at(i).getCoord().y] = '.';
							vecV.at(i).move({ vecV.at(i).getCoord().x + 1 , vecV.at(i).getCoord().y + 1 });
							Map[vecV.at(i).getCoord().x + 1][vecV.at(i).getCoord().y + 1] = vecV.at(i).getName();
							break;
						}
					}
				}
			}
			j++;
		}
		else if (Map[coord.x][coord.y] == 'V') {
			vector<Vampire>::iterator itr3;
			int k = 0;
			for (itr3 = vecV.begin(); itr3 != vecV.end(); itr3++) {
				if (coord.x != vecV.at(k).getCoord().x or coord.y != vecV.at(k).getCoord().y or vecV.at(i) == vecV.at(k)) // == operator is overloaded
					continue;
				if (vecV.at(k)[0] < HP and vecV.at(i)[1] > 0) {
					vecV.at(k).heal();
					vecV.at(i).consumedMed();
				}
				k++;
			}
		}
		i++;
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
			delete potion;
			break;

		}
	}
}

void pause(Avatar avatar, warewolfVector& vecW, vampireVector& vecV, bool team) {

	system("cls");
	int defeatedWarewolves = 0;
	int defeatedVampires = 0;

	if(team)
		cout << "You support: " << "Warewolves." << endl << endl;
	else 
		cout << "You support: " << "Vampires." << endl << endl;

	cout << "You have: " << avatar.getPotions() << " potions." << endl << endl;

	for (int i = 0; i < vecW.size(); i++) {
		WareWolf wolf = vecW.at(i);

		if (wolf[4] == true) {
			cout << "|Warewolf stats ->" "HP: " << wolf[0] << " Meds: " << wolf[1] << " STR: "
				<< wolf[2] << " DEF: " << wolf[3] << ".|" << endl;
		}
		else {
			defeatedWarewolves++;
			cout << "*This warewolf is defeated.*" << endl;
		}

	}

	cout << endl << endl;
	
	for (int i = 0; i < vecV.size(); i++) {
		Vampire vamp = vecV.at(i);

		if (vamp[4] == true) {
			cout << "|Vampire stats ->" "HP: " << vamp[0] << " Meds: " << vamp[1] << " STR: "
				<< vamp[2] << " DEF: " << vamp[3] << ".|" << endl;
		}
		else {
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
