#include <iostream>
#include <vector>

#include <conio.h>
#include <Windows.h>

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
	vecR.clear(); // deallocate memory, we don't need those objects anymore
	vecT.clear(); // deallocate memory, we don't need those objects anymore
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
	long current_tick, two_second_delay = (GetTickCount64() + 5000);
	char keydown = 'k';
	printMap(Map, rows, columns);
	do {
		if (_kbhit()) {
			keydown = _getch();
		}
		unsigned int avatars_x = avatar.getCoord().x;
		unsigned int avatars_y = avatar.getCoord().y;
		if (keydown == key_W) {
			if (avatars_x - 1 == -1 or Map[avatars_x - 1][avatars_y] != '.') {
				return;
			}
			if(Map[avatars_x - 1][avatars_y] == 'P') {
				Map[avatars_x - 1][avatars_y] = '.';
				avatar.addPotion();
			}
			else {
				Map[avatars_x][avatars_y] = '.';
				avatar.move({ (unsigned int)(avatars_x - 1), (unsigned int)(avatars_y) });
				avatars_x = avatar.getCoord().x;
				avatars_y = avatar.getCoord().y;
				Map[avatars_x][avatars_y] = avatar.getName();
			}
		}
		else if (keydown == key_S) {
			if (avatars_x + 1 > (rows - 1) or Map[avatars_x + 1][avatars_y] != '.') {
				return;
			}
			if(Map[avatars_x + 1][avatars_y] == 'P') {
				Map[avatars_x - 1][avatars_y] = '.';
				avatar.addPotion();
			}
			else {
				Map[avatars_x][avatars_y] = '.';
				avatar.move({ (unsigned int)(avatars_x + 1), (unsigned int)(avatars_y) });
				avatars_x = avatar.getCoord().x;
				avatars_y = avatar.getCoord().y;
				Map[avatars_x][avatars_y] = avatar.getName();
			}
		}
		else if (keydown == key_D) {
			if (avatars_y + 1 > (columns - 1) or Map[avatars_x][avatars_y + 1] != '.') {
				return;
			}
			if(Map[avatars_x][avatars_y + 1] == 'P') {
				Map[avatars_x - 1][avatars_y] = '.';
				avatar.addPotion();
			}
			else {
				Map[avatars_x][avatars_y] = '.';
				avatar.move({ (unsigned int)(avatars_x), (unsigned int)(avatars_y + 1) });
				avatars_x = avatar.getCoord().x;
				avatars_y = avatar.getCoord().y;
				Map[avatars_x][avatars_y] = avatar.getName();
			}
		}
		else if (keydown == key_A) {
			if (avatars_y - 1 == 0 or Map[avatars_x][avatars_y - 1] != '.') {
				return;
			}
			if(Map[avatars_x][avatars_y - 1] == 'P') {
				Map[avatars_x - 1][avatars_y] = '.';
				avatar.addPotion();
			}
			else {
				Map[avatars_x][avatars_y] = '.';
				avatar.move({ (unsigned int)(avatars_x), (unsigned int)(avatars_y - 1) });
				avatars_x = avatar.getCoord().x;
				avatars_y = avatar.getCoord().y;
				Map[avatars_x][avatars_y] = avatar.getName();
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
			}
			else if (team == true and avatar.getDayState() == false and avatar.getPotions() > 0) {
				for (int i = 0; i < vecW.size(); i++) {
					if (vecW.at(i)[4] == true) {
						vecW.at(i).fullHP(HP);
					}
				}
			}
		}
		current_tick = GetTickCount64();
	} while (current_tick < two_second_delay && keydown == 'k');
	system("cls");
	if (current_tick >= two_second_delay) {
		return;
	}
}




void moveWareWolves(char** Map, warewolfVector& vecW, int rows, int columns) {
	srand(time(NULL));
	int i = 0;
	
	vector<WareWolf>::iterator itr;
	for (itr = vecW.begin(); itr != vecW.end(); itr++) {

		Coordinates coord = { 0,0 };
		unsigned int wolf_x = vecW.at(i).getCoord().x;
		unsigned int wolf_y = vecW.at(i).getCoord().y;

		int r = rand() % 4 + 1;
		
		switch (r) {
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

				if (vecV.at(j)[4] != false) {
					unsigned int vamp_x = vecV.at(j).getCoord().x;
					unsigned int vamp_y = vecV.at(j).getCoord().y;

					if (wolf_x != vamp_x or wolf_y != vamp_y) {
						continue;
					}

					else {

						if (vecW.at(i)[2] >= vecV.at(j)[2]) { // if wolf strenght is higher or equal to vamp 
							int diff = vecW.at(i)[2] - vecV.at(j)[3];
							vecV.at(j).loseHP(diff);
							if (diff < 0) break;
							else {
								if (vecV.at(j)[0] <= 0) {
									Map[vamp_x][vamp_y] = '.';
									vecV.at(j).changeState();	
								}
								break;
							}
						}
						else {
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
			}
			j++;
		}
		/*else if (Map[coord.x][coord.y] == 'W') {


		}*/
		i++;
	}
}



void placePotion(char** Map, Avatar& avatar, int rows, int columns) {
	bool put_potion = false;
	while (put_potion == false and avatar.getPotions() > 0 and  avatar.getPotions() < 3) {
		unsigned int x = rand() % (rows - 1) + 1;
		unsigned int y = rand() % (columns - 1) + 1;
		if (Map[x][y] == '.') {
			Potion potion({x, y});
			Map[x][y] = potion.getName();
			put_potion = true;
		}
	}
	return;
}




void pause(Avatar avatar, warewolfVector& vecW, vampireVector& vecV) {

	system("cls");
	int defeatedWarewolves = 0;
	int defeatedVampires = 0;

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
