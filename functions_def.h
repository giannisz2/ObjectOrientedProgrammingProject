#pragma once

#include <iostream>
#include <vector>

#include "classes.h"
#include "types.h"

char** mapCreate(int, int);
void printMap(char**, int, int);
bool chooseTeam();
void placeHazards(char**, treeVector&, riverVector&, int, int);
void placeCharacters(char**, warewolfVector&, vampireVector&, int, int);
void pause(Avatar, warewolfVector&, vampireVector&, bool);
void getActionAvatar(char**, Avatar&, warewolfVector&, vampireVector&, int, int, int);
void moveWareWolves(char**, warewolfVector&, vampireVector&, int, int, int);
void moveVampires(char**, warewolfVector&, vampireVector&, int, int, int);
void placePotion(char**, Avatar&, int, int);
