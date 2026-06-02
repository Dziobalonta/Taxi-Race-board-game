#pragma once
#include <vector>

void initGame();
void setZoneFromList(const std::vector<int>& fields, char zone);
void initZones();

int getNextTrafficState(int currentState, char zone);
int getCurrentZoneState(char zone);

bool isZoneLockedByEvent(char zone, int& forcedColor);
void activateEvent(int id);
void updateEvents();

void updateTraffic(int currentRound);
void updateBarriers();