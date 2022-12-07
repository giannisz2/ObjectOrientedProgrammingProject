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
void pause(warewolfVector&, vampireVector&);
void getActionAvatar(char**, Avatar, int, int);
void placePotions(char**, int, int);
void moveCharacters(char**, warewolfVector&, vampireVector&, i
