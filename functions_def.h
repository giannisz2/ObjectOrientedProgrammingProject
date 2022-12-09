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
void pause(Avatar, warewolfVector&, vampireVector&);
void getActionAvatar(char**, Avatar&, warewolfVector&, vampireVector&, int, int, int);
void placePotion(char**, int, int);
void moveWareWolves(char**, warewolfVector&, vampireVector&, i)
