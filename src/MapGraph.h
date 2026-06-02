#pragma once

#include <vector>

bool isJunction(int fieldIndex);

bool isNearJunction(int fieldIndex);

void addTwoWay(int a, int b);

void addOneWay(int from, int to);

void addSequence(int start, int end);

void removeOneWay(int from, int to);

void makeOneWay(int start, int end);

void initGraph();

void printGraph();

std::vector<int> findShortestPath(int startIndex, int destinationIndex);
