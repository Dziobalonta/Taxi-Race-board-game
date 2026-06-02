#include "MapGraph.h"
#include "Config.h"

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

bool isJunction(int fieldIndex) {
    return mapGraph[fieldIndex].size() > 2;
}

bool isNearJunction(int fieldIndex) {
    // Look at all adjacent fields
    for (size_t i = 0; i < mapGraph[fieldIndex].size(); i++) {
        int neighbor = mapGraph[fieldIndex][i];
        if (isJunction(neighbor)) {
            return true;
        }
    }
    return false;
}

// Helper, adds two way connections - can create intersetions
void addTwoWay(int a, int b) {
    mapGraph[a - 1].push_back(b - 1);
    mapGraph[b - 1].push_back(a - 1);
}

void addOneWay(int from, int to) {
    mapGraph[from - 1].push_back(to - 1);
}

// Helper, adds long paths of fields
void addSequence(int start, int end) {
    for (int i = start; i < end; i++) {
        addTwoWay(i, i + 1);
    }
}

void removeOneWay(int from, int to) {
    // find a connection in vector
    auto it = find(mapGraph[from - 1].begin(), mapGraph[from - 1].end(), to - 1);
    
    // if found remove it
    if (it != mapGraph[from - 1].end()) {
        mapGraph[from - 1].erase(it);
    }
}

void makeOneWay(int start, int end) {

    if(start <= end){
        for (int i = start; i < end; i++) {
            removeOneWay(i, i + 1);
        }
    } else {
        for (int i = start; i > end; i--) {
            removeOneWay(i, i - 1);
        }
    }
}

void initGraph() {

    addSequence(1, 65);
    addSequence(66, 67);
    addSequence(68, 101);
    addSequence(102, 125);
    addSequence(126, 126);
    addSequence(127, 128);
    addSequence(129, 131);
    addSequence(132, 137);
    addSequence(138, 140);
    addSequence(141, 149);
    addSequence(150, 155);
    addSequence(156, 163);
    addSequence(164, 166);
    addSequence(167, 174);
    addSequence(175, 176);
    addSequence(177, 181);
    addSequence(182, 184);
    addSequence(185, 191);

    makeOneWay(68, 72);
    makeOneWay(141, 145);
    makeOneWay(98, 92);
    makeOneWay(98, 101);


    addTwoWay(16, 137);
    addTwoWay(16, 138);
    
    addTwoWay(19, 67);
    addOneWay(68, 19);

    addTwoWay(25, 65);
    addTwoWay(25, 66);

    addTwoWay(27, 140);
    addOneWay(141, 27);

    addTwoWay(33, 184);
    addTwoWay(33, 185);

    addTwoWay(45, 182);
    addTwoWay(45, 181);

    addTwoWay(57, 150);
    addTwoWay(57, 149);

    addTwoWay(63, 175);
    addTwoWay(63, 174);

    addTwoWay(72, 131);
    addTwoWay(72, 132);

    addTwoWay(80, 128);
    addTwoWay(80, 129);

    addTwoWay(86, 126);
    addTwoWay(86, 127);

    addTwoWay(92, 125);
    addTwoWay(92, 126);

    addTwoWay(98, 166);
    addTwoWay(98, 167);

    addTwoWay(114, 156);
    addTwoWay(114, 155);

    addTwoWay(118, 163);
    addTwoWay(118, 164);

    addTwoWay(135, 1);

    addTwoWay(139, 191);

    addTwoWay(145, 177);
    addTwoWay(145, 176);

    addTwoWay(153, 102);
    addOneWay(153, 101);
}

void printGraph() {
    cout << "--- VERIFYING MAP CONNECTIONS ---" << endl;
    
    for (int i = 0; i < ROAD_SIZE; i++) {

        cout << "Field " << (i + 1) << " connects to: [ ";
        
        // Loop through all connections for this specific field
        for (size_t j = 0; j < mapGraph[i].size(); j++) {
            
            // Add 1 to the neighbor's index to match the physical map
            cout << (mapGraph[i][j] + 1); 
            
            // Add a comma between numbers but not after the last one
            if (j < mapGraph[i].size() - 1) {
                cout << ", ";
            }
        }
        cout << " ]" << endl;
    }
}

// BFS for calculating the shortest path to destination point
// used to calcualte the taxi rate
// using 0-based indices (0 to 184)
vector<int> findShortestPath(int startIndex, int destinationIndex) {

    vector<int> parent(ROAD_SIZE, -1); // Keeps track of where we came from
    vector<bool> visited(ROAD_SIZE, false); // Keeps track of already checked fields 
    queue<int> q; // The BFS queue

    // 2. Start the BFS with the beginning field
    q.push(startIndex);
    visited[startIndex] = true;

    bool found = false;

    // Run the search
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // If reached the destination, stop searching
        if (current == destinationIndex) {
            found = true;
            break; 
        }

        // Check all neighboring fields
        for (size_t i = 0; i < mapGraph[current].size(); i++) {
            int neighbor = mapGraph[current][i];

            if (!visited[neighbor] && roadState[neighbor] != 5) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    // Flip the path
    vector<int> path;
    if (found) {
        int step = destinationIndex;
        while (step != -1) {
            path.push_back(step);
            step = parent[step];
        }
        // The path is currently backwards (Destination -> Start)
        reverse(path.begin(), path.end());
    }

    // Returns the path, empty if no path exists (on this board a path always exists)
    return path;
}

