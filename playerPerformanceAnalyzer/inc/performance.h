#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "player.h"

float computePerformanceIndex(const Player *player);
void computePerformanceForAll(Player players[], int playersCount);

#endif
