#pragma once

#include <iostream>
#include <string>

using namespace std;

typedef struct Coordinates {
	unsigned int x;
	unsigned int y;
}Coordinates;

class Humanoid { // Hyperclass
protected:
	Coordinates point;
public:
	Humanoid(Coordinates coord) : point(coord) {};

	void move(Coordinates new_point) {
		point = new_point;
	}

	Coordinates getCoord() const {
		return point;
	}

	~Humanoid() {};
};


class Avatar : public Humanoid {
	char name = 'A';
	bool team; // true werewolves, false vampires
	int numPotions = 1;
	bool dayState = true; // true day, false night
public:
	Avatar(Coordinates coord, bool t, int n_P, bool day) : Humanoid(coord), team(t), numPotions(n_P), dayState(day) {};

	void print() {
		cout << point.x << " " << point.y << " " << team << " " << numPotions << endl;
	}

	bool getTeam() const {
		return team;
	}

	int getPotions() const {
		return numPotions;
	}

	char getName() {
		return name;
	}

	bool getDayState() {
		return dayState;
	}

	void changeDayState() {
		dayState = !dayState;
	}

	void addPotion() {
		numPotions += 1;
	}
	
	void removePotion() {
		numPotions -= 1;
	}

	~Avatar() {};
};

class Monster : public Humanoid { // subclass that inherit Humanoid hyperclass
	int HealthPoints; // m[0]
	int numMeds; // m[1]
	int strengthPoints; // m[2]
	int defencePoints; // m[3]
	bool state = true; // true if alive, false if defeated. m[4]
public:
	Monster(Coordinates coord, int HP, int n_M, int str, int def) :
		Humanoid(coord), HealthPoints(HP), numMeds(n_M), strengthPoints(str), defencePoints(def) {};

	void print() {
		cout << point.x << point.y << HealthPoints << numMeds << strengthPoints << defencePoints << endl;
	}

	const int& operator [] (int x) { // operator overloading
		switch (x) {
		case 0:
			return HealthPoints;
			break;
		case 1:
			return numMeds;
			break;
		case 2:
			return strengthPoints;
			break;
		case 3:
			return defencePoints;
		case 4:
			return state;
		default:
			return 11; // error 
		}
	}

	const bool& operator == (Monster mon) {
		if (point.x == mon.getCoord().x and point.y == mon.getCoord().y)
			return true;
		return false;
	}

	void fullHP(int HP) {
		HealthPoints = HP;
	}

	void loseHP(int n) {
		if (n > 0)
			HealthPoints -= n;
	}

	void heal() {
		HealthPoints += 1;
	}

	void consumedMed() {
		numMeds -= 1;
	}
	
	void changeState() {
		state = false;
	}

	~Monster() {};
};

class Vampire : public Monster {
	char name = 'V';
public:
	Vampire(Coordinates coord, int HP, int n_M, int str, int def) :
		Monster(coord, HP, n_M, str, def) {};

	char getName() { 
		return name;
	}

	~Vampire() {};

};


class WareWolf : public Monster {
	char name = 'W';
public:
	WareWolf(Coordinates coord, int HP, int n_M, int str, int def) :
		Monster(coord, HP, n_M, str, def) {};

	char getName() { 
		return name;
	}

	~WareWolf() {};

};




class Terrain {
	Coordinates point;
public:
	Terrain(Coordinates coords) : point(coords) {};

	void print() {
		cout << point.x << point.y << endl;
	}

	Coordinates getCoord() const {
		return point;
	}

	~Terrain() {};
};

class Tree : public Terrain {
	char name = 'T';
public:
	Tree(Coordinates coord) : Terrain(coord) {};

	char getName() {
		return name;
	}

	~Tree() {};
};

class River : public Terrain {
	char name = 'R';
public:
	River(Coordinates coord) : Terrain(coord) {};

	char getName() {
		return name;
	}

	~River() {};
};

class Potion {
	char name = 'P';
	Coordinates point;
public:
	Potion(Coordinates coord) : point(coord) {};

	Coordinates getCoord() const {
		return point;
	}

	char getName() {
		return name;
	}

	~Potion() {};
};
