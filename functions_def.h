#pragma once

#include <iostream>
#include <vector>

#include "classes.h"

char** mapCreate(int, int);
void printMap(char**, int, int);
bool chooseTeam();
void placeHazards(char**, vector<Tree>&, vector<River>&, int, int);
